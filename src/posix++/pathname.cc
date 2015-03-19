/* This is free and unencumbered software released into the public domain. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "error.h"
#include "pathname.h"

#include <cassert>     /* for assert() */
#include <cstring>     /* for std::strcpy(), GNU basename() */
#include <libgen.h>    /* for basename(), dirname() */
#include <sys/types.h> /* for lstat() */
#include <sys/stat.h>  /* for lstat() */
#include <unistd.h>    /* for lstat(), unlink() */

#undef basename        /* get rid of the glibc macro */

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

bool
pathname::exists() const {
  struct stat buffer;
  if (lstat(c_str(), &buffer) == -1) {
    assert(errno != EFAULT);
    switch (errno) {
      case ENOENT:  /* No such file or directory */
        return false;
      default:
        throw_error("lstat", "\"%s\", %s", c_str(), "buffer");
    }
  }
  return true;
}

void
pathname::unlink() const {
  if (::unlink(c_str()) == -1) {
    throw_error("unlink", "\"%s\"", c_str());
  }
}

pathname&
pathname::append(const char* const path) {
  assert(path);
  _string.append(path);
  return *this;
}
