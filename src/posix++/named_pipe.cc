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
#include <sys/stat.h>  /* for fstat() */
#include <sys/types.h> /* for struct stat */

using namespace posix;

named_pipe
named_pipe::create(const int dirfd,
                   const char* const pathname,
                   const mode mode) {
  assert(dirfd > 0 || dirfd == AT_FDCWD);
  assert(pathname != nullptr);
  assert(pathname[0] != '\0');

  (void)dirfd, (void)pathname, (void)mode;
  return named_pipe(-1); // TODO
}

named_pipe
named_pipe::open(const int dirfd,
                 const char* const pathname,
                 int flags,
                 const mode mode) {
  assert(dirfd > 0 || dirfd == AT_FDCWD);
  assert(pathname != nullptr);
  assert(pathname[0] != '\0');

  (void)dirfd, (void)pathname, (void)flags, (void)mode;
  return named_pipe(-1); // TODO
}

named_pipe
named_pipe::create(const pathname& pathname,
                   const mode mode) {
  return create(AT_FDCWD, pathname.c_str(), mode);
}

named_pipe
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
