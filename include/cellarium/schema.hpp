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
#include <tuple>
#include <array>
#include "field.hpp"



namespace cellarium {
  
  
  namespace detail {
        
    template<typename T> struct describer {
      static field from_type(char const*, char const*) noexcept {
        static_assert(false, "Unsupported field type");
      }
    };
    
    
    template<> struct describer<bool> {
      static field from_type(char const* title, char const* description) noexcept {
        return field::byte(title, description);
      }
    };
    
    
    template<> struct describer<std::int16_t> {
      static field from_type(char const* title, char const* description) noexcept {
        return field::i16(title, description);
      }
    };


    template<> struct describer<std::uint16_t> {
      static field from_type(char const* title, char const* description) noexcept {
        return field::u16(title, description);
      }
    };
    
    
    template<> struct describer<std::int32_t> {
      static field from_type(char const* title, char const* description) noexcept {
        return field::i32(title, description);
      }
    };
    
    
    template<> struct describer<std::uint32_t> {
      static field from_type(char const* title, char const* description) noexcept {
        return field::u32(title, description);
      }
    };
    
    
    template<> struct describer<std::int64_t> {
      static field from_type(char const* title, char const* description) noexcept {
        return field::i64(title, description);
      }
    };
    
    
    template<> struct describer<std::uint64_t> {
      static field from_type(char const* title, char const* description) noexcept {
        return field::u64(title, description);
      }
    };
    
    
    template<> struct describer<float> {
      static field from_type(char const* title, char const* description) noexcept {
        return field::f32(title, description);
      }
    };
    
    
    template<> struct describer<double> {
      static field from_type(char const* title, char const* description) noexcept {
        return field::f64(title, description);
      }
    };
    
    
    template<std::size_t N> struct describer<char[N]> {
      static field from_type(char const* title, char const* description) noexcept {
        return field::string(N - 1, title, description);
      }
    };
    
    
    template<std::size_t N> struct describer<wchar_t[N]> {
      static field from_type(char const* title, char const* description) noexcept {
        return field::utf16_string(N - 1, title, description);
      }
    };
    
    
    template<std::size_t N> struct describer<std::array<char, N>> {
      static field from_type(char const* title, char const* description) noexcept {
        return field::string(N - 1, title, description);
      }
    };

    
    template<std::size_t N> struct describer<std::array<wchar_t, N>> {
      static field from_type(char const* title, char const* description) noexcept {
        return field::utf16_string(N - 1, title, description);
      }
    };
    
    
    template<std::size_t N> struct describer<unsigned char[N]> {
      static field from_type(char const* title, char const* description) noexcept {
        return field::byte_array(N, title, description);
      }
    };
    
    
    template<std::size_t N> struct describer<std::array<unsigned char, N>> {
      static field from_type(char const* title, char const* description) noexcept {
        return field::byte_array(N, title, description);
      }
    };


    template<std::size_t N> struct describer<std::int16_t[N]> {
      static field from_type(char const* title, char const* description) noexcept {
        return field::i16_array(N, title, description);
      }
    };
    
    
    template<std::size_t N> struct describer<std::array<std::int16_t, N>> {
      static field from_type(char const* title, char const* description) noexcept {
        return field::i16_array(N, title, description);
      }
    };


    template<std::size_t N> struct describer<std::uint16_t[N]> {
      static field from_type(char const* title, char const* description) noexcept {
        return field::u16_array(N, title, description);
      }
    };
    
    
    template<std::size_t N> struct describer<std::array<std::uint16_t, N>> {
      static field from_type(char const* title, char const* description) noexcept {
        return field::u16_array(N, title, description);
      }
    };
    
    
    template<std::size_t N> struct describer<std::int32_t[N]> {
      static field from_type(char const* title, char const* description) noexcept {
        return field::i32_array(N, title, description);
      }
    };
    
    
    template<std::size_t N> struct describer<std::array<std::int32_t, N>> {
      static field from_type(char const* title, char const* description) noexcept {
        return field::i32_array(N, title, description);
      }
    };
    
    
    template<std::size_t N> struct describer<std::uint32_t[N]> {
      static field from_type(char const* title, char const* description) noexcept {
        return field::u32_array(N, title, description);
      }
    };
    
    
    template<std::size_t N> struct describer<std::array<std::uint32_t, N>> {
      static field from_type(char const* title, char const* description) noexcept {
        return field::u32_array(N, title, description);
      }
    };
    
    
    template<std::size_t N> struct describer<std::int64_t[N]> {
      static field from_type(char const* title, char const* description) noexcept {
        return field::i64_array(N, title, description);
      }
    };
    
    
    template<std::size_t N> struct describer<std::array<std::int64_t, N>> {
      static field from_type(char const* title, char const* description) noexcept {
        return field::i64_array(N, title, description);
      }
    };

    
    template<std::size_t N> struct describer<std::uint64_t[N]> {
      static field from_type(char const* title, char const* description) noexcept {
        return field::u64_array(N, title, description);
      }
    };
    
    
    template<std::size_t N> struct describer<std::array<std::uint64_t, N>> {
      static field from_type(char const* title, char const* description) noexcept {
        return field::u64_array(N, title, description);
      }
    };
    
    
    template<std::size_t N> struct describer<float[N]> {
      static field from_type(char const* title, char const* description) noexcept {
        return field::f32_array(N, title, description);
      }
    };
    
    
    template<std::size_t N> struct describer<std::array<float, N>> {
      static field from_type(char const* title, char const* description) noexcept {
        return field::f32_array(N, title, description);
      }
    };
    
    
    template<std::size_t N> struct describer<double[N]> {
      static field from_type(char const* title, char const* description) noexcept {
        return field::f64_array(N, title, description);
      }
    };
    
    
    template<std::size_t N> struct describer<std::array<double, N>> {
      static field from_type(char const* title, char const* description) noexcept {
        return field::f64_array(N, title, description);
      }
    };
    
  } // detail
  
  
  
  struct annotation {    
    char const* title;
    char const* description;
    
    template<std::size_t N1, std::size_t N2>
    annotation(char const (&title)[N1], char const (&description)[N2]) noexcept:
      title{title}, description{description}
    { }    
  }; // annotation
  
  
  template<typename T1, typename... Tn>
  class schema : public std::tuple<T1, Tn...> {
  public:
  
    static constexpr std::size_t fields_count = 1 + sizeof...(Tn);
    
    using tuple = std::tuple<T1, Tn...>;
    using fields = std::array<field, fields_count>;
    using annotations = std::array<annotation, fields_count>;

    template<std::size_t I> static constexpr std::size_t field_size =
        sizeof(std::tuple_element<I, tuple>::type);

    static fields describe(annotations const& as) {
      fields fields;
      describe_types<0, T1, Tn...>(fields, as);
      return fields;
    }


    schema() noexcept = default;
    schema(schema const&) noexcept = default;
    schema& operator = (schema const&) noexcept = default;
    schema(T1 const& a1, Tn const&... an) noexcept: tuple{a1, an...} { }


    template<std::size_t I>
    typename std::tuple_element<I, tuple>::type const& field() const noexcept {
      return std::get<I>(*this);
    }


    template<std::size_t I>
    typename std::tuple_element<I, tuple>::type& field() noexcept {
      return std::get<I>(*this);
    }
        
  private:
  
    template<std::size_t I, typename V1, typename... Vn>
    static void describe_types(fields& fs, annotations const& as) noexcept {
      fs[I] = detail::describer<V1>::from_type(as[I].title, as[I].description);
      describe_types<I + 1, Vn...>(fs, as);
    }
    
    
    template<std::size_t I>
    static void describe_types(fields&, annotations const&) noexcept {
    }
    
  }; // schema
  
  
} // cellarium
