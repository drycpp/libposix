/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_ERROR_H
#define POSIXXX_ERROR_H

#include <system_error> /* for std::error_code, std::system_error */

namespace posix {
  class error;
  class fatal_error;
}

/**
 * Represents a POSIX error condition.
 *
 * @see http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/errno.h.html
 */
class posix::error : public std::system_error {
  public:
    /**
     * Default constructor.
     */
    error() noexcept;

    /**
     * Constructor.
     */
    error(const std::error_code code) noexcept
      : std::system_error(code) {}

    /**
     * Constructor.
     */
    error(const int code) noexcept
      : std::system_error(code, std::generic_category()) {}
};

class posix::fatal_error : public posix::error {
  public:
    /**
     * Default constructor.
     */
    fatal_error() noexcept
      : error() {}

    /**
     * Constructor.
     */
    fatal_error(const std::error_code code) noexcept
      : error(code) {}

    /**
     * Constructor.
     */
    fatal_error(const int code) noexcept
      : error(code) {}
};

#endif /* POSIXXX_ERROR_H */
