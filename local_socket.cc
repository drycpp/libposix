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
#include <cstring>      /* for std::memset(), std::strlen() */
#include <fcntl.h>      /* for F_*, fcntl() */
#include <stdexcept>    /* for std::logic_error */
#include <sys/socket.h> /* for AF_LOCAL, CMSG_*, connect(), recvmsg(), sendmsg(), socket() */
#include <sys/un.h>     /* for struct sockaddr_un */

using namespace posix;

local_socket
local_socket::connect(const pathname& pathname) {
  assert(!pathname.empty());

  int sockfd;
  if ((sockfd = ::socket(AF_LOCAL, SOCK_STREAM, 0)) == -1) {
    switch (errno) {
      case EMFILE: /* Too many open files */
      case ENFILE: /* Too many open files in system */
      case ENOMEM: /* Cannot allocate memory in kernel */
        throw posix::fatal_error(errno);
      default:
        throw posix::error(errno);
    }
  }

  local_socket socket = local_socket(sockfd);

  struct sockaddr_un addr;
  addr.sun_family = AF_LOCAL;
  strcpy(addr.sun_path, pathname.c_str()); // TODO: check bounds

  const socklen_t addrlen = sizeof(addr.sun_family) + std::strlen(addr.sun_path);

retry:
  if (::connect(sockfd, reinterpret_cast<struct sockaddr*>(&addr), addrlen) == -1) {
    switch (errno) {
      case EINTR:  /* Interrupted system call */
        goto retry;
      default:
        throw posix::error(errno);
    }
  }

  return socket;
}

void
local_socket::send(const descriptor& descriptor) {
  (void)descriptor; // TODO
}

descriptor
local_socket::recvfd() {
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
      case ENOMEM: /* Cannot allocate memory in kernel */
        throw posix::fatal_error(errno);
      default:
        throw posix::error(errno);
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
  result.fcntl(F_SETFD, result.fcntl(F_GETFD) | FD_CLOEXEC);
#endif

  return result;
}
