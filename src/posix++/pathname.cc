/* This is free and unencumbered software released into the public domain. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "pathname.h"

#include "error.h"

#include <cassert>      /* for assert() */
#include <cerrno>       /* for errno */
#include <cstring>      /* for std::strcpy(), GNU basename() */
#include <libgen.h>     /* for basename(), dirname() */
#include <unistd.h>     /* for unlink() */

#undef basename         /* get rid of the glibc macro */

using namespace posix;

bool
pathname::is_portable() const noexcept {
  return true; // TODO
}

pathname
pathname::dirname() const {
  char buffer[_string.size() + 1];
  std::strcpy(buffer, _string.c_str());
  return pathname(::dirname(buffer));
}

pathname
pathname::basename() const {
  char buffer[_string.size() + 1];
  std::strcpy(buffer, _string.c_str());
  return pathname(::basename(buffer));
}

void
pathname::unlink() const {
  if (::unlink(c_str()) == -1) {
    switch (errno) {
      case ENOMEM:  /* Cannot allocate memory in kernel */
        throw posix::fatal_error(errno);
      case EBADF:   /* Bad file descriptor */
        throw posix::bad_descriptor();
      case EFAULT:  /* Bad address */
        throw posix::bad_address();
      case EINVAL:  /* Invalid argument */
        throw posix::invalid_argument();
      case ENAMETOOLONG: /* File name too long */
      case ENOTDIR: /* Not a directory */
        throw posix::logic_error(errno);
      default:
        throw posix::runtime_error(errno);
    }
  }
}
