/* This is free and unencumbered software released into the public domain. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "directory.h"
#include "error.h"
#include "pathname.h"

#include <cassert>      /* for assert() */
#include <dirent.h>     /* for fdopendir() */
#include <fcntl.h>      /* for AT_FDCWD, fcntl() */
#include <unistd.h>     /* for close(), readlinkat() */
#include <sys/types.h>  /* for DIR */

using namespace posix;

directory
directory::open(const int dirfd, const char* const pathname) {
  assert(dirfd > 0 || dirfd == AT_FDCWD);
  assert(pathname != nullptr);
  assert(*pathname != '\0');

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

  int fd;
  if ((fd = openat(dirfd, pathname, flags)) == -1) {
    switch (errno) {
      case EMFILE: /* Too many open files */
      case ENFILE: /* Too many open files in system */
      case ENOMEM: /* Cannot allocate memory in kernel */
        throw posix::fatal_error(errno);
      case EBADF:  /* Bad file descriptor */
        throw posix::bad_descriptor();
      default:
        throw posix::error(errno);
    }
  }

  return directory(fd);
}

directory
directory::open(const pathname& pathname) {
  return open(AT_FDCWD, pathname.c_str());
}

directory
directory::open(const char* const pathname) {
  return open(AT_FDCWD, pathname);
}

directory
directory::open(const directory& directory,
                const pathname& pathname) {
  return open(directory.fd(), pathname.c_str());
}

directory
directory::open(const directory& directory,
                const char* const pathname) {
  return open(directory.fd(), pathname);
}

pathname
directory::readlink(const char* const pathname) const {
  char buffer[PATH_MAX];

  const int rc = readlinkat(fd(), pathname, buffer, sizeof(buffer) - 1);
  if (rc == -1) {
    switch (errno) {
      case ENOMEM: /* Cannot allocate memory in kernel */
        throw posix::fatal_error(errno);
      case EBADF:  /* Bad file descriptor */
        throw posix::bad_descriptor();
      default:
        throw posix::error(errno);
    }
  }

  assert(rc < sizeof(buffer));
  buffer[rc] = '\0';

  return posix::pathname(buffer);
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
  const int dirfd = dir.dup(/*true*/).release();

  if (!(_dirp = fdopendir(dirfd))) {
    switch (errno) {
      case EMFILE: /* Too many open files */
      case ENOMEM: /* Cannot allocate memory in kernel */
        throw posix::fatal_error(errno);
      case EBADF:  /* Bad file descriptor */
        throw posix::bad_descriptor();
      case EACCES:
      case ENOENT:
      case ENOTDIR:
      default:
        throw posix::error(errno);
    }
  }
}

directory::iterator::~iterator() noexcept {
  if (_dirp) {
    if (closedir(reinterpret_cast<DIR*>(_dirp)) == -1) {
      /* Ignore any errors from closedir(). */
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
