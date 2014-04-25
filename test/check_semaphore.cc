/* This is free and unencumbered software released into the public domain. */

#include "catch.hpp"

#include <posix++/semaphore.h> /* for posix::semaphore */

using namespace posix;

TEST_CASE("constructor") {
  REQUIRE_NOTHROW(unnamed_semaphore{});
  REQUIRE_NOTHROW(unnamed_semaphore{0});
  REQUIRE_NOTHROW(unnamed_semaphore{1});
}

TEST_CASE("#value()") {
  REQUIRE(unnamed_semaphore{}.value() == 0);
  REQUIRE(unnamed_semaphore{0}.value() == 0);
  REQUIRE(unnamed_semaphore{1}.value() == 1);
}

TEST_CASE("#destroy()") {
  /* It's currently unsafe to call #destroy() explicitly, as the destructor
   * will also call it and invoking it twice leads to undefined behavior. */
}

TEST_CASE("#wait()") {
  REQUIRE_NOTHROW(unnamed_semaphore{1}.wait()); /* won't block */
}

TEST_CASE("#try_wait()") {
  REQUIRE(unnamed_semaphore{0}.try_wait() == false);
  REQUIRE(unnamed_semaphore{1}.try_wait() == true);
}

TEST_CASE("#notify()") {
  REQUIRE_NOTHROW(unnamed_semaphore{}.notify());
}
