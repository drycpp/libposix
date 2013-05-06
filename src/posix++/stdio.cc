/* This is free and unencumbered software released into the public domain. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "stdio.h"

#include <array>    /* for std::array */
#include <cstdint>  /* for std::uint8_t */
#include <unistd.h> /* for STD{IN,OUT,ERR}_FILENO */

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
