#pragma once

#include <cstdint>

#include <doctest/doctest.h>

#include <cellarium/record.hpp>


TEST_CASE("record::record") {
  cellarium::record<std::int32_t> record{1};
  REQUIRE(record.data() == 1);
}


TEST_CASE("record::fill") {
  cellarium::record<std::int32_t> record{1};
  REQUIRE(record.fill(-1) == 1);
  REQUIRE(record.data() == -1);
}


TEST_CASE("record::clear") {
  cellarium::record<std::int32_t> record{1};
  record.fill(-1);
  record.clear(2);
  REQUIRE(record.data() == 2);
}
