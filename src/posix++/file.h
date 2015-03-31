/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_FILE_H
#define POSIXXX_FILE_H

#ifndef __cplusplus
#error "<posix++/file.h> requires a C++ compiler"
#endif

////////////////////////////////////////////////////////////////////////////////

#include "descriptor.h"
#include "mode.h"

#include <cstddef>  /* for std::size_t */
#include <unistd.h> /* for SEEK_*, off_t */
#include <utility>  /* for std::move() */

namespace posix {
  class directory;
  class file;
  class pathname;
}

////////////////////////////////////////////////////////////////////////////////

/**
 * Represents a POSIX file.
 *
 * @see http://pubs.opengroup.org/onlinepubs/009695399/basedefs/xbd_chap03.html#tag_03_163
 */
class posix::file : public posix::descriptor {
public:
  static file create(const pathname& pathname, mode mode);

  static file create(const directory& directory, const pathname& pathname, mode mode);

  static file open(const pathname& pathname, int flags, mode mode = 0);

  static file open(const directory& directory, const pathname& pathname, int flags, mode mode = 0);

  /**
   * Default constructor.
   */
  file() noexcept
    : descriptor{} {}

  /**
   * Constructor.
   */
  file(int dirfd, const char* pathname, int flags, mode mode = 0);

  /**
   * Constructor.
   */
  file(const int fd) noexcept
    : descriptor{fd} {}

  /**
   * Copy constructor.
   */
  file(const file& other) /* may throw */
    : descriptor{other} {}

  /**
   * Move constructor.
   */
  file(file&& other) noexcept
    : descriptor{std::move(other)} {}

  /**
   * Destructor.
   */
  ~file() noexcept = default;

  /**
   * Checks whether this file's size is zero.
   */
  bool empty() const {
    return size() == 0;
  }

  /**
   * Returns the current file size in bytes.
   */
  std::size_t size() const;

  /**
   * Returns the current file offset.
   */
  std::size_t offset() const {
    return seek(0, SEEK_CUR);
  }

  /**
   * Sets the file offset to the beginning of the file.
   */
  void rewind() const {
    seek(0, SEEK_SET);
  }

  /**
   * Returns or changes the current file offset.
   */
  std::size_t seek(off_t offset, int whence = SEEK_SET) const;

  /**
   * ...
   */
  void allocate(off_t offset, off_t length) const;

  /**
   * ...
   */
  void truncate(off_t length = 0) const;
};

////////////////////////////////////////////////////////////////////////////////

#endif /* POSIXXX_FILE_H */
