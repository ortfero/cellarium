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


#include <system_error>


namespace cellarium {
  
  
  enum class error {
    none, invalid_specified_header, not_a_storage_file, different_format_version,
    different_data_version, different_data_size, invalid_file_size,
    not_enough_memory, not_enough_pages, storage_not_found_to_open,
    invalid_json_data,
    merging_incompatible_storages
  }; // error
  
  
  struct error_category: std::error_category {
  
    char const* name() const noexcept override {
      return "cellarium";
    }
    
    std::string message(int code) const noexcept override {
      switch(code) {
        case error::none:
          return "None";
        case error::invalid_specified_header:
          return "Invalid storage header was specified";
        case error::not_a_storage_file:
          return "Not a storage file";
        case error::different_format_version:
          return "Different storage format version";
        case error::different_data_version:
          return "Different data type version";
        case error::different_data_size:
          return "Different data type size";
        case error::invalid_file_size:
          return "Invalid file size";
        case error::not_enough_memory:
          return "Not enough memory";
        case error::not_enough_pages:
          return "Not enough pages";
        case error::storage_not_found_to_open:
          return "Specified storage is not found";
        case error::merging_incompatible_storages:
          return "Unable to merge incompatible storages";
        case error::invalid_json_data:
          return "Invalid JSON data";
        default:
          return "Unknown";
      }
    }
  
  }; // error_category


  inline error_category const cellarium_category;
  
  
  inline std::error_code make_error_code(error e) {    
    return std::error_code{int(e), cellarium_category};     
  }

} // cellarium


namespace std {
  
    
template<>
  struct is_error_code_enum<cellarium::error>: true_type { };
	
	
} // std
