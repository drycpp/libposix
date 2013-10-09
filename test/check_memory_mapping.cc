/* This is free and unencumbered software released into the public domain. */

#define BOOST_TEST_MODULE memory_mapping
#include <boost/test/unit_test.hpp>

#include <posix++/memory_mapping.h> /* for posix::memory_mapping */

using namespace posix;

static std::uint8_t buffer[0x1000] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

BOOST_AUTO_TEST_CASE(test_size) {
  BOOST_CHECK_EQUAL(memory_mapping(buffer, sizeof(buffer)).size(), sizeof(buffer));
}

BOOST_AUTO_TEST_CASE(test_data) {
  BOOST_CHECK_EQUAL(memory_mapping(buffer, sizeof(buffer)).data(), buffer);
  BOOST_CHECK_EQUAL(memory_mapping(buffer, sizeof(buffer)).data<std::uint8_t>(), buffer);
  BOOST_CHECK_EQUAL(memory_mapping(buffer, sizeof(buffer)).data(1), buffer + 1);
}

BOOST_AUTO_TEST_CASE(test_operator_bool) {
#ifdef NDEBUG
  BOOST_CHECK(!memory_mapping(nullptr, 0)); /* asserts */
#endif
  BOOST_CHECK(memory_mapping(buffer, sizeof(buffer)));
}

BOOST_AUTO_TEST_CASE(test_operator_at) {
  BOOST_CHECK_EQUAL(memory_mapping(buffer, sizeof(buffer))[1], buffer[1]);
}
