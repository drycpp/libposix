/* This is free and unencumbered software released into the public domain. */

#include "catch.hpp"

#include <posix++/sysv_segment.h> /* for posix::sysv_segment */

#include <unistd.h> /* for getpagesize(), getpid() */

using namespace posix;

TEST_CASE("test_sysv_segment") {
  sysv_segment shm = sysv_segment::create(getpid(), getpagesize(), 0600);
  REQUIRE(!shm.is_attached());
  shm.attach();
  REQUIRE(shm.is_attached());
  shm.detach();
  REQUIRE(!shm.is_attached());
  shm.remove();
}
