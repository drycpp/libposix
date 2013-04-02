/* This is free and unencumbered software released into the public domain. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "posix++/file.h"

#include <cassert>      /* for assert() */
#include <cerrno>       /* for errno */
#include <sys/stat.h>   /* for fstat() */
#include <sys/types.h>  /* for struct stat */
#include <system_error> /* for std::system_error */
#include <unistd.h>     /* for lseek() */

using namespace posix;

std::size_t
file::size() const {
  struct stat st;

  if (fstat(_fd, &st) == -1) {
    switch (errno) {
      case ENOMEM: /* Out of memory in the kernel */
        throw std::system_error(errno, std::system_category()); // FIXME
      default:
        throw std::system_error(errno, std::system_category());
    }
  }

  return static_cast<std::size_t>(st.st_size);
}
