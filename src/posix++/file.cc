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
#include <fcntl.h>      /* for AT_FDCWD, O_CLOEXEC */
#include <sys/stat.h>   /* for fstat() */
#include <sys/types.h>  /* for struct stat */
#include <unistd.h>     /* for lseek() */

using namespace posix;

file
file::open(const int dirfd,
           const char* const pathname,
           int flags,
           const mode mode) {
  assert(dirfd > 0 || dirfd == AT_FDCWD);
  assert(pathname != nullptr);
  assert(pathname[0] != '\0');

#ifdef O_CLOEXEC
  flags |= O_CLOEXEC; /* POSIX.1-2008 (Linux, FreeBSD) */
#endif

  int fd;
  if ((fd = openat(dirfd, pathname, flags, mode)) == -1) {
    switch (errno) {
      case EMFILE: /* Too many open files */
      case ENFILE: /* Too many open files in system */
      case ENOMEM: /* Cannot allocate memory in kernel */
        throw posix::fatal_error(errno);
      case EBADF:  /* Bad file descriptor */
        throw posix::bad_descriptor();
      default:
        throw posix::runtime_error(errno);
    }
  }

  return file(fd);
}

file
file::create(const pathname& pathname,
             const mode mode) {
  const int flags = O_CREAT | O_WRONLY | O_TRUNC;
  return open(AT_FDCWD, pathname.c_str(), flags, mode);
}

file
file::create(const directory& directory,
             const pathname& pathname,
             const mode mode) {
  const int flags = O_CREAT | O_WRONLY | O_TRUNC;
  return open(directory.fd(), pathname.c_str(), flags, mode);
}

file
file::open(const pathname& pathname,
           const int flags,
           const mode mode) {
  return open(AT_FDCWD, pathname.c_str(), flags, mode);
}

file
file::open(const directory& directory,
           const pathname& pathname,
           const int flags,
           const mode mode) {
  return open(directory.fd(), pathname.c_str(), flags, mode);
}

std::size_t
file::size() const {
  struct stat st;

  if (fstat(_fd, &st) == -1) {
    switch (errno) {
      case ENOMEM: /* Cannot allocate memory in kernel */
        throw posix::fatal_error(errno);
      case EBADF:  /* Bad file descriptor */
        throw posix::bad_descriptor();
      default:
        throw posix::runtime_error(errno);
    }
  }

  return static_cast<std::size_t>(st.st_size);
}
