/* This is free and unencumbered software released into the public domain. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "directory.h"
#include "error.h"
#include "pathname.h"

#include <cassert>      /* for assert() */
#include <cstdio>       /* for renameat() */
#include <dirent.h>     /* for fdopendir() */
#include <fcntl.h>      /* for AT_FDCWD, fcntl() */
#include <unistd.h>     /* for close(), readlinkat(), unlinkat() */
#include <sys/stat.h>   /* for fstatat() */
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
        throw posix::runtime_error(errno);
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

std::size_t
directory::count(const char* const pathname) const {
  assert(pathname != nullptr);
  assert(*pathname != '\0');

  struct stat buffer;

  if (fstatat(fd(), pathname, &buffer, AT_SYMLINK_NOFOLLOW) == -1) {
    switch (errno) {
      case ENOENT:  /* No such file or directory */
        return 0;
      case ENOMEM:  /* Cannot allocate memory in kernel */
        throw posix::fatal_error(errno);
      case EBADF:   /* Bad file descriptor */
        throw posix::bad_descriptor();
      case EFAULT:  /* Bad address */
        throw posix::bad_address();
      case EINVAL:  /* Invalid argument */
        throw posix::invalid_argument();
      case ENAMETOOLONG: /* File name too long */
      case ENOTDIR: /* Not a directory */
        throw posix::logic_error(errno);
      case EACCES:  /* Permission denied */
      case ELOOP:   /* Too many levels of symbolic links */
      default:
        throw posix::runtime_error(errno);
    }
  }

  return 1;
}

void
directory::link(const char* const old_pathname,
                const char* const new_pathname) const {
  assert(old_pathname != nullptr);
  assert(*old_pathname != '\0');
  assert(new_pathname != nullptr);
  assert(*new_pathname != '\0');

  if (linkat(fd(), old_pathname, fd(), new_pathname, 0) == -1) {
    switch (errno) {
      case ENOMEM:  /* Cannot allocate memory in kernel */
        throw posix::fatal_error(errno);
      case EBADF:   /* Bad file descriptor */
        throw posix::bad_descriptor();
      case EFAULT:  /* Bad address */
        throw posix::bad_address();
      case EINVAL:  /* Invalid argument */
        throw posix::invalid_argument();
      case ENAMETOOLONG: /* File name too long */
      case ENOTDIR: /* Not a directory */
        throw posix::logic_error(errno);
      default:
        throw posix::runtime_error(errno);
    }
  }
}

void
directory::mkdir(const char* const pathname,
                 const mode mode) const {
  assert(pathname != nullptr);
  assert(*pathname != '\0');

  if (mkdirat(fd(), pathname, static_cast<mode_t>(mode)) == -1) {
    switch (errno) {
      case ENOMEM:  /* Cannot allocate memory in kernel */
        throw posix::fatal_error(errno);
      case EBADF:   /* Bad file descriptor */
        throw posix::bad_descriptor();
      case EFAULT:  /* Bad address */
        throw posix::bad_address();
      case EINVAL:  /* Invalid argument */
        throw posix::invalid_argument();
      case ENAMETOOLONG: /* File name too long */
      case ENOTDIR: /* Not a directory */
        throw posix::logic_error(errno);
      default:
        throw posix::runtime_error(errno);
    }
  }
}

void
directory::rmdir(const char* const pathname) const {
  return unlink(pathname, AT_REMOVEDIR);
}

void
directory::unlink(const char* const pathname) const {
  return unlink(pathname, 0);
}

void
directory::unlink(const char* const pathname,
                  const int flags) const {
  assert(pathname != nullptr);
  assert(*pathname != '\0');

  if (unlinkat(fd(), pathname, flags) == -1) {
    switch (errno) {
      case ENOMEM:  /* Cannot allocate memory in kernel */
        throw posix::fatal_error(errno);
      case EBADF:   /* Bad file descriptor */
        throw posix::bad_descriptor();
      case EFAULT:  /* Bad address */
        throw posix::bad_address();
      case EINVAL:  /* Invalid argument */
        throw posix::invalid_argument();
      case ENAMETOOLONG: /* File name too long */
      case ENOTDIR: /* Not a directory */
        throw posix::logic_error(errno);
      default:
        throw posix::runtime_error(errno);
    }
  }
}

void
directory::rename(const char* const old_pathname,
                  const char* const new_pathname) const {
  assert(old_pathname != nullptr);
  assert(*old_pathname != '\0');
  assert(new_pathname != nullptr);
  assert(*new_pathname != '\0');

  if (renameat(fd(), old_pathname, fd(), new_pathname) == -1) {
    switch (errno) {
      case ENOMEM:  /* Cannot allocate memory in kernel */
      case ENOSPC:  /* No space left on device */
        throw posix::fatal_error(errno);
      case EBADF:   /* Bad file descriptor */
        throw posix::bad_descriptor();
      case EFAULT:  /* Bad address */
        throw posix::bad_address();
      case EINVAL:  /* Invalid argument */
        throw posix::invalid_argument();
      case ENAMETOOLONG: /* File name too long */
      case ENOTDIR: /* Not a directory */
        throw posix::logic_error(errno);
      default:
        throw posix::runtime_error(errno);
    }
  }
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
        throw posix::runtime_error(errno);
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
        throw posix::runtime_error(errno);
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
