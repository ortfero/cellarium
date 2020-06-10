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


#include <cstdint>
#include <initializer_list>
#include <limits>
#include "field.hpp"


namespace cellarium {
  
  

  class header {
  public:
  
    using index_type = std::uint32_t;
    using size_type = std::uint32_t;
    
    static constexpr std::uint32_t valid_signature = 0xDA1AF11E;
    static constexpr std::uint32_t valid_format_version = 1;
    static constexpr size_type fields_capacity = 64;
    static constexpr index_type no_index = index_type(-1);

    template<typename T>
    static header make(std::uint32_t data_version, size_type capacity,
                       float occupancy_factor, std::initializer_list<field> const& fields) noexcept {
      return header{data_version, std::uint32_t(sizeof (T)), capacity, occupancy_factor, fields.begin(), fields.end()};
    }


    template<typename T, std::size_t N>
    static header make(std::uint32_t data_version, size_type capacity,
                       float occupancy_factor, std::array<field, N> const& fields) noexcept {
      return header{data_version, std::uint32_t(sizeof (T)), capacity, occupancy_factor, fields.begin(), fields.end()};
    }
    
    
    static header with_page_number(header const& other, size_type page_number) {
      header r{other};
      r.page_number_ = page_number;
      return r;
    }
    
    
    static header with_capacity(header const& other, size_type capacity) {
      header r{other};
      r.capacity(capacity);
      return r;
    }
    
    
    header() noexcept = default;
    header(header const&) noexcept = default;
    header& operator = (header const&) noexcept = default;
    std::uint32_t signature() const noexcept { return signature_; }
    std::uint32_t format_version() const noexcept { return format_version_; }
    std::uint32_t data_version() const noexcept { return data_version_; }
    size_type data_size() const noexcept { return data_size_; }
    size_type capacity() const noexcept { return capacity_; }
    size_type page_number() const noexcept { return page_number_; }
    index_type free_index() const noexcept { return free_index_; }
    float occupancy_factor() const noexcept { return occupancy_factor_; }
    void free_index(index_type index) noexcept { free_index_ = index; }
    void capacity(size_type n) noexcept { capacity_ = ceil2(n); }
    void occupancy_factor(float f) noexcept { occupancy_factor_ = f; }
    field const* begin() const noexcept { return &fields_[0]; }
    field const* end() const noexcept { return &fields_[fields_count_]; }
    size_type fields_count() const noexcept { return fields_count_; }
    field const& field_at(index_type n) const noexcept { return fields_[n]; }
            
        
    explicit operator bool () const noexcept {
      return data_version_ != 0 && data_size_ != 0 && fields_count_ != 0;
    }
           
    
    bool has_valid_signature() const noexcept {
      return signature_ == valid_signature;
    }


    bool has_valid_format_version() const noexcept {
      return format_version_ == valid_format_version;
    }
    
    
    size_type needed_capacity(size_type size) const noexcept {
      size_type r = ceil2(size_type(size / occupancy_factor_ + 0.5f));
      if(capacity_ > r)
        r = capacity_;
      return r;
    }
    
    
    void offset(size_type index) {
      
      
    }

    
    
  private:
  
    std::uint32_t signature_{valid_signature};
    std::uint32_t format_version_{valid_format_version};
    std::uint32_t data_version_{0};
    size_type data_size_{0};
    size_type capacity_{0};
    size_type page_number_{0};
    index_type free_index_{no_index};
    float occupancy_factor_{.0f};
    size_type fields_count_{0};
    field fields_[fields_capacity];
    

    static std::uint32_t ceil2(std::uint32_t n) {
      if(n < 2)
        return 2;
      n--;
      n |= n >> 1;
      n |= n >> 2;
      n |= n >> 4;
      n |= n >> 8;
      n |= n >> 16;
      n++;
      return n;
    }

    
    template<typename It>
    header(std::uint32_t data_version, std::uint32_t data_size, size_type capacity,
           float occupancy_factor, It const& fields_begin, It const& fields_end) noexcept:
           data_version_{data_version}, data_size_{data_size}, capacity_{ceil2(capacity)},
           page_number_{0}, occupancy_factor_{occupancy_factor}, fields_count_{0} {
      
      if(occupancy_factor <= 0. || occupancy_factor >= 1.)
        return;
            
      size_type i = 0;
      for(It each_field = fields_begin; each_field != fields_end; ++each_field) {
        if(!*each_field)
          return;
        fields_[i++] = *each_field;
        if(i > fields_capacity)
          return;
      }
      
      fields_count_ = i;
    }

  }; // header
  
} // cellarium
