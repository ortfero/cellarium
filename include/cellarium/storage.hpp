/* This file is part of cellarium library
 * Copyright 2020 Andrei Ilin <ortfero@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once


#include <filesystem>
#include <system_error>
#include <type_traits>
#include <memory>
#include <cstring>
#include <cstdio>
#include <new>

#include "file.hpp"
#include "mapped_file.hpp"
#include "header.hpp"
#include "record.hpp"
#include "error.hpp"


namespace cellarium {
  

  template<typename T>
  class storage {
  public:
  
    static_assert(std::is_trivial_v<T>, "Only trivial types can be stored");

    using path_type = std::filesystem::path;
    using size_type = header::size_type;
    using index_type = header::index_type;
    using value_type = T;
    
    using record_type = record<T>;

    static constexpr index_type no_index = header::no_index;
    
    
    static bool read_info(path_type const& path, header& h, size_type& items_count, std::error_code& ec) noexcept {

      auto f = file::open_to_read(path);      
      if(!f)
        return (ec = file::last_error()), false;
      
      if(!f.read(h))
        return (ec = file::last_error()), false;
      
      auto const target_size = sizeof(class header) + h.capacity() * h.data_size() + h.capacity();
      
      auto const file_size = std::filesystem::file_size(path, ec);
      if(file_size == static_cast<std::uintmax_t>(-1))
        return false;
      
      if(file_size != target_size)
        return (ec = std::error_code{error::invalid_file_size}), false;
      
      if(!f.seek(sizeof(class header) + h.capacity() * h.data_size()))
        return (ec = file::last_error()), false;
      
      try {
        auto occupancy_map = std::make_unique<bool[]>(h.capacity());
        
        if(!f.read(reinterpret_cast<char*>(&occupancy_map[0]), h.capacity()))
          return (ec = file::last_error()), false;
        
        items_count = 0;
        for(std::uint32_t i = 0; i != h.capacity(); ++i)
          if(occupancy_map[i])
            ++items_count;

        return true;
      } catch(std::bad_alloc const&) {
        return (ec = std::error_code{error::not_enough_memory}), false;
      }      
    }
    
    
    storage() noexcept = default;
    ~storage() { close(); }
    storage(storage const&) = delete;
    storage& operator = (storage const&) = delete;
    explicit operator bool () const noexcept { return records_ != nullptr; }
    header const* header() const noexcept { return header_; }
    

    bool create(path_type const& path, class header const& specified, std::error_code& ec) noexcept {
      
      close();
      
      if(!specified)
        return (ec = std::error_code{error::invalid_specified_header}), false;
      
      auto f = file::create(path);
      if(!f)
        return (ec = file::last_error()), false;
      bool const resized = f.resize(storage_size(specified.capacity()));
      if(!resized)
        return (ec = file::last_error()), false;
      f.close();
      
      mapped_file_ = mapped_file::open(path);
      if(!mapped_file_)
        return (ec = mapped_file::last_error()), false;
      
      mapped_region_ = mapped_file_.map();
      if(mapped_region_.address == nullptr)
        return (ec = mapped_file::last_error()), false;
                  
      header_ = reinterpret_cast<class header*>(mapped_region_.address);
      records_ = reinterpret_cast<record_type*>(mapped_region_.address + sizeof(class header));
      occupancy_map_ = reinterpret_cast<bool*>(mapped_region_.address
                                               + sizeof(class header)
                                               + sizeof(record_type) * specified.capacity());
      std::memset(&occupancy_map_[0], 0, specified.capacity());
      
      *header_ = specified;      
      index_type next_index = 0;
      header_->free_index(next_index);
      record_type* last_cell = records_ + header_->capacity() - 1;
            
      
      for(record_type* each_cell = records_;
          each_cell != last_cell;
          ++each_cell) {
        new(each_cell) record_type{++next_index};
      }
      
      new(last_cell) record_type{no_index};
      
      return true;
    }
    
    
    bool open(path_type const& path, class header const& specified, std::error_code& ec) noexcept {

      close();
      
      class header actual; header::size_type items_count;
      if(!check_header(path, specified, actual, items_count, ec))
        return false;
      
      size_type const needed_capacity = specified.needed_capacity(items_count);
      if(needed_capacity > actual.capacity()) {
        std::filesystem::resize_file(path, storage_size(needed_capacity), ec);
        if(!!ec)
          return false;        
      }
      
      if(!map_file(path, specified, ec))
        return false;
            
      if(header_->capacity() >= needed_capacity)
        return true;
      
      if(!expand_storage(needed_capacity, ec))
        return false;
      
      return true;
    }
    
    
    bool open_to_read(path_type const& path, cellarium::header const& specified, std::error_code& ec) noexcept {
      
      close();
      
      header actual; header::size_type items_count;      
      if(!check_header(path, specified, actual, items_count, ec))
        return false;
      
      if(!map_file(path, specified, ec))
        return false;
      
      return true;
    }
    
    
    void close() noexcept {
      if(records_ == nullptr)
        return;
      mapped_region_ = mapped_file::region{};
      mapped_file_ = mapped_file{};
      header_ = nullptr;
      records_ = nullptr;
      occupancy_map_ = nullptr;
    }
    
    
    index_type try_insert(T const& data) noexcept {
      auto const index = header_->free_index();
      if(index == cellarium::header::no_index)
        return cellarium::header::no_index;
      header_->free_index(records_[index].fill(data));
      occupancy_map_[index] = true;
      return index;
    }
    
    
    void remove(index_type index) noexcept {
      records_[index].clear(header_->free_index());
      header_->free_index(index);
      occupancy_map_[index] = false;
    }


    T const& operator [](index_type index) const noexcept {
      return records_[index].data();
    }


    T& operator [](index_type index) noexcept {
      return records_[index].data();
    }
    
    
    template<typename F> void for_each(F&& f) {
      for(index_type i = 0; i != header_->capacity(); ++i)
        if(occupancy_map_[i])
          f(records_[i].data());
    }


    template<typename F> void for_each(F&& f) const {
      for(index_type i = 0; i != header_->capacity(); ++i)
        if(occupancy_map_[i])
          f(records_[i].data());
    }
 
    
  private:
    
    mapped_file mapped_file_;
    mapped_file::region mapped_region_;
    cellarium::header* header_{nullptr};
    record_type* records_{nullptr};
    bool* occupancy_map_{nullptr};
        
    
    static mapped_file::size_type storage_size(index_type capacity) noexcept {
      return sizeof(class header) + capacity * sizeof(record_type) + capacity;
    }
    
    
    bool check_header(path_type const& path, class header const& specified,
                      class header& actual, size_type& items_count,
                      std::error_code& ec) noexcept {
                        
      if(!specified)
        return (ec = std::error_code{error::invalid_specified_header}), false;
      
      if(!read_info(path, actual, items_count, ec))
        return false;
            
      if(!actual.has_valid_signature())
        return (ec = std::error_code{error::not_a_storage_file}), false;      
      
      if(!actual.has_valid_format_version())
        return (ec = std::error_code{error::different_format_version}), false;
      
      if(actual.data_version() != specified.data_version())
        return (ec = std::error_code{error::different_data_version}), false;
      
      if(actual.data_size() != specified.data_size())
        return (ec = std::error_code{error::different_data_size}), false;
      
      auto const file_size = std::filesystem::file_size(path, ec);
      if(!!ec)
        return false;      
      if(file_size != storage_size(actual.capacity()))
        return (ec = std::error_code{error::invalid_file_size}), false;
      
      return true;
    }
    
    
    bool map_file(path_type const& path, class header const& specified, std::error_code& ec) noexcept {
      
      mapped_file_ = mapped_file::open(path);
      if(!mapped_file_)
        return (ec = mapped_file::last_error()), false;
      
      mapped_region_ = mapped_file_.map();
      if(mapped_region_.address == nullptr)
        return (ec = mapped_file::last_error()), false;
                  
      header_ = reinterpret_cast<class header*>(mapped_region_.address);
      header_->occupancy_factor(specified.occupancy_factor());
      records_ = reinterpret_cast<record_type*>(mapped_region_.address + sizeof(class header));
      occupancy_map_ = reinterpret_cast<bool*>(mapped_region_.address
                                               + sizeof(class header)
                                               + sizeof(record_type) * header_->capacity());
      return true;
    }
    
    
    bool expand_storage(size_type new_capacity, std::error_code& ec) noexcept {
      try {

        std::unique_ptr<bool[]> occupancy_map = std::make_unique<bool[]>(header_->capacity());
        auto const occupancy_ptr = &occupancy_map[0];
        std::memcpy(occupancy_ptr, occupancy_map_, header_->capacity());      

        index_type next_index = header_->capacity();
        record_type* last_record = records_ + new_capacity - 1;
        for(record_type* new_record = records_ + header_->capacity(); new_record != last_record; ++new_record)
          new(new_record) record_type{++next_index};
        new(last_record) record_type{header_->free_index()};
        header_->free_index(header_->capacity());      

        occupancy_map_ = reinterpret_cast<bool*>(records_ + new_capacity);
        std::memcpy(occupancy_map_, occupancy_ptr, header_->capacity());
        std::memset(occupancy_map_ + header_->capacity(), 0, new_capacity - header_->capacity());

        header_->capacity(new_capacity);
        return true;

      } catch(std::bad_alloc const&) {
        ec = std::error_code{error::not_enough_memory};
        return false;        
      }
    }
        
  }; // storage


} // cellarium
