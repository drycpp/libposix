/* This is free and unencumbered software released into the public domain. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "directory.h"
#include "error.h"
#include "named_pipe.h"
#include "pathname.h"

#include <cassert>     /* for assert() */
#include <cerrno>      /* for errno */
#include <fcntl.h>     /* for AT_FDCWD, O_CLOEXEC */
#include <sys/stat.h>  /* for mkfifoat() */
#include <sys/types.h> /* for struct stat */

using namespace posix;

void
named_pipe::create(const int dirfd,
                   const char* const pathname,
                   const mode mode) {
  assert(dirfd > 0 || dirfd == AT_FDCWD);
  assert(pathname);
  assert(pathname[0] != '\0');

  if (mkfifoat(dirfd, pathname, mode) == -1) {
    switch (errno) {
      case EBADF:  /* Bad file descriptor */
        throw posix::bad_descriptor();
      case ENOTDIR: /* Not a directory */
        throw posix::logic_error(errno);
      default:
        throw posix::runtime_error(errno);
    }
  }
}

named_pipe
named_pipe::open(const int dirfd,
                 const char* const pathname,
                 int flags,
                 const mode mode) {
  assert(dirfd > 0 || dirfd == AT_FDCWD);
  assert(pathname);
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

  return named_pipe(fd);
}

void
named_pipe::create(const pathname& pathname,
                   const mode mode) {
  return create(AT_FDCWD, pathname.c_str(), mode);
}

void
named_pipe::create(const directory& directory,
                   const pathname& pathname,
                   const mode mode) {
  return create(directory.fd(), pathname.c_str(), mode);
}

named_pipe
named_pipe::open(const pathname& pathname,
                 const int flags,
                 const mode mode) {
  return open(AT_FDCWD, pathname.c_str(), flags, mode);
}

named_pipe
named_pipe::open(const directory& directory,
                 const pathname& pathname,
                 const int flags,
                 const mode mode) {
  return open(directory.fd(), pathname.c_str(), flags, mode);
}
