/* This is free and unencumbered software released into the public domain. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "directory.h"
#include "error.h"
#include "mapped_file.h"
#include "pathname.h"

#include <cassert> /* for assert() */
#include <cerrno>  /* for errno */
#include <fcntl.h> /* for AT_FDCWD */

using namespace posix;

////////////////////////////////////////////////////////////////////////////////

mapped_file
mapped_file::open(const pathname& pathname,
                  const int flags,
                  const mode mode) {

  return mapped_file{AT_FDCWD, pathname.c_str(), flags, mode};
}

mapped_file
mapped_file::open(const directory& directory,
                  const pathname& pathname,
                  const int flags,
                  const mode mode) {

  return mapped_file{directory.fd(), pathname.c_str(), flags, mode};
}

////////////////////////////////////////////////////////////////////////////////

appendable_mapped_file
appendable_mapped_file::open(const pathname& pathname,
                             const int flags,
                             const mode mode) {

  return appendable_mapped_file{AT_FDCWD, pathname.c_str(), flags, mode};
}

appendable_mapped_file
appendable_mapped_file::open(const directory& directory,
                             const pathname& pathname,
                             const int flags,
                             const mode mode) {

  return appendable_mapped_file{directory.fd(), pathname.c_str(), flags, mode};
}

////////////////////////////////////////////////////////////////////////////////

std::size_t
appendable_mapped_file::append(const char* const data,
                               const std::size_t size) {

  const auto offset = seek(0, SEEK_END);

  write(data, size);

  return offset;
}
