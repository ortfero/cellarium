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
#include <cstring>
#include <tuple>
#include <initializer_list>


namespace cellarium {
  
  
  enum class field_kind: std::int32_t {
    
    undefined, byte, i16, u16, i32, u32, i64, u64, f32, f64,
    utf8, utf16 
    
  }; // field_kind
  
  
  template<field_kind> struct for_kind;

  template<> struct for_kind<field_kind::byte> { using type = unsigned char; };
  template<> struct for_kind<field_kind::i16> { using type = std::int16_t; };
  template<> struct for_kind<field_kind::u16> { using type = std::uint16_t; };
  template<> struct for_kind<field_kind::i32> { using type = std::int32_t; };
  template<> struct for_kind<field_kind::u32> { using type = std::uint32_t; };
  template<> struct for_kind<field_kind::i64> { using type = std::int64_t; };
  template<> struct for_kind<field_kind::u64> { using type = std::uint64_t; };
  template<> struct for_kind<field_kind::f32> { using type = float; };
  template<> struct for_kind<field_kind::f64> { using type = double; };
  template<> struct for_kind<field_kind::utf8> { using type = char; };
  template<> struct for_kind<field_kind::utf16> { using type = short; };
  
  
  enum class field_view_kind: std::int32_t {
    
    unspecified, fixed, scientific, enumeration, utc_time_seconds,
    utc_time_milliseconds, utc_time_microseconds, utc_time_nanoseconds,
    local_time_seconds, local_time_milliseconds, local_time_microseconds,
    local_time_nanoseconds
    
  }; // field_view_kind
  
  
  class field_view {
  public:

    static field_view fixed(std::uint32_t width) {
      return field_view{field_view_kind::fixed, width, 0};
    }
    
    
    static field_view fixed(std::uint32_t width, std::uint32_t precision) {
      return field_view{field_view_kind::fixed, width, precision};
    }
    
    
    static field_view scientific() {
      return field_view{field_view_kind::scientific};
    }
    
    
    static field_view enumeration() {
      return field_view{field_view_kind::enumeration};
    }
    
    
    static field_view utc_time_seconds() {
      return field_view{field_view_kind::utc_time_seconds};
    }
    
    
    static field_view utc_time_milliseconds() {
      return field_view{field_view_kind::utc_time_milliseconds};
    }
    
    
    static field_view utc_time_microseconds() {
      return field_view{field_view_kind::utc_time_microseconds};
    }
    
    
    static field_view utc_time_nanoseconds() {
      return field_view{field_view_kind::utc_time_nanoseconds};
    }
    
    
    static field_view local_time_seconds() {
      return field_view{field_view_kind::local_time_seconds};
    }
    
    
    static field_view local_time_milliseconds() {
      return field_view{field_view_kind::local_time_milliseconds};
    }
    
    
    static field_view local_time_microseconds() {
      return field_view{field_view_kind::local_time_microseconds};
    }
    
    
    static field_view local_time_nanoseconds() {
      return field_view{field_view_kind::local_time_nanoseconds};
    }
    
    
    field_view() noexcept = default;
    field_view(field_view const&) noexcept = default;
    field_view& operator = (field_view const&) noexcept = default;
    field_view_kind kind() const noexcept { return kind_; }
    std::uint32_t width() const noexcept { return width_; }
    std::uint32_t precision() const noexcept { return precision_; }
    
  private:
  
    field_view_kind kind_{field_view_kind::unspecified};
    std::uint32_t width_;
    std::uint32_t precision_;
    
    explicit field_view(field_view_kind kind) noexcept:
      kind_{kind}, width_{0}, precision_{0}
    { }
    
    
    field_view(field_view_kind kind, std::uint32_t width, std::uint32_t precision) noexcept:
      kind_{kind}, width_{width}, precision_{precision}
    { }
    
    
  };  // field_view
    
  
  class field {
  public:
  
    using size_type = std::uint32_t;
    
    static constexpr size_type name_capacity = 63;
    static constexpr size_type description_capacity = 511;
    static constexpr size_type min_page_size = 4096;
        
    static field byte(char const* title, char const* description) noexcept {
      return field{field_kind::byte, 1, title, description};
    }
    
    
    static field i16(char const* title, char const* description) noexcept {
      return field{field_kind::i16, 1, title, description};
    }
    
    
    static field u16(char const* title, char const* description) noexcept {
      return field{field_kind::u16, 1, title, description};
    }
    
    
    static field i32(char const* title, char const* description) noexcept {
      return field{field_kind::i32, 1, title, description};
    }
    
    
    static field u32(char const* title, char const* description) noexcept {
      return field{field_kind::u32, 1, title, description};
    }
    
    
    static field i64(char const* title, char const* description) noexcept {
      return field{field_kind::i64, 1, title, description};
    }
    
    
    static field u64(char const* title, char const* description) noexcept {
      return field{field_kind::u64, 1, title, description};
    }
    
    
    static field f32(char const* title, char const* description) noexcept {
      return field{field_kind::f32, 1, title, description};
    }
    
    
    static field f64(char const* title, char const* description) noexcept {
      return field{field_kind::f64, 1, title, description};
    }
    
    
    static field string(size_type capacity, char const* title, char const* description) noexcept {
      return field{field_kind::utf8, capacity + 1, title, description};
    }
    
    
    static field utf16_string(size_type capacity, char const* title, char const* description) noexcept {
      return field{field_kind::utf16, capacity + 1, title, description};
    }
    
    
    static field byte_array(size_type capacity, char const* title, char const* description) noexcept {
      return field{field_kind::byte, capacity, title, description};
    }
    
    
    static field i16_array(size_type capacity, char const* title, char const* description) noexcept {
      return field{field_kind::i16, capacity, title, description};
    }
    
    
    static field u16_array(size_type capacity, char const* title, char const* description) noexcept {
      return field{field_kind::u16, capacity, title, description};
    }
    
    
    static field i32_array(size_type capacity, char const* title, char const* description) noexcept {
      return field{field_kind::i32, capacity, title, description};
    }
    
    
    static field u32_array(size_type capacity, char const* title, char const* description) noexcept {
      return field{field_kind::u32, capacity, title, description};
    }
    
    
    static field i64_array(size_type capacity, char const* title, char const* description) noexcept {
      return field{field_kind::i64, capacity, title, description};
    }
    
    
    static field u64_array(size_type capacity, char const* title, char const* description) noexcept {
      return field{field_kind::u64, capacity, title, description};
    }
    
    
    static field f32_array(size_type capacity, char const* title, char const* description) noexcept {
      return field{field_kind::f32, capacity, title, description};
    }
    
    
    static field f64_array(size_type capacity, char const* title, char const* description) noexcept {
      return field{field_kind::f64, capacity, title, description};
    }
    
    
    field(field const&) noexcept = default;
    field& operator = (field const&) noexcept = default;
    field_kind kind() const noexcept { return kind_; }
    char const* name() const noexcept { return name_; }
    char const* description() const noexcept { return description_; }
    bool array() const noexcept { return capacity_ > 1; }
    size_type capacity() const noexcept { return capacity_; }
    field_view const& view() const noexcept { return view_; }
    void view(field_view const& fv) noexcept { view_ = fv; }
    bool has_view() const noexcept { return view_.kind() != field_view_kind::unspecified; }
    size_type offset() const noexcept { return offset_; }
    void offset(size_type offset) noexcept { offset_ = offset; }
    
        
    field() noexcept {
      name_[0] = '\0';
      description_[0] = '\0';
    }
    
    
    explicit operator bool() const noexcept {
      return kind_ != field_kind::undefined && name_[0] != '\0';
    }
    
    
    size_type size_of() const noexcept {
      return capacity_ * size_for_kind();
    }
    
    
    size_type align_of() const noexcept {
      switch(kind_) {
        case field_kind::byte   : return alignof(for_kind<field_kind::byte>::type);
        case field_kind::i16    : return alignof(for_kind<field_kind::i16>::type);
        case field_kind::u16    : return alignof(for_kind<field_kind::u16>::type);
        case field_kind::i32    : return alignof(for_kind<field_kind::i32>::type);
        case field_kind::u32    : return alignof(for_kind<field_kind::u32>::type);
        case field_kind::i64    : return alignof(for_kind<field_kind::i64>::type);
        case field_kind::u64    : return alignof(for_kind<field_kind::u64>::type);
        case field_kind::f32    : return alignof(for_kind<field_kind::f32>::type);
        case field_kind::f64    : return alignof(for_kind<field_kind::f64>::type);
        case field_kind::utf8   : return alignof(for_kind<field_kind::utf8>::type);
        case field_kind::utf16  : return alignof(for_kind<field_kind::utf16>::type);
        default                 : return 0;
      }      
    }
    
    
  private:
  
    field_kind kind_{field_kind::undefined};
    size_type capacity_{0};
    field_view view_;
    size_type offset_{0};
    char name_[name_capacity + 1];
    char description_[description_capacity + 1];
    
    
    size_type size_for_kind() const noexcept {
      switch(kind_) {
        case field_kind::byte   : return sizeof(for_kind<field_kind::byte>::type);
        case field_kind::i16    : return sizeof(for_kind<field_kind::i16>::type);
        case field_kind::u16    : return sizeof(for_kind<field_kind::u16>::type);
        case field_kind::i32    : return sizeof(for_kind<field_kind::i32>::type);
        case field_kind::u32    : return sizeof(for_kind<field_kind::u32>::type);
        case field_kind::i64    : return sizeof(for_kind<field_kind::i64>::type);
        case field_kind::u64    : return sizeof(for_kind<field_kind::u64>::type);
        case field_kind::f32    : return sizeof(for_kind<field_kind::f32>::type);
        case field_kind::f64    : return sizeof(for_kind<field_kind::f64>::type);
        case field_kind::utf8   : return sizeof(for_kind<field_kind::utf8>::type);
        case field_kind::utf16  : return sizeof(for_kind<field_kind::utf16>::type);
        default                 : return 0;
      }
    }
    
    
    field(field_kind kind, size_type capacity,
          char const* name, char const* description) noexcept:
          
          kind_{kind}, capacity_{capacity} {
      
#pragma warning(suppress:4996)
      std::strncpy(name_, name, name_capacity);
      name_[name_capacity] = '\0';

#pragma warning(suppress:4996)
      std::strncpy(description_, description, description_capacity);
      description_[description_capacity] = '\0';
    }
  }; // field
  
  
}
