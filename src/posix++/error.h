/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_ERROR_H
#define POSIXXX_ERROR_H

#ifndef __cplusplus
#error "<posix++/error.h> requires a C++ compiler"
#endif

////////////////////////////////////////////////////////////////////////////////

#include <cstdarg>      /* for va_list */
#include <system_error> /* for std::error_*, std::system_* */

namespace posix {
  class error;
  class logic_error;
  class runtime_error;
  class fatal_error;
  class bad_descriptor;
  class bad_address;
  class invalid_argument;
  class connection_refused;

  extern void throw_error[[noreturn]]();

  extern void throw_error[[noreturn]](const char* origin, const char* format = nullptr, ...);

  extern void throw_error[[noreturn]](int code);
  extern void throw_error[[noreturn]](int code, const char* origin, const char* format = nullptr, ...);
  extern void throw_error[[noreturn]](int code, const char* origin, const char* format, va_list args);
}

////////////////////////////////////////////////////////////////////////////////

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
    : std::system_error{code} {}

  /**
   * Constructor.
   */
  error(const std::error_code code,
        const char* const what) noexcept
    : std::system_error{code, what} {}

  /**
   * Constructor.
   */
  error(const int code) noexcept
    : std::system_error{code, std::generic_category()} {}

  /**
   * Constructor.
   */
  error(const int code,
        const std::error_category& category) noexcept
    : std::system_error{code, category} {}

  /**
   * Constructor.
   */
  error(const int code,
        const std::error_category& category,
        const char* const what) noexcept
    : std::system_error{code, category, what} {}

  /**
   * Returns the error number.
   */
  int number() const noexcept {
    return code().value();
  }
};

////////////////////////////////////////////////////////////////////////////////

/**
 * Represents a POSIX logic error.
 *
 * @see http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/errno.h.html
 */
class posix::logic_error : public posix::error {
public:
  using posix::error::error; /* inherit constructors */
};

////////////////////////////////////////////////////////////////////////////////

/**
 * Represents a POSIX runtime error.
 *
 * @see http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/errno.h.html
 */
class posix::runtime_error : public posix::error {
public:
  using posix::error::error; /* inherit constructors */
};

////////////////////////////////////////////////////////////////////////////////

/**
 * Represents a POSIX runtime error that cannot be recovered from.
 *
 * @see http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/errno.h.html
 */
class posix::fatal_error : public posix::error {
public:
  using posix::error::error; /* inherit constructors */
};

////////////////////////////////////////////////////////////////////////////////

/**
 * Represents a POSIX `EBADF` (Bad file descriptor) error.
 *
 * @see http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/errno.h.html
 */
class posix::bad_descriptor : public posix::logic_error {
public:
  /**
   * Default constructor.
   */
  bad_descriptor() noexcept;

  /**
   * Constructor.
   */
  bad_descriptor(const char* origin) noexcept;
};

////////////////////////////////////////////////////////////////////////////////

/**
 * Represents a POSIX `EFAULT` (Bad address) error.
 *
 * @see http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/errno.h.html
 */
class posix::bad_address : public posix::logic_error {
public:
  /**
   * Default constructor.
   */
  bad_address() noexcept;

  /**
   * Constructor.
   */
  bad_address(const char* origin) noexcept;
};

////////////////////////////////////////////////////////////////////////////////

/**
 * Represents a POSIX `EINVAL` (Invalid argument) error.
 *
 * @see http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/errno.h.html
 */
class posix::invalid_argument : public posix::logic_error {
public:
  /**
   * Default constructor.
   */
  invalid_argument() noexcept;

  /**
   * Constructor.
   */
  invalid_argument(const char* origin) noexcept;
};

////////////////////////////////////////////////////////////////////////////////

/**
 * Represents a POSIX `ECONNREFUSED` (Connection refused) error.
 *
 * @see http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/errno.h.html
 */
class posix::connection_refused : public posix::runtime_error {
public:
  /**
   * Default constructor.
   */
  connection_refused() noexcept;

  /**
   * Constructor.
   */
  connection_refused(const char* origin) noexcept;
};

////////////////////////////////////////////////////////////////////////////////

#endif /* POSIXXX_ERROR_H */
