#pragma once

#include <system_error>

#include <doctest/doctest.h>

#include <cellarium/storage.hpp>


TEST_CASE("storage::storage") {
  cellarium::storage<int> target;
  REQUIRE(!target);
}


TEST_CASE("storage::create") {
  cellarium::storage<int> target;
  auto const header = cellarium::header::make<int>(1, 17, 0.7f, {cellarium::field::i32("id", "")});
  std::error_code ec; bool const created = target.create("test.storage", header, ec);
  REQUIRE(created);
  target.close();
}


TEST_CASE("storage::open") {
  cellarium::storage<int> target;
  auto const header = cellarium::header::make<int>(1, 17, 0.7f, {cellarium::field::i32("id", "")});
  std::error_code ec; bool const opened = target.open("test.storage", header, ec);
  if(!opened)
    puts(ec.message().data());
  REQUIRE(opened);
}

