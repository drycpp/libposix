/* This is free and unencumbered software released into the public domain. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "file.h"

#include "directory.h"
#include "error.h"
#include "pathname.h"

#include <cassert>      /* for assert() */
#include <cerrno>       /* for errno */
#include <fcntl.h>      /* for AT_FDCWD */
#include <sys/stat.h>   /* for fstat() */
#include <sys/types.h>  /* for struct stat */
#include <unistd.h>     /* for lseek() */

using namespace posix;

file
file::open(const int dirfd,
           const pathname& pathname,
           const int flags,
           const mode mode) {
  assert(dirfd > 0 || dirfd == AT_FDCWD);
  assert(!pathname.empty());

  int fd;
  if ((fd = openat(dirfd, pathname.c_str(), flags, mode)) == -1) {
    switch (errno) {
      case EMFILE: /* Too many open files */
      case ENFILE: /* Too many open files in system */
      case ENOMEM: /* Cannot allocate memory in kernel */
        throw posix::fatal_error(errno);
      default:
        throw posix::error(errno);
    }
  }

  return file(fd);
}

file
file::create(const pathname& pathname,
             const mode mode) {
  const int flags = O_CREAT | O_WRONLY | O_TRUNC;
  return open(AT_FDCWD, pathname, flags, mode);
}

file
file::create(const directory& directory,
             const pathname& pathname,
             const mode mode) {
  const int flags = O_CREAT | O_WRONLY | O_TRUNC;
  return open(directory.fd(), pathname, flags, mode);
}

file
file::open(const pathname& pathname,
           const int flags) {
  return open(AT_FDCWD, pathname, flags);
}

file
file::open(const directory& directory,
           const pathname& pathname,
           const int flags) {
  return open(directory.fd(), pathname, flags);
}

std::size_t
file::size() const {
  struct stat st;

  if (fstat(_fd, &st) == -1) {
    switch (errno) {
      case ENOMEM: /* Cannot allocate memory in kernel */
        throw posix::fatal_error(errno);
      default:
        throw posix::error(errno);
    }
  }

  return static_cast<std::size_t>(st.st_size);
}
