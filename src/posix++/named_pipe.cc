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
    throw_error("mkfifoat");
  }
}

named_pipe
named_pipe::open(const int dirfd,
                 const char* const pathname,
                 int flags) {
  assert(dirfd > 0 || dirfd == AT_FDCWD);
  assert(pathname);
  assert(pathname[0] != '\0');

#ifdef O_CLOEXEC
  flags |= O_CLOEXEC; /* POSIX.1-2008 (Linux, FreeBSD) */
#endif

  int fd;
  if ((fd = openat(dirfd, pathname, flags)) == -1) {
    throw_error("openat");
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
                 const int flags) {
  return open(AT_FDCWD, pathname.c_str(), flags);
}

named_pipe
named_pipe::open(const directory& directory,
                 const pathname& pathname,
                 const int flags) {
  return open(directory.fd(), pathname.c_str(), flags);
}
