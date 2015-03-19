/* This is free and unencumbered software released into the public domain. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "descriptor.h"
#include "error.h"
#include "stdio.h"

#include <array>    /* for std::array */
#include <cassert>  /* for assert() */
#include <cerrno>   /* for E*, errno */
#include <cstdint>  /* for std::uint8_t */
#include <unistd.h> /* for STD{IN,OUT,ERR}_FILENO, write() */

using namespace posix;

static struct {
  std::array<std::uint8_t, sizeof(descriptor)> standard_input;
  std::array<std::uint8_t, sizeof(descriptor)> standard_output;
  std::array<std::uint8_t, sizeof(descriptor)> standard_error;
} static_data;

const descriptor&
posix::standard_input() noexcept {
  static const descriptor* const stdin =
    new(static_data.standard_input.data()) descriptor(STDIN_FILENO);
  return *stdin;
}

const descriptor&
posix::standard_output() noexcept {
  static const descriptor* const stdout =
    new(static_data.standard_output.data()) descriptor(STDOUT_FILENO);
  return *stdout;
}

const descriptor&
posix::standard_error() noexcept {
  static const descriptor* const stderr =
    new(static_data.standard_error.data()) descriptor(STDERR_FILENO);
  return *stderr;
}

void
posix::write(const int fd, const void* const buffer, const std::size_t count) {
  assert(fd > 0);
  assert(buffer != nullptr);

  std::size_t position = 0;

  while (position < count) {
    const ssize_t rc = ::write(fd, reinterpret_cast<const uint8_t*>(buffer) + position, count - position);
    if (rc == -1) {
      switch (errno) {
        case EINTR:
        case EAGAIN:
          continue; /* try again */
        default:
          throw_error("write", "%d, %s, %zu", fd, "chunk", count - position);
      }
    }
    position += static_cast<decltype(position)>(rc);
  }
}
