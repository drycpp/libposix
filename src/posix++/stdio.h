/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_STDIO_H
#define POSIXXX_STDIO_H

#include "descriptor.h"

namespace posix {
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
}

#endif /* POSIXXX_STDIO_H */
