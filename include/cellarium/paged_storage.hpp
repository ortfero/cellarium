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


#include <cmath>
#include <filesystem>
#include <system_error>
#include <memory>
#include <type_traits>

#include "storage.hpp"
#include "file_manager.hpp"


namespace cellarium {
  
  
  template<typename T>
  class paged_storage {
  public:
  
    static_assert(std::is_trivial_v<T>, "Only trivial types can be stored");
  
    using path_type = std::filesystem::path;
    using storage_type = storage<T>;
    using storage_ptr = std::unique_ptr<storage_type>;
    using value_type = T;
    using size_type = typename storage_type::size_type;
    using index_type = typename storage_type::index_type;
    
    static constexpr index_type no_index = storage_type::no_index;
      
    
    paged_storage() noexcept = default;
    ~paged_storage() { close(); }
    paged_storage(paged_storage const&) = delete;
    paged_storage& operator = (paged_storage const&) = delete;
    explicit operator bool () const noexcept { return pages_count_ != 0; }
    
    
    bool initialize(path_type const& path, size_type max_pages,
                    header const& specified, std::error_code& ec) noexcept {
      
      bool const exists = std::filesystem::exists(path, ec);
      if(!!ec)
        return false;
      
      if(exists)
        return open(path, max_pages, specified, ec);
      else
        return create(path, max_pages, specified, ec);
    }
    
    
    bool create(path_type const& path, size_type max_pages,
                header const& specified, std::error_code& ec) {
      
      file_manager_ = file_manager{path};      
      max_pages_ = max_pages;
      page_capacity_ = specified.capacity();

      if(!file_manager_::remove_all(ec))
        return false;
      pages_ = std::make_unique<storage_ptr[]>(max_pages_);      
      auto storage = std::make_unique<cellarium::storage>();
      path_type const path = file_manager_.name_for_page(0);
      if(!storage->create(path, specified, ec))
        return false;
      pages_[0] = std::move(storage);
      last_page_ = std::to_address(pages_[0]);
      last_page_base_ = 0;
      pages_count_ = 1;
      return true;
    }
        
    
    bool open(path_type const& path, size_type max_pages,
              header const& specified, std::error_code& ec) {
                
      file_manager_ = file_manager{path};
      max_pages_ = max_pages;      
      
      auto files = file_manager_.list(ec);
      if(!!ec)
        return false;
      if(files.empty())
        return (ec = std::error_code{error::storage_not_found_to_open}), false;
      
      pages_ = std::make_unique<storage_ptr[]>(max_pages_);
      
      if(files.size() == 1) {
        
        auto storage = std::make_unique<storage_type>();
        if(!storage->open(path, specified, ec))
          return false;
        
        page_capacity_ = storage->header()->capacity();
        pages_[0] = std::move(storage);
        
      } else {
        
        size_type total_size = 0; size_type last_storage_capacity = 0;
        header each_header; size_type each_size;
        
        for(auto& each_file: files) {
          if(!storage_type::read_info(each_file, each_header, each_size, ec))
            return false;
          if(last_storage_capacity != 0 && last_storage_capacity != each_header.capacity())
            return (ec = std::error_code{error::merging_incompatible_storages})
          total_size += each_size;
        }
        
        files[0] = file_manager_.generate_zero_page_name();
        std::filesystem::rename(path, files[0], ec);
        if(!!ec)
          return false;
                
        size_type const needed_capacity = specified.needed_capacity(total_size);
        header const merged_header = header::with_capacity(specified, needed_capacity);
        page_capacity_ = merged_header.capacity();
        
        auto merged_storage = std::make_unique<storage_type>();
        if(!merged_storage->create(path, merged_header, ec)) {
          std::error_code none;
          std::filesystem::rename(files[0], path, none);
          return false;
        }
        
        storage_type each_storage;
        for(auto& each_file: files) {
          if(!each_storage.open_to_read(each_file, specified, ec)) {
            std::error_code none;
            std::filesystem::rename(files[0], path, none);
            return false;
          }
          each_storage.for_each([&](value_type const& value) { merged_storage.try_insert(value); });
          each_storage.close();
        }
        
        for(auto& each_file: files) {
          std::filesystem::remove(each_file, ec);
          if(!!ec)
            return false;
        }
        
        pages_[0] = std::move(merged_storage);
      }
      
      last_page_ = std::to_address(pages_[0]);
      last_page_base_ = 0;      
      pages_count_ = 1;
        
      return true;
    }
    
    
    void close() noexcept {
      pages_.reset();
      last_page_ = nullptr;
      last_page_base_ = 0;
      file_manager_ = file_manager{};
      page_capacity_ = 0;
      max_pages_ = 0;
      pages_count_ = 0;
    }
    
    
    index_type try_insert(T const& data) noexcept {
      index_type const inserted = last_page_->try_insert(data);
      if(inserted != no_index)
        return last_page_base_ + inserted;
      if(!add_page(*last_page_->header()))
        return no_index;
      return last_page_base_ + last_page_->try_insert(data);
    }
    
    
    void remove(index_type index) noexcept {
      if(pages_count_ == 1)
        return last_page_->remove(index);
      index_type base = index / page_capacity_;
      index_type offset = index % page_capacity_;
      pages_[base]->remove(offset);
    }

    
    template<typename F> void for_each(F&& f) {
      for(index_type i = 0; i != pages_count_; ++i)
        pages_[i]->for_each(std::forward<F>(f));
    }


    template<typename F> void for_each(F&& f) const {
      for(index_type i = 0; i != pages_count_; ++i)
        pages_[i]->for_each(std::forward<F>(f));
    }


  private:
  
    file_manager file_manager_;
    size_type page_capacity_{0};
    size_type max_pages_{0};
    size_type pages_count_{0};
    std::unique_ptr<storage_ptr[]> pages_;
    storage* last_page_{nullptr};
    size_type last_page_base_{0};
    
    
    bool add_page(header const& last_header) {
      if(pages_count_ == max_pages_)
        return false;      
      auto storage = std::make_unique<cellarium::storage>();
      path_type const path = file_manager_.name_for_page(pages_count_);
      std::error_code ec;
      header new_page_header{header::with_page_number(last_header, pages_count_)};
      if(!storage->create(path, new_page_header, ec))
        return false;
      pages_[pages_count_] = std::move(storage);
      last_page_ = std::to_address(pages_[pages_count_]);
      last_page_base_ = page_capacity_ * pages_count_;
      ++pages_count_;
      return true;
    }
    
    
  }; // paged_storage
  
  
} // cellarium
