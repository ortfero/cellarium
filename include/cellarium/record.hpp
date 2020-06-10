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


#include <cstddef>
#include <cstdint>
#include <limits>
#include <new>

#include "header.hpp"


namespace cellarium {

  
  template<typename T>
  union record {
  public:    
    
    using index_type = header::index_type;
    
    static constexpr index_type no_index = header::no_index;
    
    record() = delete;
    record(record const&) = delete;
    record& operator = (record const&)  = delete;
    T const& data() const noexcept { return data_; }
    T& data() noexcept { return data_; }
    
    explicit record(index_type next_index) noexcept:
      next_index_{next_index}
    { }
    
    
    void clear(index_type next_index) noexcept {
      data_.~T();
      next_index_ = next_index;
    }
    
    
    index_type fill(T const& data) {
      index_type const index = next_index_;
      new(&data_) T(data);
      return index;
    }
        
  private:
  
    index_type next_index_; 
    T data_;
    
  }; // record
  
  
} // cellarium
