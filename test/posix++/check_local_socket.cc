/* This is free and unencumbered software released into the public domain. */

#define BOOST_TEST_MODULE local_socket
#include <boost/test/unit_test.hpp>

#include <posix++/local_socket.h> /* for posix::local_socket */
#include <posix++/pathname.h>     /* for posix::pathname */

#include <sys/socket.h> /* for AF_LOCAL, SOCK_STREAM */

using namespace posix;

BOOST_AUTO_TEST_CASE(test_pair) {
  auto sp = local_socket::pair();
  auto s1 = sp.first;
  auto s2 = sp.second;

  BOOST_CHECK(s1.valid());
  BOOST_CHECK(s2.valid());

  BOOST_CHECK_EQUAL(s1.domain(), AF_LOCAL);
  BOOST_CHECK_EQUAL(s1.type(), SOCK_STREAM);
  BOOST_CHECK_EQUAL(s1.protocol(), 0);

  s1.send("Hello, world!");
  s1.close_write();

  BOOST_CHECK_EQUAL(s2.recv_string(), "Hello, world!");
}

BOOST_AUTO_TEST_CASE(test_connect) {
  // TODO
}
