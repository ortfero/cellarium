#pragma once

#include <doctest/doctest.h>

#include <cellarium/mapped_file.hpp>


TEST_CASE("mapped_file::mapped_file") {
  cellarium::mapped_file target;
  REQUIRE(!target);
}


TEST_CASE("mapped_file::open") {
  auto file = cellarium::file::create("test.file");
  file.close();
  auto target = cellarium::mapped_file::open("test.file");
  REQUIRE(target);
}


TEST_CASE("mapped_file::map/1") {
  auto file = cellarium::file::open_to_rw("test.file");
  file.resize(2 * cellarium::mapped_file::granularity());
  file.close();
  using cellarium::mapped_file;
  auto target = mapped_file::open("test.file");
  auto region = target.map(0, mapped_file::granularity());
  REQUIRE(region);
  region = mapped_file::region{};
  REQUIRE(!region);
}


TEST_CASE("mapped_file::map/2") {
  using cellarium::mapped_file;
  auto target = cellarium::mapped_file::open("test.file");
  auto region1 = target.map(0, mapped_file::granularity());
  REQUIRE(region1);
  auto region2 = target.map(mapped_file::granularity(), mapped_file::granularity());
  REQUIRE(region2);
}
