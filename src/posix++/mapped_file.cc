/* This is free and unencumbered software released into the public domain. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "mapped_file.h"

using namespace posix;

////////////////////////////////////////////////////////////////////////////////

std::size_t
appendable_mapped_file::append(const char* const data,
                               const std::size_t size) {

  const auto offset = seek(0, SEEK_END);

  write(data, size);

  return offset;
}
