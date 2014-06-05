/* This is free and unencumbered software released into the public domain. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "error.h"

#include <array>   /* for std::array */
#include <cassert> /* for assert() */
#include <cerrno>  /* for E*, errno */
#include <cstdio>  /* for std::snprintf() */
#include <cstring> /* for std::strerror(), stpncpy() */

using namespace posix;

error::error() noexcept
  : std::system_error(errno, std::system_category()) {}

bad_descriptor::bad_descriptor() noexcept
  : logic_error(EBADF) {}

bad_descriptor::bad_descriptor(const char* const origin) noexcept
  : logic_error(EBADF, std::generic_category(), origin) {}

bad_address::bad_address() noexcept
  : logic_error(EFAULT) {}

bad_address::bad_address(const char* const origin) noexcept
  : logic_error(EFAULT, std::generic_category(), origin) {}

invalid_argument::invalid_argument() noexcept
  : logic_error(EINVAL) {}

invalid_argument::invalid_argument(const char* const origin) noexcept
  : logic_error(EINVAL, std::generic_category(), origin) {}

connection_refused::connection_refused() noexcept
  : runtime_error(ECONNREFUSED) {}

connection_refused::connection_refused(const char* const origin) noexcept
  : runtime_error(ECONNREFUSED, std::generic_category(), origin) {}

void
posix::throw_error() {
  throw_error(errno, nullptr);
}

void
posix::throw_error(const char* const origin) {
  throw_error(errno, origin);
}

void
posix::throw_error(const int code) {
  throw_error(code, nullptr);
}

void
posix::throw_error(const int code,
                   const char* const origin) {
  static thread_local std::array<char, 4096> buffer;

  if (origin) {
    std::snprintf(buffer.data(), buffer.size(),
      "%s [in %s]", std::strerror(code), origin);
  }
  else {
    stpncpy(buffer.data(), std::strerror(code), buffer.size());
    buffer.data()[buffer.size()-1] = '\0';
  }

  switch (code) {
    case EBADF:        /* Bad file descriptor */
      throw bad_descriptor(buffer.data());
    case ECONNREFUSED: /* Connection refused */
      throw connection_refused(buffer.data());
    case EFAULT:       /* Bad address */
      throw bad_address(buffer.data());
    case EINVAL:       /* Invalid argument */
      throw invalid_argument(buffer.data());
    case EMFILE:       /* Too many open files */
      throw posix::fatal_error(code, std::generic_category(), buffer.data());
    case EMSGSIZE:     /* Message too long */
      throw posix::logic_error(code, std::generic_category(), buffer.data());
    case ENAMETOOLONG: /* File name too long */
      throw posix::logic_error(code, std::generic_category(), buffer.data());
    case ENFILE:       /* Too many open files in system */
      throw posix::fatal_error(code, std::generic_category(), buffer.data());
    case ENOBUFS:      /* No buffer space available in kernel */
      throw posix::fatal_error(code, std::generic_category(), buffer.data());
    case ENOMEM:       /* Cannot allocate memory in kernel */
      throw posix::fatal_error(code, std::generic_category(), buffer.data());
    case ENOSPC:       /* No space left on device */
      throw posix::fatal_error(code, std::generic_category(), buffer.data());
    case ENOSYS:       /* Function not implemented */
      throw posix::logic_error(code, std::generic_category(), buffer.data());
    case ENOTDIR:      /* Not a directory */
      throw posix::logic_error(code, std::generic_category(), buffer.data());
    case EACCES:       /* Permission denied */
    case ELOOP:        /* Too many levels of symbolic links */
    case ENOENT:       /* No such file or directory */
    case ENOPROTOOPT:  /* Protocol not available */
    case ENOTCONN:     /* Transport endpoint is not connected */
    case ENOTSOCK:     /* Socket operation on non-socket */
    default:
      throw runtime_error(code, std::generic_category(), buffer.data());
  }
}
