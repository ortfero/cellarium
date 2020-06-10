#pragma once

#include <cstring>

#include <doctest/doctest.h>

#include <cellarium/field.hpp>


TEST_CASE("field::field") {
  cellarium::field f;
  REQUIRE(!f);
}


TEST_CASE("field::byte") {
  using namespace cellarium;
  auto const f = field::byte("test", "description");
  REQUIRE(f);
  REQUIRE(f.kind() == field_kind::byte);
  REQUIRE(std::strcmp(f.name(), "test") == 0);
  REQUIRE(std::strcmp(f.description(), "description") == 0);
  REQUIRE(!f.array());
  REQUIRE(!f.has_view());
  REQUIRE(f.size_of() == sizeof(for_kind<field_kind::byte>::type));
  REQUIRE(f.align_of() == alignof(for_kind<field_kind::byte>::type));
}


TEST_CASE("field::i16") {
  using namespace cellarium;
  auto const f = field::i16("test", "description");
  REQUIRE(f);
  REQUIRE(f.kind() == field_kind::i16);
  REQUIRE(std::strcmp(f.name(), "test") == 0);
  REQUIRE(std::strcmp(f.description(), "description") == 0);
  REQUIRE(!f.array());
  REQUIRE(!f.has_view());
  REQUIRE(f.size_of() == sizeof(for_kind<field_kind::i16>::type));
  REQUIRE(f.align_of() == alignof(for_kind<field_kind::i16>::type));
}


TEST_CASE("field::u16") {
  using namespace cellarium;
  auto const f = field::u16("test", "description");
  REQUIRE(f);
  REQUIRE(f.kind() == field_kind::u16);
  REQUIRE(std::strcmp(f.name(), "test") == 0);
  REQUIRE(std::strcmp(f.description(), "description") == 0);
  REQUIRE(!f.array());
  REQUIRE(!f.has_view());
  REQUIRE(f.size_of() == sizeof(for_kind<field_kind::u16>::type));
  REQUIRE(f.align_of() == alignof(for_kind<field_kind::u16>::type));
}


TEST_CASE("field::i32") {
  using namespace cellarium;
  auto const f = field::i32("test", "description");
  REQUIRE(f);
  REQUIRE(f.kind() == field_kind::i32);
  REQUIRE(std::strcmp(f.name(), "test") == 0);
  REQUIRE(std::strcmp(f.description(), "description") == 0);
  REQUIRE(!f.array());
  REQUIRE(!f.has_view());
  REQUIRE(f.size_of() == sizeof(for_kind<field_kind::i32>::type));
  REQUIRE(f.align_of() == alignof(for_kind<field_kind::i32>::type));
}


TEST_CASE("field::u32") {
  using namespace cellarium;
  auto const f = field::u32("test", "description");
  REQUIRE(f);
  REQUIRE(f.kind() == field_kind::u32);
  REQUIRE(std::strcmp(f.name(), "test") == 0);
  REQUIRE(std::strcmp(f.description(), "description") == 0);
  REQUIRE(!f.array());
  REQUIRE(!f.has_view());
  REQUIRE(f.size_of() == sizeof(for_kind<field_kind::u32>::type));
  REQUIRE(f.align_of() == alignof(for_kind<field_kind::u32>::type));
}


TEST_CASE("field::i64") {
  using namespace cellarium;
  auto const f = field::i64("test", "description");
  REQUIRE(f);
  REQUIRE(f.kind() == field_kind::i64);
  REQUIRE(std::strcmp(f.name(), "test") == 0);
  REQUIRE(std::strcmp(f.description(), "description") == 0);
  REQUIRE(!f.array());
  REQUIRE(!f.has_view());
  REQUIRE(f.size_of() == sizeof(for_kind<field_kind::i64>::type));
  REQUIRE(f.align_of() == alignof(for_kind<field_kind::i64>::type));
}


TEST_CASE("field::u64") {
  using namespace cellarium;
  auto const f = field::u64("test", "description");
  REQUIRE(f);
  REQUIRE(f.kind() == field_kind::u64);
  REQUIRE(std::strcmp(f.name(), "test") == 0);
  REQUIRE(std::strcmp(f.description(), "description") == 0);
  REQUIRE(!f.array());
  REQUIRE(!f.has_view());
  REQUIRE(f.size_of() == sizeof(for_kind<field_kind::u64>::type));
  REQUIRE(f.align_of() == alignof(for_kind<field_kind::u64>::type));
}


TEST_CASE("field::f32") {
  using namespace cellarium;
  auto const f = field::f32("test", "description");
  REQUIRE(f);
  REQUIRE(f.kind() == field_kind::f32);
  REQUIRE(std::strcmp(f.name(), "test") == 0);
  REQUIRE(std::strcmp(f.description(), "description") == 0);
  REQUIRE(!f.array());
  REQUIRE(!f.has_view());
  REQUIRE(f.size_of() == sizeof(for_kind<field_kind::f32>::type));
  REQUIRE(f.align_of() == alignof(for_kind<field_kind::f32>::type));
}


TEST_CASE("field::f64") {
  using namespace cellarium;
  auto const f = field::f64("test", "description");
  REQUIRE(f);
  REQUIRE(f.kind() == field_kind::f64);
  REQUIRE(std::strcmp(f.name(), "test") == 0);
  REQUIRE(std::strcmp(f.description(), "description") == 0);
  REQUIRE(!f.array());
  REQUIRE(!f.has_view());
  REQUIRE(f.size_of() == sizeof(for_kind<field_kind::f64>::type));
  REQUIRE(f.align_of() == alignof(for_kind<field_kind::f64>::type));
}


TEST_CASE("field::string") {
  using namespace cellarium;
  auto const f = field::string(127, "test", "description");
  REQUIRE(f);
  REQUIRE(f.kind() == field_kind::utf8);
  REQUIRE(f.capacity() == 128);
  REQUIRE(std::strcmp(f.name(), "test") == 0);
  REQUIRE(std::strcmp(f.description(), "description") == 0);
  REQUIRE(f.array());
  REQUIRE(!f.has_view());
  REQUIRE(f.size_of() == sizeof(for_kind<field_kind::utf8>::type) * f.capacity());
  REQUIRE(f.align_of() == alignof(for_kind<field_kind::utf8>::type));
}


TEST_CASE("field::utf16_string") {
  using namespace cellarium;
  auto const f = field::utf16_string(127, "test", "description");
  REQUIRE(f);
  REQUIRE(f.kind() == field_kind::utf16);
  REQUIRE(f.capacity() == 128);
  REQUIRE(std::strcmp(f.name(), "test") == 0);
  REQUIRE(std::strcmp(f.description(), "description") == 0);
  REQUIRE(f.array());
  REQUIRE(!f.has_view());
  REQUIRE(f.size_of() == sizeof(for_kind<field_kind::utf16>::type) * f.capacity());
  REQUIRE(f.align_of() == alignof(for_kind<field_kind::utf16>::type));
}


TEST_CASE("field::byte_array") {
  using namespace cellarium;
  auto const f = field::byte_array(127, "test", "description");
  REQUIRE(f);
  REQUIRE(f.kind() == field_kind::byte);
  REQUIRE(f.capacity() == 127);
  REQUIRE(std::strcmp(f.name(), "test") == 0);
  REQUIRE(std::strcmp(f.description(), "description") == 0);
  REQUIRE(f.array());
  REQUIRE(!f.has_view());
  REQUIRE(f.size_of() == sizeof(for_kind<field_kind::byte>::type) * f.capacity());
  REQUIRE(f.align_of() == alignof(for_kind<field_kind::byte>::type));
}



TEST_CASE("field::i16_array") {
  using namespace cellarium;
  auto const f = field::i16_array(127, "test", "description");
  REQUIRE(f);
  REQUIRE(f.kind() == field_kind::i16);
  REQUIRE(f.capacity() == 127);
  REQUIRE(std::strcmp(f.name(), "test") == 0);
  REQUIRE(std::strcmp(f.description(), "description") == 0);
  REQUIRE(f.array());
  REQUIRE(!f.has_view());
  REQUIRE(f.size_of() == sizeof(for_kind<field_kind::i16>::type) * f.capacity());
  REQUIRE(f.align_of() == alignof(for_kind<field_kind::i16>::type));
}


TEST_CASE("field::u16_array") {
  using namespace cellarium;
  auto const f = field::u16_array(127, "test", "description");
  REQUIRE(f);
  REQUIRE(f.kind() == field_kind::u16);
  REQUIRE(f.capacity() == 127);
  REQUIRE(std::strcmp(f.name(), "test") == 0);
  REQUIRE(std::strcmp(f.description(), "description") == 0);
  REQUIRE(f.array());
  REQUIRE(!f.has_view());
  REQUIRE(f.size_of() == sizeof(for_kind<field_kind::u16>::type) * f.capacity());
  REQUIRE(f.align_of() == alignof(for_kind<field_kind::u16>::type));
}


TEST_CASE("field::i32_array") {
  using namespace cellarium;
  auto const f = field::i32_array(127, "test", "description");
  REQUIRE(f);
  REQUIRE(f.kind() == field_kind::i32);
  REQUIRE(f.capacity() == 127);
  REQUIRE(std::strcmp(f.name(), "test") == 0);
  REQUIRE(std::strcmp(f.description(), "description") == 0);
  REQUIRE(f.array());
  REQUIRE(!f.has_view());
  REQUIRE(f.size_of() == sizeof(for_kind<field_kind::i32>::type) * f.capacity());
  REQUIRE(f.align_of() == alignof(for_kind<field_kind::i32>::type));
}


TEST_CASE("field::i64_array") {
  using namespace cellarium;
  auto const f = field::i64_array(127, "test", "description");
  REQUIRE(f);
  REQUIRE(f.kind() == field_kind::i64);
  REQUIRE(f.capacity() == 127);
  REQUIRE(std::strcmp(f.name(), "test") == 0);
  REQUIRE(std::strcmp(f.description(), "description") == 0);
  REQUIRE(f.array());
  REQUIRE(!f.has_view());
  REQUIRE(f.size_of() == sizeof(for_kind<field_kind::i64>::type) * f.capacity());
  REQUIRE(f.align_of() == alignof(for_kind<field_kind::i64>::type));
}


TEST_CASE("field::u64_array") {
  using namespace cellarium;
  auto const f = field::u64_array(127, "test", "description");
  REQUIRE(f);
  REQUIRE(f.kind() == field_kind::u64);
  REQUIRE(f.capacity() == 127);
  REQUIRE(std::strcmp(f.name(), "test") == 0);
  REQUIRE(std::strcmp(f.description(), "description") == 0);
  REQUIRE(f.array());
  REQUIRE(!f.has_view());
  REQUIRE(f.size_of() == sizeof(for_kind<field_kind::u64>::type) * f.capacity());
  REQUIRE(f.align_of() == alignof(for_kind<field_kind::u64>::type));
}


TEST_CASE("field::f32_array") {
  using namespace cellarium;
  auto const f = field::f32_array(127, "test", "description");
  REQUIRE(f);
  REQUIRE(f.kind() == field_kind::f32);
  REQUIRE(f.capacity() == 127);
  REQUIRE(std::strcmp(f.name(), "test") == 0);
  REQUIRE(std::strcmp(f.description(), "description") == 0);
  REQUIRE(f.array());
  REQUIRE(!f.has_view());
  REQUIRE(f.size_of() == sizeof(for_kind<field_kind::f32>::type) * f.capacity());
  REQUIRE(f.align_of() == alignof(for_kind<field_kind::f32>::type));
}


TEST_CASE("field::f64_array") {
  using namespace cellarium;
  auto const f = field::f64_array(127, "test", "description");
  REQUIRE(f);
  REQUIRE(f.kind() == field_kind::f64);
  REQUIRE(f.capacity() == 127);
  REQUIRE(std::strcmp(f.name(), "test") == 0);
  REQUIRE(std::strcmp(f.description(), "description") == 0);
  REQUIRE(f.array());
  REQUIRE(!f.has_view());
  REQUIRE(f.size_of() == sizeof(for_kind<field_kind::f64>::type) * f.capacity());
  REQUIRE(f.align_of() == alignof(for_kind<field_kind::f64>::type));
}
