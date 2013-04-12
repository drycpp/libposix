/* This is free and unencumbered software released into the public domain. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "descriptor.h"

#include "group.h"
#include "mode.h"
#include "user.h"

#include <array>        /* for std::array */
#include <cassert>      /* for assert() */
#include <cerrno>       /* for errno */
#include <fcntl.h>      /* for F_*, fcntl() */
#include <stdexcept>    /* for std::invalid_argument */
#include <string>       /* for std::string */
#include <sys/stat.h>   /* for fchmod() */
#include <system_error> /* for std::system_error */
#include <unistd.h>     /* for close(), fchown(), read() */

using namespace posix;

static const std::string invalid_in_copy_constructor = "invalid descriptor passed to copy constructor";

descriptor::descriptor(const descriptor& other) {
  if (other.valid()) {
#ifdef F_DUPFD_CLOEXEC
    const bool fd_cloexec = other.flags() & FD_CLOEXEC;
    const int fcntl_cmd = fd_cloexec ? F_DUPFD_CLOEXEC : F_DUPFD;
    _fd = fcntl(other._fd, fcntl_cmd, 0);
#else
    _fd = fcntl(other._fd, F_DUPFD, 0);
#endif

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
}

descriptor::~descriptor() noexcept {
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
descriptor::chown(const user& user,
                  const group& group) {
  const uid_t uid = static_cast<uid_t>(user.id());
  const gid_t gid = static_cast<gid_t>(group.id());

  if (fchown(_fd, uid, gid) == -1) {
    switch (errno) {
      case ENOMEM: /* Cannot allocate memory in kernel */
        throw std::system_error(errno, std::system_category()); // FIXME
      default:
        throw std::system_error(errno, std::system_category());
    }
  }
}

void
descriptor::chmod(const mode mode) {
  if (fchmod(_fd, static_cast<mode_t>(mode)) == -1) {
    switch (errno) {
      case ENOMEM: /* Cannot allocate memory in kernel */
        throw std::system_error(errno, std::system_category()); // FIXME
      default:
        throw std::system_error(errno, std::system_category());
    }
  }
}

std::string
descriptor::read() {
  std::string result;
  std::array<char, 4096> buffer;

  for (;;) {
    const ssize_t rc = ::read(_fd, buffer.data(), buffer.size());
    switch (rc) {
      case -1:
        switch (errno) {
          case EINTR: /* Interrupted system call */
            continue; /* try again */
          case EFAULT:
            assert(errno != EFAULT); /* should never be reached */
            throw std::system_error(errno, std::system_category());
          default:
            throw std::system_error(errno, std::system_category());
        }

      case 0:
        return result; /* all done, EOF reached */

      default:
        assert(rc > 0);
        result.append(buffer.data(), static_cast<std::size_t>(rc));
    }
  }
}

void
descriptor::close() noexcept {
  if (valid()) {
    if (::close(_fd) == -1) {
      /* Ignore any errors from close(). */
    }
    _fd = -1;
  }
}
