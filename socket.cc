/* This is free and unencumbered software released into the public domain. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "socket.h"

#include "error.h"

#include <array>        /* for std::array */
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

std::size_t
socket::recv(std::function<bool (const void* chunk_data, std::size_t chunk_size)> callback) {
  assert(callback != nullptr);

  std::size_t byte_count = 0;

  std::array<std::uint8_t, 4096> buffer;
  for (;;) {
    const ssize_t rc = ::recv(fd(), buffer.data(), buffer.size(), 0);
    switch (rc) {
      case -1:
        switch (errno) {
          case EINTR: /* Interrupted system call */
            continue; /* try again */
          default:
            throw posix::error(errno);
        }

      case 0:
        goto exit; /* peer has performed an orderly shutdown */

      default:
        assert(rc > 0);
        const std::size_t chunk_size = static_cast<std::size_t>(rc);
        byte_count += chunk_size;
        if (!callback(buffer.data(), chunk_size)) {
          goto exit; /* caller has requested early exit */
        }
    }
  }

exit:
  return byte_count;
}

std::size_t
socket::recv(std::string& buffer) {
  return recv([&buffer](const void* const chunk_data, const std::size_t chunk_size) -> bool {
    buffer.append(reinterpret_cast<const char*>(chunk_data), chunk_size);
    return true; /* continue indefinitely */
  });
}

std::size_t
socket::recv(void* const buffer,
             const std::size_t buffer_size) {
  assert(buffer != nullptr);

  std::size_t byte_count = 0;

  while (byte_count < buffer_size) {
    const ssize_t rc = ::recv(fd(),
      reinterpret_cast<std::uint8_t*>(buffer) + byte_count,
      buffer_size - byte_count, 0);
    switch (rc) {
      case -1:
        switch (errno) {
          case EINTR: /* Interrupted system call */
            continue; /* try again */
          default:
            throw posix::error(errno);
        }

      case 0:
        goto exit; /* peer has performed an orderly shutdown */

      default:
        assert(rc > 0);
        const std::size_t chunk_size = static_cast<std::size_t>(rc);
        byte_count += chunk_size;
    }
  }

exit:
  return byte_count;
}
