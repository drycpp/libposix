/* This is free and unencumbered software released into the public domain. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "error.h"
#include "pathname.h"

#include <cassert>  /* for assert() */
#include <cstring>  /* for std::strcpy(), GNU basename() */
#include <libgen.h> /* for basename(), dirname() */
#include <unistd.h> /* for unlink() */

#undef basename     /* get rid of the glibc macro */

using namespace posix;

bool
pathname::is_portable() const noexcept {
  return true; // TODO
}

pathname
pathname::dirname() const {
  char buffer[_string.size() + 1];
  std::strcpy(buffer, _string.c_str());
  return pathname{::dirname(buffer)};
}

pathname
pathname::basename() const {
  char buffer[_string.size() + 1];
  std::strcpy(buffer, _string.c_str());
  return pathname{::basename(buffer)};
}

void
pathname::unlink() const {
  if (::unlink(c_str()) == -1) {
    throw_error("unlink");
  }
}
