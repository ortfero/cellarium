#pragma once


#include <cstdint>
#include <doctest/doctest.h>

#include <cellarium/header.hpp>


TEST_CASE("header::make") {
  using cellarium::header;
  auto const target = header::make<std::int32_t>(1, 17, 0.7f, {cellarium::field::i32("field", "")});
  REQUIRE(!!target);
  REQUIRE(target.signature() == header::valid_signature);
  REQUIRE(target.has_valid_signature());
  REQUIRE(target.format_version() == header::valid_format_version);
  REQUIRE(target.has_valid_format_version());
  REQUIRE(target.data_version() == 1);
  REQUIRE(target.data_size() == sizeof(std::int32_t));
  REQUIRE(target.capacity() == 32);
  REQUIRE(target.page_number() == 0);
  REQUIRE(target.occupancy_factor() == 0.7f);
  REQUIRE(target.free_index() == header::no_index);
  REQUIRE(target.fields_count() == 1);
}


TEST_CASE("header::with_capacity") {
  using cellarium::header;
  auto const base = header::make<std::int32_t>(1, 17, 0.7f, {cellarium::field::i32("field", "")});
  auto const target = header::with_capacity(base, 33);
  REQUIRE(!!target);
  REQUIRE(target.capacity() == 64);
}


TEST_CASE("header::with_page_number") {
  using cellarium::header;
  auto const base = header::make<std::int32_t>(1, 17, 0.7f, {cellarium::field::i32("field", "")});
  auto const target = header::with_page_number(base, 1);
  REQUIRE(!!target);
  REQUIRE(target.page_number() == 1);
}
