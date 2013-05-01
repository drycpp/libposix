/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_LOCAL_SOCKET_H
#define POSIXXX_LOCAL_SOCKET_H

#include "descriptor.h"
#include "mode.h"

#include <cstddef> /* for std::size_t */
#include <string>  /* for std::string */
#include <utility> /* for std::move() */

namespace posix {
  class local_socket;
  class pathname;
}

/**
 * Represents a POSIX local socket (aka Unix domain socket).
 *
 * @see http://en.wikipedia.org/wiki/Unix_domain_socket
 */
class posix::local_socket : public posix::descriptor {
  public:
    static local_socket connect(const pathname& pathname);

    /**
     * Constructor.
     */
    local_socket(const int fd) noexcept
      : descriptor(fd) {}

    /**
     * Copy constructor.
     */
    local_socket(const local_socket& other) /* may throw */
      : descriptor(other) {}

    /**
     * Move constructor.
     */
    local_socket(local_socket&& other) noexcept
      : descriptor(std::move(other)) {}

    /**
     * Destructor.
     */
    ~local_socket() noexcept {}

    /**
     * Sends a string to the peer.
     */
    void send(const std::string& string);

    /**
     * Sends data to the peer.
     */
    void send(const void* data);

    /**
     * Sends data to the peer.
     */
    void send(const void* data, std::size_t size);

    /**
     * Sends a descriptor to the peer.
     */
    void send(const descriptor& descriptor);

    /**
     * Receives a descriptor from the peer.
     */
    descriptor recvfd();
};

#endif /* POSIXXX_LOCAL_SOCKET_H */
