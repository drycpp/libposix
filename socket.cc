/* This is free and unencumbered software released into the public domain. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "socket.h"

#include <cassert>      /* for assert() */
#include <cerrno>       /* for errno */
#include <cstdint>      /* for std::uint8_t */
#include <cstring>      /* for std::strlen() */
#include <sys/socket.h> /* for recv(), send() */
#include <system_error> /* for std::system_error */

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

  std::size_t sent = 0;
  while (sent < size) {
    const ssize_t rc = ::send(fd(), reinterpret_cast<const std::uint8_t*>(data) + sent, size - sent, 0);
    if (rc == -1) {
      switch (errno) {
        case EINTR:  /* Interrupted system call */
          continue;
        case ENOMEM: /* Cannot allocate memory in kernel */
          throw std::system_error(errno, std::system_category()); // FIXME
        default:
          throw std::system_error(errno, std::system_category());
      }
    }
    sent += rc;
  }
}
