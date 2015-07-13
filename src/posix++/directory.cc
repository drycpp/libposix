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

////////////////////////////////////////////////////////////////////////////////

directory
directory::temporary(const char* const /*basename*/) {
  return open("/tmp"); // TODO: improve on this
}

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
  //flags |= O_NOATIME;   /* Linux-specific */ // FIXME
#endif

  int fd;
  if ((fd = openat(dirfd, pathname, flags)) == -1) {
    throw_error("openat", "%d, \"%s\", 0x%x",
      dirfd, pathname, static_cast<unsigned int>(flags));
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

////////////////////////////////////////////////////////////////////////////////

std::size_t
directory::count(const char* const pathname) const {
  assert(pathname != nullptr);
  assert(*pathname != '\0');

  struct stat buffer;

  if (fstatat(fd(), pathname, &buffer, AT_SYMLINK_NOFOLLOW) == -1) {
    switch (errno) {
      case ENOENT:  /* No such file or directory */
        return 0;
      default:
        throw_error("fstatat", "%d, \"%s\", %s, %s", fd(), pathname, "buffer", "AT_SYMLINK_NOFOLLOW");
    }
  }

  return 1;
}

bool
directory::stat(const char* const pathname,
                struct stat& result,
                const int flags) const {
  assert(pathname != nullptr);
  assert(*pathname != '\0');

  if (fstatat(fd(), pathname, &result, flags) == -1) {
    switch (errno) {
      case ENOENT:  /* No such file or directory */
        return false;
      default:
        throw_error("fstatat", "%d, \"%s\", %p, 0x%x", fd(), pathname, &result, flags);
    }
  }

  return true;
}

void
directory::link(const char* const old_pathname,
                const char* const new_pathname) const {
  assert(old_pathname != nullptr);
  assert(*old_pathname != '\0');
  assert(new_pathname != nullptr);
  assert(*new_pathname != '\0');

  if (linkat(fd(), old_pathname, fd(), new_pathname, 0) == -1) {
    throw_error("linkat", "%d, \"%s\", %d, \"%s\", 0x%x",
      fd(), old_pathname, fd(), new_pathname, 0U);
  }
}

void
directory::symlink(const char* const old_pathname,
                   const char* const new_pathname) const {
  assert(old_pathname != nullptr);
  assert(*old_pathname != '\0');
  assert(new_pathname != nullptr);
  assert(*new_pathname != '\0');

  if (symlinkat(old_pathname, fd(), new_pathname) == -1) {
    throw_error("symlinkat", "\"%s\", %d, \"%s\"",
      old_pathname, fd(), new_pathname);
  }
}

void
directory::mkdir(const char* const pathname,
                 const mode mode) const {
  assert(pathname != nullptr);
  assert(*pathname != '\0');

  if (mkdirat(fd(), pathname, static_cast<mode_t>(mode)) == -1) {
    throw_error("mkdirat", "%d, \"%s\", 0%o",
      fd(), pathname, static_cast<unsigned int>(mode));
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
    throw_error("unlinkat", "%d, \"%s\", 0x%x",
      fd(), pathname, static_cast<unsigned int>(flags));
  }
}

void
directory::rename(const char* const old_pathname,
                  const char* const new_pathname) const {
  rename(old_pathname, *this, new_pathname);
}

void
directory::rename(const char* const old_pathname,
                  const directory& new_directory,
                  const char* const new_pathname) const {
  assert(old_pathname != nullptr);
  assert(*old_pathname != '\0');
  assert(new_pathname != nullptr);
  assert(*new_pathname != '\0');

  if (renameat(fd(), old_pathname, new_directory.fd(), new_pathname) == -1) {
    throw_error("renameat", "%d, \"%s\", %d, \"%s\"",
      fd(), old_pathname, new_directory.fd(), new_pathname);
  }
}

pathname
directory::readlink(const char* const pathname) const {
  char buffer[PATH_MAX];

  const int rc = readlinkat(fd(), pathname, buffer, sizeof(buffer) - 1);
  if (rc == -1) {
    throw_error("readlinkat", "%d, \"%s\", %s, %zu", fd(), pathname, "buffer", sizeof(buffer) - 1);
  }

  assert(rc < sizeof(buffer));
  buffer[rc] = '\0';

  return posix::pathname(buffer);
}

void
directory::for_each(std::function<void (const entry&)> callback) const {
  const int dirfd = dup(/*true*/).release();

  DIR* dir;
  if (!(dir = fdopendir(dirfd))) {
    throw_error("fdopendir", "%d", dirfd);
  }

  entry entry {0, 0, {}};

  try {
    const struct dirent* dirent;
    while ((dirent = readdir(dir))) {
#ifdef __linux__
      entry.type = dirent->d_type;
#endif
      entry.inode = dirent->d_ino;
      entry.name = dirent->d_name;
      callback(entry);
    }

    closedir(dir);
  }
  catch (...) {
    /* std::bad_alloc from std::string, or else of callback() origin */
    closedir(dir);
    throw;
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
  const int dirfd = dir.dup(/*true*/).release();

  if (!(_dirp = fdopendir(dirfd))) {
    throw_error("fdopendir", "%d", dirfd);
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
