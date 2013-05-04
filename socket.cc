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
#include <sys/socket.h> /* for getsockopt(), recv(), send(), shutdown() */

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
    switch (errno) {
      case EBADF:       /* Bad file descriptor */
        throw posix::bad_descriptor();
      case EFAULT:      /* Bad address */
      case EINVAL:      /* Invalid argument */
      case ENOPROTOOPT: /* Protocol not available */
      case ENOTSOCK:    /* Socket operation on non-socket */
      default:
        throw posix::error(errno);
    }
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
        case ENOMEM: /* Cannot allocate memory in kernel */
          throw posix::fatal_error(errno);
        case EBADF:  /* Bad file descriptor */
          throw posix::bad_descriptor();
        default:
          throw posix::error(errno);
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
          case EBADF:  /* Bad file descriptor */
            throw posix::bad_descriptor();
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
          case EBADF:  /* Bad file descriptor */
            throw posix::bad_descriptor();
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
    switch (errno) {
      case EBADF:    /* Bad file descriptor */
        throw posix::bad_descriptor();
      case EINVAL:   /* Invalid argument */
      case ENOTCONN: /* Transport endpoint is not connected */
      case ENOTSOCK: /* Socket operation on non-socket */
      default:
        throw posix::error(errno);
    }
  }
}
