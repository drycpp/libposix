/* This is free and unencumbered software released into the public domain. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "socket.h"

#include "error.h"

#include <cassert>      /* for assert() */
#include <cerrno>       /* for errno */
#include <cstdint>      /* for std::uint8_t */
#include <cstring>      /* for std::strlen() */
#include <sys/socket.h> /* for recv(), send() */

using namespace posix;

void
socket::send(const std::string& string) {
  return send(string.data(), string.size());
}

void
socket::send(const char* const data) {
  assert(data != nullptr);

  return send(data, std::strlen(data));
}

void
socket::send(const void* const data,
             const std::size_t size) {
  assert(data != nullptr);

  std::size_t pos = 0;
  while (pos < size) {
    const ssize_t rc = ::send(fd(), reinterpret_cast<const std::uint8_t*>(data) + pos, size - pos, 0);
    if (rc == -1) {
      switch (errno) {
        case EINTR:  /* Interrupted system call */
          continue;
        case ENOMEM: /* Cannot allocate memory in kernel */
          throw posix::fatal_error(errno);
        default:
          throw posix::error(errno);
      }
    }
    pos += rc;
  }
}
