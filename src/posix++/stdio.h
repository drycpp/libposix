/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_STDIO_H
#define POSIXXX_STDIO_H

#ifndef __cplusplus
#error "<posix++/stdio.h> requires a C++ compiler"
#endif

////////////////////////////////////////////////////////////////////////////////

#include <cstddef> /* for std::size_t */
#include <string>  /* for std::string */

namespace posix {
  struct descriptor; // @see <posix++/descriptor.h>

  /**
   * Returns a reference to the descriptor for the standard input stream.
   */
  const descriptor& standard_input() noexcept;

  /**
   * Returns a reference to the descriptor for the standard output stream.
   */
  const descriptor& standard_output() noexcept;

  /**
   * Returns a reference to the descriptor for the standard error stream.
   */
  const descriptor& standard_error() noexcept;

  /**
   * Writes some data to the given descriptor, handling `EINTR`/`EAGAIN`.
   *
   * @throws posix::error on failure
   */
  void write(int fd, const void* buffer, std::size_t count);

  /**
   * Writes a string to the given descriptor, handling `EINTR`/`EAGAIN`.
   *
   * @throws posix::error on failure
   */
  inline void write(const int fd, const std::string& string) {
    write(fd, string.data(), string.size());
  }
}

////////////////////////////////////////////////////////////////////////////////

#endif /* POSIXXX_STDIO_H */
