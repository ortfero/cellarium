#pragma once

#include <cstdint>

#include <doctest/doctest.h>

#include <cellarium/schema.hpp>
#include <cellarium/header.hpp>


using schema = cellarium::schema<std::int32_t,
                                 char[64],
                                 std::array<char, 256>,
                                 double>;
auto const annotations = schema::annotations{
  cellarium::annotation{"id", "Unique identifier"},
  cellarium::annotation{"name", "Data name"},
  cellarium::annotation{"description", "Data description"},
  cellarium::annotation{"factor", "Data factor"}
};

class data_type : public schema {
public:

  data_type() noexcept = default;
  data_type(data_type const&) noexcept = default;
  data_type& operator = (data_type const&) noexcept = default;
  std::int32_t id() const noexcept { return schema::field<0>(); }
  char const* name() const noexcept { return schema::field<1>(); }
  char const* description() const noexcept { return schema::field<2>().data(); }
  double factor() const noexcept { return schema::field<3>(); }

  data_type(std::int32_t id, char const* name, char *description, double factor) noexcept {
    schema::field<0>() = id;
#pragma warning(suppress:4996)
    strncpy(schema::field<1>(), name, schema::field_size<1>);
    schema::field<1>()[schema::field_size<1> - 1] = '\0';
#pragma warning(suppress:4996)
    strncpy(schema::field<2>().data(), description, schema::field_size<2>);
    schema::field<2>()[schema::field_size<2> - 1] = '\0';
    schema::field<3>() = factor;
  }

}; // data_type


TEST_CASE("schema") {
  std::array<cellarium::field, 4> fields = data_type::describe(annotations);
  //auto const header = cellarium::header::make(1, 17, 0.7f, );

}
