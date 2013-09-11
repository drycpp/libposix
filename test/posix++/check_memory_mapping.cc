/* This is free and unencumbered software released into the public domain. */

#define BOOST_TEST_MODULE memory_mapping
#include <boost/test/unit_test.hpp>

#include <posix++/memory_mapping.h> /* for posix::memory_mapping */

using namespace posix;

static char buffer[64];

BOOST_AUTO_TEST_CASE(test_operator_bool) {
  BOOST_CHECK(!memory_mapping(nullptr, 0));
  BOOST_CHECK(memory_mapping(buffer, 0x1000));
}

BOOST_AUTO_TEST_CASE(test_data) {
  BOOST_CHECK_EQUAL(memory_mapping(buffer, 0x1000).data(), buffer);
  BOOST_CHECK_EQUAL(memory_mapping(buffer, 0x1000).data<char>(), buffer);
}
