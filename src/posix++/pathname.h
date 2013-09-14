/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_PATHNAME_H
#define POSIXXX_PATHNAME_H

#include <stdexcept> /* for std::invalid_argument */
#include <string>    /* for std::string */

namespace posix {
  class pathname;
}

/**
 * Represents a POSIX pathname.
 *
 * @see http://pubs.opengroup.org/onlinepubs/009695399/basedefs/xbd_chap03.html#tag_03_266
 */
class posix::pathname {
public:
  /**
   * Constructor.
   */
  pathname(const std::string& path) : _string(path) {}

  /**
   * Constructor.
   */
  pathname(const char* const path) : _string(path) {
    if (!path) {
      throw std::invalid_argument("path cannot be nullptr");
    }
  }

  /**
   * Destructor.
   */
  ~pathname() noexcept = default;

  /**
   * ...
   */
  bool empty() const noexcept {
    return _string.empty();
  }

  /**
   * ...
   */
  const std::string& string() const noexcept {
    return _string;
  }

  /**
   * ...
   */
  const char* c_str() const noexcept {
    return _string.c_str();
  }

  /**
   * Determines whether this is an absolute pathname.
   *
   * @see http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap03.html#tag_03_02
   */
  bool is_absolute() const noexcept {
    return _string[0] == '/';
  }

  /**
   * Determines whether this is a relative pathname.
   *
   * @see http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap03.html#tag_03_320
   */
  inline bool is_relative() const noexcept {
    return !is_absolute();
  }

  /**
   * Determines whether this is a portable pathname.
   *
   * @see http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap03.html#tag_03_277
   */
  bool is_portable() const noexcept;

  /**
   * @see http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap03.html#tag_03_129
   */
  posix::pathname dirname() const;

  /**
   * @see http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap03.html#tag_03_40
   */
  posix::pathname basename() const;

protected:
  std::string _string;
};

#endif /* POSIXXX_PATHNAME_H */
