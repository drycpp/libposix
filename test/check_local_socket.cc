/* This is free and unencumbered software released into the public domain. */

#include "catch.hpp"

#include <posix++/local_socket.h> /* for posix::local_socket */
#include <posix++/pathname.h>     /* for posix::pathname */

#include <sys/socket.h> /* for AF_LOCAL, SOCK_STREAM */

using namespace posix;

TEST_CASE("test_pair") {
  auto sp = local_socket::pair();
  auto s1 = sp.first;
  auto s2 = sp.second;

  REQUIRE(s1.valid());
  REQUIRE(s2.valid());

#ifdef __linux__
  // FIXME: this fails on Mac OS X 10.7.
  REQUIRE(s1.domain() == AF_LOCAL);
#endif
  REQUIRE(s1.type() == SOCK_STREAM);
  REQUIRE(s1.protocol() == 0);

  s1.send("Hello, world!");
  s1.close_write();

  REQUIRE(s2.recv_string() == "Hello, world!");
}

TEST_CASE("test_connect") {
  // TODO
}
