/* This is free and unencumbered software released into the public domain. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "error.h"

#include <cassert> /* for assert() */
#include <cerrno>  /* for EBADF, errno */

using namespace posix;

error::error() noexcept
  : std::system_error(errno, std::system_category()) {}

bad_descriptor::bad_descriptor() noexcept
  : error(EBADF) {}