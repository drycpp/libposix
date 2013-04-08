/* This is free and unencumbered software released into the public domain. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "file.h"

#include "directory.h"
#include "pathname.h"

#include <cassert>      /* for assert() */
#include <cerrno>       /* for errno */
#include <fcntl.h>      /* for AT_FDCWD */
#include <sys/stat.h>   /* for fstat() */
#include <sys/types.h>  /* for struct stat */
#include <system_error> /* for std::system_error */
#include <unistd.h>     /* for lseek() */

using namespace posix;

static_assert(sizeof(mode_t) <= sizeof(file_mode),
  "sizeof(mode_t) > sizeof(file_mode)");

file
file::open(const int dirfd,
           const pathname& pathname,
           const int flags,
           const file_mode mode) {
  assert(dirfd > 0 || dirfd == AT_FDCWD);
  assert(!pathname.empty());

  int fd;
  if ((fd = openat(dirfd, pathname.c_str(), flags, mode)) == -1) {
    switch (errno) {
      case EMFILE: /* Too many open files */
      case ENFILE: /* Too many open files in system */
      case ENOMEM: /* Cannot allocate memory in kernel */
        throw std::system_error(errno, std::system_category()); // FIXME
      default:
        throw std::system_error(errno, std::system_category());
    }
  }

  return file(fd);
}

file
file::create(const pathname& pathname,
             const file_mode mode) {
  const int flags = O_CREAT | O_WRONLY | O_TRUNC;
  return open(AT_FDCWD, pathname, flags, mode);
}

file
file::create(const directory& directory,
             const pathname& pathname,
             const file_mode mode) {
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
      case ENOMEM: /* Out of memory in the kernel */
        throw std::system_error(errno, std::system_category()); // FIXME
      default:
        throw std::system_error(errno, std::system_category());
    }
  }

  return static_cast<std::size_t>(st.st_size);
}
