/* This is free and unencumbered software released into the public domain. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "error.h"

#include <cassert> /* for assert() */
#include <cerrno>  /* for E*, errno */

using namespace posix;

error::error() noexcept
  : std::system_error(errno, std::system_category()) {}

bad_descriptor::bad_descriptor() noexcept
  : logic_error(EBADF) {}

bad_address::bad_address() noexcept
  : logic_error(EFAULT) {}

invalid_argument::invalid_argument() noexcept
  : logic_error(EINVAL) {}

connection_refused::connection_refused() noexcept
  : runtime_error(ECONNREFUSED) {}

void
posix::throw_error() {
  throw_error(errno);
}

void
posix::throw_error(const int code) {
  switch (code) {
    case EBADF:        /* Bad file descriptor */
      throw bad_descriptor();
    case ECONNREFUSED: /* Connection refused */
      throw connection_refused();
    case EFAULT:       /* Bad address */
      throw bad_address();
    case EINVAL:       /* Invalid argument */
      throw invalid_argument();
    case EMFILE:       /* Too many open files */
      throw posix::fatal_error(code);
    case ENAMETOOLONG: /* File name too long */
      throw posix::logic_error(code);
    case ENFILE:       /* Too many open files in system */
      throw posix::fatal_error(code);
    case ENOBUFS:      /* No buffer space available in kernel */
      throw posix::fatal_error(code);
    case ENOMEM:       /* Cannot allocate memory in kernel */
      throw posix::fatal_error(code);
    case ENOSPC:       /* No space left on device */
      throw posix::fatal_error(code);
    case ENOSYS:       /* Function not implemented */
      throw posix::logic_error(code);
    case ENOTDIR:      /* Not a directory */
      throw posix::logic_error(code);
    default:
      throw runtime_error(code);
  }
}
