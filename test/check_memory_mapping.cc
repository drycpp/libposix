/* This is free and unencumbered software released into the public domain. */

#include "catch.hpp"

#include <posix++/memory_mapping.h> /* for posix::memory_mapping */

using namespace posix;

static std::uint8_t buffer[0x1000] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

TEST_CASE("test_size") {
  REQUIRE(memory_mapping(buffer, sizeof(buffer)).size() == sizeof(buffer));
}

TEST_CASE("test_data") {
  REQUIRE(memory_mapping(buffer, sizeof(buffer)).data() == buffer);
  REQUIRE(memory_mapping(buffer, sizeof(buffer)).data<std::uint8_t>() == buffer);
  REQUIRE(memory_mapping(buffer, sizeof(buffer)).data(1) == buffer + 1);
}

TEST_CASE("test_operator_bool") {
#ifdef NDEBUG
  REQUIRE(!memory_mapping(nullptr, 0)); /* asserts */
#endif
  REQUIRE(memory_mapping(buffer, sizeof(buffer)));
}

TEST_CASE("test_operator_at") {
  REQUIRE(memory_mapping(buffer, sizeof(buffer))[1] == buffer[1]);
}
