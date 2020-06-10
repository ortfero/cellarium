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


#include <vector>
#include <filesystem>
#include <system_error>

#include "mapped_file.hpp"


namespace cellarium {
  

class file_manager {
public:

  using path_type = std::filesystem::path;
  
  file_manager() noexcept = default;
  file_manager(file_manager const&) = default;
  file_manager& operator = (file_manager const&) = default;
  file_manager(file_manager&&) noexcept = default;
  file_manager& operator = (file_manager&&) noexcept = default;
  
  
  file_manager(path_type const& path) {
    directory_slash_name_ = path;
    directory_slash_name_.replace_extension();
    extension_ = path.extension();
  }
    
  
  bool remove_all(std::error_code& ec) {
    
    enlist(ec, [](path_type const& path) {
      std::filesystem::remove(path, ec);
    });
    
    return !ec;    
  }
  
  
  path_type name_for_page(size_type page_index) const {
    
    if(page_index == 0)
      return directory_slash_name_ + extension_;
    else
      return directory_slash_name_ + '@' + std::to_string(page_index + 1) + extension_;
  }
  
  
  path_type generate_zero_page_name() const {
    return directory_slash_name_ + '@' + '0' + extension_;
  }
  
  
  std::vector<path_type> list(std::error_code& ec) const {
    
    std::vector<path_type> result;
    
    enlist(ec, [&result](path_type const& path) {
      result.push_back(path);
    });
    
    return result;
  }
  
  

private:

  path_type directory_slash_name_
  path_type extension_;

  
  template<typename F>
  bool enlist(std::error_code& ec, F&& f) const {
    
    namespace fs = std::filesystem;
    path_type current_name = name_for_page(0);
    
    std::error_code ec;
    if(!fs::exists(current_name, ec))
      return !ec;
    
    f(current_name);
    
    size_type n = 1;
    current_name = name_for_page(n);
    while(fs::exists(current_name, ec)) {
      f(current_name);
      current_name = name_for_page(++n);
    }
    
    return !ec;
  }
  
}; // file_manager


} // cellarium