/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_DIRECTORY_H
#define POSIXXX_DIRECTORY_H

#include "descriptor.h"

#include <string>  /* for std::string */
#include <utility> /* for std::move() */

namespace posix {
  class directory;
  class pathname;
}

/**
 * Represents a POSIX directory.
 *
 * @see http://pubs.opengroup.org/onlinepubs/009695399/basedefs/xbd_chap03.html#tag_03_128
 */
class posix::directory : public posix::descriptor {
public:
  class iterator;

  /**
   * Opens an existing directory.
   */
  static directory open(const pathname& pathname);

  /**
   * Opens an existing directory.
   */
  static directory open(const char* pathname);

  /**
   * Opens an existing directory.
   */
  static directory open(const directory& directory, const pathname& pathname);

  /**
   * Opens an existing directory.
   */
  static directory open(const directory& directory, const char* pathname);

  /**
   * Constructor.
   */
  directory(const int fd) noexcept
    : descriptor(fd) {}

  /**
   * Copy constructor.
   */
  directory(const directory& other) /* may throw */
    : descriptor(other) {}

  /**
   * Move constructor.
   */
  directory(directory&& other) noexcept
    : descriptor(std::move(other)) {}

  /**
   * Destructor.
   */
  ~directory() noexcept = default;

  // TODO: copy/move assignment operators.

  /**
   * Reads the target path of a symbolic link.
   */
  pathname readlink(const char* pathname) const;

  iterator begin() const;

  iterator end() const;

  const iterator cbegin() const;

  const iterator cend() const;

protected:
  static directory open(int dirfd, const char* pathname);
};

/**
 * POSIX directory iterator.
 */
class posix::directory::iterator {
public:
  /**
   * Default constructor.
   */
  iterator() noexcept = default;

  /**
   * Constructor.
   */
  iterator(const directory& dir);

  /**
   * Destructor.
   */
  ~iterator() noexcept;

  bool operator==(const iterator& other) {
    return !operator!=(other);
  }

  bool operator!=(const iterator& other);

  iterator& operator++();

  std::string operator*();

protected:
  void* _dirp = nullptr;
};

#endif /* POSIXXX_DIRECTORY_H */
