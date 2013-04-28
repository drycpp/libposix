/* This is free and unencumbered software released into the public domain. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "local_socket.h"

#include "pathname.h"

#include <cassert>      /* for assert() */
#include <cerrno>       /* for errno */
#include <cstring>      /* for std::strlen() */
#include <sys/socket.h> /* for AF_LOCAL, connect(), socket() */
#include <sys/un.h>     /* for struct sockaddr_un */
#include <system_error> /* for std::system_error */

using namespace posix;

local_socket
local_socket::connect(const pathname& pathname) {
  assert(!pathname.empty());

  int sockfd;
  if ((sockfd = socket(AF_LOCAL, SOCK_STREAM, 0)) == -1) {
    switch (errno) {
      case EMFILE: /* Too many open files */
      case ENFILE: /* Too many open files in system */
      case ENOMEM: /* Cannot allocate memory in kernel */
        throw std::system_error(errno, std::system_category()); // FIXME
      default:
        throw std::system_error(errno, std::system_category());
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
        throw std::system_error(errno, std::system_category());
    }
  }

  return socket;
}
