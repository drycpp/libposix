/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_ERROR_H
#define POSIXXX_ERROR_H

#include <system_error> /* for std::system_category, std::system_error */

namespace posix {
  class error;
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
      : std::system_error(code, std::system_category()) {}
};

#endif /* POSIXXX_ERROR_H */
