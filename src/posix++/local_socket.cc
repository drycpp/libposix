/* This is free and unencumbered software released into the public domain. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "local_socket.h"

#include "error.h"
#include "pathname.h"

#include <array>        /* for std::array */
#include <cassert>      /* for assert() */
#include <cerrno>       /* for errno */
#include <cstdint>      /* for std::uint8_t */
#include <cstring>      /* for std::memset(), std::strcpy(), std::strlen() */
#include <fcntl.h>      /* for F_*, fcntl() */
#include <stdexcept>    /* for std::length_error, std::logic_error */
#include <sys/socket.h> /* for AF_LOCAL, CMSG_*, accept(), connect(), recvmsg(), sendmsg(), socket(), socketpair() */
#include <sys/un.h>     /* for struct sockaddr_un */

using namespace posix;

local_socket::local_socket() : socket() {
  int flags = 0;
#ifdef SOCK_CLOEXEC
  /* Nonstandard Linux extension to set O_CLOEXEC atomically: */
  flags |= SOCK_CLOEXEC;
#endif

  int sockfd;
  if ((sockfd = ::socket(AF_LOCAL, SOCK_STREAM | flags, 0)) == -1) {
    throw_error("socket(AF_LOCAL, SOCK_STREAM)");
  }

  assign(sockfd);
#ifndef SOCK_CLOEXEC
  cloexec(true);
#endif
}

std::pair<local_socket, local_socket>
local_socket::pair() {
  int flags = 0;
#ifdef SOCK_CLOEXEC
  /* Nonstandard Linux extension to set O_CLOEXEC atomically: */
  flags |= SOCK_CLOEXEC;
#endif

  int fds[2] = {0, 0};
  if (::socketpair(AF_LOCAL, SOCK_STREAM | flags, 0, fds) == -1) {
    throw_error("socketpair(AF_LOCAL, SOCK_STREAM)");
  }

  std::pair<local_socket, local_socket> pair{local_socket(fds[0]), local_socket(fds[1])};
#ifndef SOCK_CLOEXEC
  pair.first.cloexec(true);
  pair.second.cloexec(true);
#endif

  return pair;
}

local_socket
local_socket::bind(const pathname& pathname) {
  assert(!pathname.empty());

  local_socket socket = local_socket();

  struct sockaddr_un addr;
  addr.sun_family = AF_LOCAL;

  if (pathname.size() >= sizeof(addr.sun_path)) {
    throw std::length_error{"socket pathname exceeds maximum length"};
  }
  std::strncpy(addr.sun_path, pathname.c_str(), sizeof(addr.sun_path) - 1);
  addr.sun_path[sizeof(addr.sun_path) - 1] = '\0';

  const socklen_t addrlen = sizeof(addr.sun_family) + std::strlen(addr.sun_path);

retry:
  if (::bind(socket.fd(), reinterpret_cast<struct sockaddr*>(&addr), addrlen) == -1) {
    switch (errno) {
      case EINTR:  /* Interrupted system call */
        goto retry;
      default:
        throw_error("bind");
    }
  }

  return socket;
}

local_socket
local_socket::connect(const pathname& pathname) {
  assert(!pathname.empty());

  local_socket socket = local_socket();

  struct sockaddr_un addr;
  addr.sun_family = AF_LOCAL;

  if (pathname.size() >= sizeof(addr.sun_path)) {
    throw std::length_error{"socket pathname exceeds maximum length"};
  }
  std::strncpy(addr.sun_path, pathname.c_str(), sizeof(addr.sun_path) - 1);
  addr.sun_path[sizeof(addr.sun_path) - 1] = '\0';

  const socklen_t addrlen = sizeof(addr.sun_family) + std::strlen(addr.sun_path);

retry:
  if (::connect(socket.fd(), reinterpret_cast<struct sockaddr*>(&addr), addrlen) == -1) {
    switch (errno) {
      case EINTR:  /* Interrupted system call */
        goto retry;
      default:
        throw_error("connect");
    }
  }

  return socket;
}

local_socket
local_socket::accept() {
  int connfd;
retry:
#if defined(HAVE_ACCEPT4) && defined(SOCK_CLOEXEC)
  /* Nonstandard Linux extension to set O_CLOEXEC atomically: */
  if ((connfd = ::accept4(fd(), nullptr, nullptr, SOCK_CLOEXEC)) == -1) {
#else
  if ((connfd = ::accept(fd(), nullptr, nullptr)) == -1) {
#endif
    switch (errno) {
      case EINTR:   /* Interrupted system call */
        goto retry;
      default:
        throw_error("accept");
    }
  }

  local_socket connection(connfd);
#if !(defined(HAVE_ACCEPT4) && defined(SOCK_CLOEXEC))
  connection.cloexec(true);
#endif

  return connection;
}

void
local_socket::send_descriptor(const descriptor& descriptor) {
#ifdef __linux__
  /* The control buffer must be large enough to hold a single frame of
   * cmsghdr ancillary data: */
  std::array<std::uint8_t, CMSG_SPACE(sizeof(int))> cmsg_buffer;
  static_assert(sizeof(struct cmsghdr) <= cmsg_buffer.size(),
    "sizeof(struct cmsghdr) > cmsg_buffer.size()");
  static_assert(CMSG_SPACE(sizeof(int)) <= cmsg_buffer.size(),
    "CMSG_SPACE(sizeof(int)) > cmsg_buffer.size()");
  static_assert(CMSG_LEN(sizeof(int)) <= cmsg_buffer.size(),
    "CMSG_LEN(sizeof(int)) > cmsg_buffer.size()");

  /* Linux and Solaris require there to be at least 1 byte of actual data: */
  std::array<std::uint8_t, 1> data_buffer = {{'\0'}};

  struct iovec iov;
  std::memset(&iov, 0, sizeof(iov));
  iov.iov_base = data_buffer.data();
  iov.iov_len  = data_buffer.size();

  struct msghdr msg;
  std::memset(&msg, 0, sizeof(msghdr));
  msg.msg_iov = &iov;
  msg.msg_iovlen = 1;
  msg.msg_control = cmsg_buffer.data();
  msg.msg_controllen = cmsg_buffer.size();

  struct cmsghdr* const cmsg = CMSG_FIRSTHDR(&msg);
  cmsg->cmsg_len   = CMSG_LEN(sizeof(int));
  cmsg->cmsg_level = SOL_SOCKET;
  cmsg->cmsg_type  = SCM_RIGHTS;
  *reinterpret_cast<int*>(CMSG_DATA(cmsg)) = descriptor.fd();

retry:
  const ssize_t rc = ::sendmsg(fd(), &msg, 0);
  if (rc == -1) {
    switch (errno) {
      case EINTR:  /* Interrupted system call */
        goto retry;
      default:
        throw_error("sendmsg");
    }
  }
#else /* __linux__ */
  (void)descriptor; /* not used */
  throw_error(ENOSYS); /* Function not implemented */
#endif /* __linux__ */
}

descriptor
local_socket::recv_descriptor() {
#ifdef __linux__
  descriptor result;

  /* The control buffer must be large enough to hold a single frame of
   * cmsghdr ancillary data: */
  std::array<std::uint8_t, CMSG_SPACE(sizeof(int))> cmsg_buffer;
  static_assert(sizeof(struct cmsghdr) <= cmsg_buffer.size(),
    "sizeof(struct cmsghdr) > cmsg_buffer.size()");
  static_assert(CMSG_SPACE(sizeof(int)) <= cmsg_buffer.size(),
    "CMSG_SPACE(sizeof(int)) > cmsg_buffer.size()");
  static_assert(CMSG_LEN(sizeof(int)) <= cmsg_buffer.size(),
    "CMSG_LEN(sizeof(int)) > cmsg_buffer.size()");

  /* Linux and Solaris require there to be at least 1 byte of actual data: */
  std::array<std::uint8_t, 1> data_buffer;

  struct iovec iov;
  std::memset(&iov, 0, sizeof(iov));
  iov.iov_base = data_buffer.data();
  iov.iov_len  = data_buffer.size();

  struct msghdr msg;
  std::memset(&msg, 0, sizeof(msghdr));
  msg.msg_iov = &iov;
  msg.msg_iovlen = 1;
  msg.msg_control = cmsg_buffer.data();
  msg.msg_controllen = cmsg_buffer.size();

  int flags = 0;
#ifdef MSG_CMSG_CLOEXEC
  flags |= MSG_CMSG_CLOEXEC; /* Linux only */
#endif

retry:
  const ssize_t rc = ::recvmsg(fd(), &msg, flags);
  if (rc == -1) {
    switch (errno) {
      case EINTR:  /* Interrupted system call */
        goto retry;
      default:
        throw_error("recvmsg");
    }
  }

  const struct cmsghdr* const cmsg = CMSG_FIRSTHDR(&msg);
  if (cmsg && cmsg->cmsg_len == CMSG_LEN(sizeof(int))) {
    if (cmsg->cmsg_level != SOL_SOCKET) {
      throw std::logic_error("invalid cmsg_level");
    }
    if (cmsg->cmsg_type != SCM_RIGHTS) {
      throw std::logic_error("invalid cmsg_type");
    }
    result.assign(*reinterpret_cast<const int*>(CMSG_DATA(cmsg)));
  }

#ifndef MSG_CMSG_CLOEXEC
  result.cloexec(true);
#endif

  return result;
#else /* __linux__ */
  throw_error(ENOSYS); /* Function not implemented */
#endif /* __linux__ */
}
