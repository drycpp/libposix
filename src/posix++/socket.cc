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
#include <sys/socket.h> /* for getsockopt(), listen(), recv(), send(), shutdown() */

using namespace posix;

int
socket::domain() const {
  int optval = 0;
#ifdef SO_DOMAIN
  int optlen = sizeof(optval);
  getsockopt(SOL_SOCKET, SO_DOMAIN, &optval, &optlen);
#endif
  return optval;
}

int
socket::type() const {
  int optval = 0;
#ifdef SO_TYPE
  int optlen = sizeof(optval);
  getsockopt(SOL_SOCKET, SO_TYPE, &optval, &optlen);
#endif
  return optval;
}

int
socket::protocol() const {
  int optval = 0;
#ifdef SO_PROTOCOL
  int optlen = sizeof(optval);
  getsockopt(SOL_SOCKET, SO_PROTOCOL, &optval, &optlen);
#endif
  return optval;
}

int
socket::error() {
  int optval = 0;
#ifdef SO_ERROR
  int optlen = sizeof(optval);
  getsockopt(SOL_SOCKET, SO_ERROR, &optval, &optlen);
#endif
  return optval;
}

void
socket::getsockopt(const int level,
                   const int optname,
                   void* const optval,
                   int* const optlen) const {
  static_assert(sizeof(socklen_t) == sizeof(int), "sizeof(socklen_t) != sizeof(int)");
  if (::getsockopt(fd(), level, optname, optval, reinterpret_cast<socklen_t*>(optlen)) == -1) {
    throw_error("getsockopt", "%d, %d, %d, %s, %s", fd(), level, optname, "optval", "optlen");
  }
}

void
socket::listen(const unsigned int backlog) {
  if (::listen(fd(), static_cast<int>(backlog)) == -1) {
    throw_error("listen", "%d, %d", fd(), static_cast<int>(backlog));
  }
}

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
        default:
          throw_error("send", "%d, %s, %zu, 0x%x", fd(), "chunk", size - pos, 0U);
      }
    }
    pos += rc;
  }
}

std::string
socket::recv_chunk() {
  std::string buffer;
  recv([&buffer](const void* const chunk_data, const std::size_t chunk_size) -> bool {
    buffer.append(reinterpret_cast<const char*>(chunk_data), chunk_size);
    return false; /* all done */
  });
  return buffer;
}

std::string
socket::recv_string() {
  std::string buffer;
  recv(buffer);
  return buffer;
}

std::size_t
socket::recv(std::string& buffer) {
  return recv([&buffer](const void* const chunk_data, const std::size_t chunk_size) -> bool {
    buffer.append(reinterpret_cast<const char*>(chunk_data), chunk_size);
    return true; /* continue indefinitely */
  });
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
            throw_error("recv", "%d, %s, %zu, 0x%x", fd(), "chunk", buffer.size(), 0U);
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
socket::recv(void* const buffer,
             const std::size_t buffer_size,
             const int flags) {
  assert(buffer != nullptr);

  std::size_t byte_count = 0;

  while (byte_count < buffer_size) {
    const ssize_t rc = ::recv(fd(),
      reinterpret_cast<std::uint8_t*>(buffer) + byte_count,
      buffer_size - byte_count, flags);
    switch (rc) {
      case -1:
        switch (errno) {
          case EINTR: /* Interrupted system call */
            continue; /* try again */
          default:
            throw_error("recv", "%d, %s, %zu, 0x%x",
              fd(), "chunk", buffer_size - byte_count, static_cast<unsigned int>(flags));
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

void
socket::close_write() {
  shutdown(SHUT_WR);
}

void
socket::close_read() {
  shutdown(SHUT_RD);
}

void
socket::shutdown(const int how) {
  if (::shutdown(fd(), how) == -1) {
    throw_error("shutdown", "%d, %d", fd(), how);
  }
}
