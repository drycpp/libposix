/* This is free and unencumbered software released into the public domain. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "file.h"

#include "directory.h"
#include "error.h"
#include "pathname.h"

#include <cassert>     /* for assert() */
#include <cerrno>      /* for errno */
#include <fcntl.h>     /* for AT_FDCWD, O_CLOEXEC, posix_fallocate() */
#include <sys/stat.h>  /* for fstat() */
#include <sys/types.h> /* for struct stat */
#include <unistd.h>    /* for ftruncate(), lseek() */

using namespace posix;

////////////////////////////////////////////////////////////////////////////////

file
file::create(const pathname& pathname,
             const mode mode) {

  const int flags = O_CREAT | O_WRONLY | O_TRUNC;
  return file{AT_FDCWD, pathname.c_str(), flags, mode};
}

file
file::create(const directory& directory,
             const pathname& pathname,
             const mode mode) {

  const int flags = O_CREAT | O_WRONLY | O_TRUNC;
  return file{directory.fd(), pathname.c_str(), flags, mode};
}

file
file::open(const pathname& pathname,
           const int flags,
           const mode mode) {

  return file{AT_FDCWD, pathname.c_str(), flags, mode};
}

file
file::open(const directory& directory,
           const pathname& pathname,
           const int flags,
           const mode mode) {

  return file{directory.fd(), pathname.c_str(), flags, mode};
}

////////////////////////////////////////////////////////////////////////////////

file::file(const int dirfd,
           const char* const pathname,
           int flags,
           const mode mode)
  : descriptor{} {

  assert(dirfd > 0 || dirfd == AT_FDCWD);
  assert(pathname != nullptr);
  assert(pathname[0] != '\0');

#ifdef O_CLOEXEC
  flags |= O_CLOEXEC; /* POSIX.1-2008 (Linux, FreeBSD) */
#endif

  if ((_fd = openat(dirfd, pathname, flags, mode)) == -1) {
    throw_error("openat", "%d, \"%s\", 0x%x, 0%o", dirfd, pathname,
      flags, static_cast<unsigned int>(mode));
  }
}

////////////////////////////////////////////////////////////////////////////////

std::size_t
file::size() const {
  struct stat st;

  if (fstat(fd(), &st) == -1) {
    assert(errno != EFAULT);
    throw_error("fstat", "%d, %s", fd(), "buffer");
  }

  return static_cast<std::size_t>(st.st_size);
}

std::size_t
file::seek(const off_t offset,
           const int whence) const {
  off_t result;

  if ((result = lseek(fd(), offset, whence)) == static_cast<off_t>(-1)) {
    throw_error("lseek", "%d, 0x%lx, %d",
      fd(), static_cast<unsigned long>(offset), whence);
  }

  assert(result >= 0);
  return static_cast<std::size_t>(result);
}

void
file::allocate(const off_t offset,
               const off_t length) const {
  int err;
  if ((err = posix_fallocate(fd(), offset, length)) != 0) {
    throw_error(err, "posix_fallocate", "%d, 0x%08lx, 0x%08lx", fd(),
      static_cast<unsigned long>(offset),
      static_cast<unsigned long>(length));
  }
}

void
file::truncate(const off_t length) const {
  if (ftruncate(fd(), length) == -1) {
    throw_error("ftruncate", "%d, 0x%08lx", fd(),
      static_cast<unsigned long>(length));
  }
}
