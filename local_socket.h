/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_LOCAL_SOCKET_H
#define POSIXXX_LOCAL_SOCKET_H

#include "descriptor.h"
#include "socket.h"

#include <utility> /* for std::move(), std::pair */

namespace posix {
  class local_socket;
  class pathname;
}

/**
 * Represents a POSIX local socket (aka Unix domain socket).
 *
 * @see http://en.wikipedia.org/wiki/Unix_domain_socket
 */
class posix::local_socket : public posix::socket {
  public:
    static std::pair<local_socket, local_socket> pair();

    static local_socket connect(const pathname& pathname);

    /**
     * Constructor.
     */
    local_socket(const int fd) noexcept
      : socket(fd) {}

    /**
     * Copy constructor.
     */
    local_socket(const local_socket& other) /* may throw */
      : socket(other) {}

    /**
     * Move constructor.
     */
    local_socket(local_socket&& other) noexcept
      : socket(std::move(other)) {}

    /**
     * Destructor.
     */
    ~local_socket() noexcept {}

    using socket::send;

    /**
     * Sends a descriptor to the peer.
     */
    void send(const descriptor& descriptor);

    /**
     * Receives a descriptor from the peer.
     */
    descriptor recv_descriptor();
};

#endif /* POSIXXX_LOCAL_SOCKET_H */
