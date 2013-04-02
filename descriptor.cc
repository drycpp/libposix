/* This is free and unencumbered software released into the public domain. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "posix++/descriptor.h"

#include <cassert>      /* for assert() */
#include <fcntl.h>      /* for F_*, fcntl() */
#include <stdexcept>    /* for std::invalid_argument */
#include <string>       /* for std::string */
#include <system_error> /* for std::system_error */
#include <unistd.h>     /* for close(), dup() */

using namespace posix;

static const std::string invalid_in_copy_constructor = "invalid descriptor passed to copy constructor";

descriptor::descriptor(const descriptor& other)
  : _fd(dup(other._fd)) {
  // TODO: copy FD_CLOEXEC flag correctly using fcntl(2).
  if (_fd == -1) {
    switch (errno) {
      case EBADF:  /* Bad file descriptor */
        throw std::invalid_argument(invalid_in_copy_constructor);
      case EMFILE: /* Too many open files */
        throw std::system_error(errno, std::system_category()); // FIXME
      default:
        throw std::system_error(errno, std::system_category());
    }
  }
}

descriptor::~descriptor() {
  close();
}

bool
descriptor::readable() const {
  const int status_ = status();
  return status_ & O_RDONLY || status_ & O_RDWR;
}

bool
descriptor::writable() const {
  const int status_ = status();
  return status_ & O_WRONLY || status_ & O_RDWR;
}

int
descriptor::flags() const {
  const int flags = fcntl(_fd, F_GETFD);
  if (flags == -1) {
    switch (errno) {
      default:
        throw std::system_error(errno, std::system_category());
    }
  }
  return flags;
}

int
descriptor::status() const {
  const int status = fcntl(_fd, F_GETFL);
  if (status == -1) {
    switch (errno) {
      default:
        throw std::system_error(errno, std::system_category());
    }
  }
  return status;
}

void
descriptor::close() {
  if (valid()) {
    if (::close(_fd) == -1) {
      /* Ignore any errors from close(). */
    }
    _fd = -1;
  }
}
