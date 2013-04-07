/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_PATHNAME_H
#define POSIXXX_PATHNAME_H

#include <string> /* for std::string */

namespace posix { class pathname; }

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
    pathname(const char* const path) : _string(path) {}

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
     * ...
     */
    posix::pathname dirname() const;

    /**
     * ...
     */
    posix::pathname basename() const;

  protected:
    std::string _string;
};

#endif /* POSIXXX_PATHNAME_H */
