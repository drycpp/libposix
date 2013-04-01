/* This is free and unencumbered software released into the public domain. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "posix++/directory.h"

#include <cassert>      /* for assert() */
#include <dirent.h>     /* for fdopendir() */
#include <fcntl.h>      /* for AT_FDCWD, fcntl() */
#include <unistd.h>     /* for close() */
#include <sys/types.h>  /* for DIR */
#include <system_error> /* for std::system_error */

using namespace posix;

void
directory::open(const int dirfd, const std::string& pathname) {
  assert(dirfd > 0 || dirfd == AT_FDCWD);
  assert(!pathname.empty());

  int flags = O_RDONLY;
#ifdef O_CLOEXEC
  flags |= O_CLOEXEC;   /* POSIX.1-2008 (Linux, FreeBSD) */
#endif
#ifdef O_DIRECTORY
  flags |= O_DIRECTORY; /* Linux-specific */
#endif
#ifdef O_NOATIME
  flags |= O_NOATIME;   /* Linux-specific */
#endif

  if ((_fd = openat(dirfd, pathname.c_str(), flags)) == -1) {
    switch (errno) {
      case EMFILE: /* Too many open files */
      case ENFILE: /* Too many open files in system */
      case ENOMEM: /* Cannot allocate memory in kernel */
        throw std::system_error(errno, std::system_category()); // FIXME
      default:
        throw std::system_error(errno, std::system_category());
    }
  }
}

directory::directory(const std::string& pathname) {
  open(AT_FDCWD, pathname);
}

directory::directory(const std::string& pathname, const int dirfd) {
  open(dirfd, pathname);
}

directory::~directory() {
  close();
}

void
directory::close() {
  if (_fd > 0) {
    ::close(_fd);
    _fd = -1;
  }
}

directory::iterator
directory::begin() const {
  return directory::iterator(*this);
}

directory::iterator
directory::end() const {
  return directory::iterator();
}

const directory::iterator
directory::cbegin() const {
  return directory::iterator(*this);
}

const directory::iterator
directory::cend() const {
  return directory::iterator();
}

directory::iterator::iterator(const directory& dir) {
#ifdef F_DUPFD_CLOEXEC
  const int dirfd = fcntl(dir.fd(), F_DUPFD_CLOEXEC, 0);
#else
  const int dirfd = fcntl(dir.fd(), F_DUPFD, 0);
#endif
  if (dirfd == -1) {
    switch (errno) {
      case EMFILE: /* Too many open files */
        throw std::system_error(errno, std::system_category()); // FIXME
      case EINVAL: /* Invalid argument as `fcntl()` argument */
        assert(false);
        throw std::system_error(errno, std::system_category());
      case EBADF:
      default:
        throw std::system_error(errno, std::system_category());
    }
  }

  if (!(_dirp = fdopendir(dirfd))) {
    switch (errno) {
      case EMFILE: /* Too many open files */
        throw std::system_error(errno, std::system_category()); // FIXME
      case EACCES:
      case EBADF:
      case ENOENT:
      case ENOMEM:
      case ENOTDIR:
      default:
        throw std::system_error(errno, std::system_category());
    }
  }
}

directory::iterator::~iterator() {
  if (_dirp) {
    if (closedir(reinterpret_cast<DIR*>(_dirp)) == -1) {
      /* Ignore any errors from `closedir()`. */
    }
    _dirp = nullptr;
  }
}

bool
directory::iterator::operator!=(const directory::iterator& other) {
  return (void)other, true; // TODO
}

directory::iterator&
directory::iterator::operator++() {
  return *this; // TODO
}

std::string
directory::iterator::operator*() {
  return std::string(); // TODO
}
