/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_ERROR_H
#define POSIXXX_ERROR_H

#include <system_error> /* for std::error_*, std::system_* */

namespace posix {
  class error;
  class fatal_error;
}

/**
 * Represents a POSIX runtime error.
 *
 * @see http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/errno.h.html
 */
class posix::error : public std::system_error {
  public:
    /**
     * Default constructor.
     *
     * Will use the current value of `errno` as the error code.
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
    error(const std::error_code code,
          const char* const what) noexcept
      : std::system_error(code, what) {}

    /**
     * Constructor.
     */
    error(const int code) noexcept
      : std::system_error(code, std::generic_category()) {}

    /**
     * Constructor.
     */
    error(const int code,
          const std::error_category& category) noexcept
      : std::system_error(code, category) {}

    /**
     * Constructor.
     */
    error(const int code,
          const std::error_category& category,
          const char* const what) noexcept
      : std::system_error(code, category, what) {}
};

/**
 * Represents a POSIX runtime error that cannot be recovered from.
 *
 * @see http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/errno.h.html
 */
class posix::fatal_error : public posix::error {
  public:
    /**
     * Default constructor.
     *
     * Will use the current value of `errno` as the error code.
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
    fatal_error(const std::error_code code,
                const char* const what) noexcept
      : error(code, what) {}

    /**
     * Constructor.
     */
    fatal_error(const int code) noexcept
      : error(code) {}

    /**
     * Constructor.
     */
    fatal_error(const int code,
                const std::error_category& category) noexcept
      : error(code, category) {}

    /**
     * Constructor.
     */
    fatal_error(const int code,
                const std::error_category& category,
                const char* const what) noexcept
      : error(code, category, what) {}
};

#endif /* POSIXXX_ERROR_H */
