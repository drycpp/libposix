/* This is free and unencumbered software released into the public domain. */

#define BOOST_TEST_MODULE sysv_segment
#include <boost/test/unit_test.hpp>

#include <posix++/sysv_segment.h> /* for posix::sysv_segment */

#include <unistd.h> /* for getpagesize(), getpid() */

using namespace posix;

BOOST_AUTO_TEST_CASE(test_sysv_segment) {
  sysv_segment shm = sysv_segment::create(getpid(), getpagesize(), 0600);
  BOOST_CHECK(!shm.is_attached());
  shm.attach();
  BOOST_CHECK(shm.is_attached());
  shm.detach();
  BOOST_CHECK(!shm.is_attached());
  shm.remove();
}
