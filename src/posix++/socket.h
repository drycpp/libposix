/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_SOCKET_H
#define POSIXXX_SOCKET_H

#ifndef __cplusplus
#error "<posix++/socket.h> requires a C++ compiler"
#endif

////////////////////////////////////////////////////////////////////////////////

#include "descriptor.h"

#include <cstddef>    /* for std::size_t */
#include <functional> /* for std::function */
#include <string>     /* for std::string */
#include <utility>    /* for std::move() */

namespace posix {
  class socket;
}


////////////////////////////////////////////////////////////////////////////////

/**
 * Represents a POSIX socket.
 *
 * @see http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/sys_socket.h.html
 * @see https://en.wikipedia.org/wiki/Berkeley_sockets
 */
class posix::socket : public posix::descriptor {
public:
  /**
   * Returns the socket domain.
   */
  int domain() const;

  /**
   * Returns the socket type.
   */
  int type() const;

  /**
   * Returns the socket protocol.
   */
  int protocol() const;

  /**
   * Returns and clears the pending socket error.
   */
  int error();

  /**
   * Returns socket options.
   */
  void getsockopt(int level, int optname, void* optval, int* optlen) const;

  /**
   * Listens for connections on this socket.
   */
  void listen(unsigned int backlog = 128);

  /**
   * Sends a string to the peer.
   */
  void send(const std::string& string);

  /**
   * Sends data to the peer.
   */
  void send(const char* data);

  /**
   * Sends data to the peer.
   */
  void send(const void* data, std::size_t size);

  /**
   * Receives a text chunk from the peer.
   */
  std::string recv_chunk();

  /**
   * Receives a string from the peer.
   */
  std::string recv_string();

  /**
   * Receives data from the peer, into the given string buffer.
   */
  std::size_t recv(std::string& buffer);

  /**
   * Receives data from the peer, chunk by chunk.
   */
  std::size_t recv(std::function<bool (const void* chunk_data, std::size_t chunk_size)> callback);

  /**
   * Receives data from the peer, into the given raw buffer.
   */
  std::size_t recv(void* buffer, std::size_t buffer_size, int flags = 0);

  /**
   * Closes this socket for writing.
   */
  void close_write();

  /**
   * Closes this socket for reading.
   */
  void close_read();

  /**
   * Shuts down part of a full-duplex connection.
   */
  void shutdown(int how);

protected:
  /**
   * Default constructor.
   */
  socket() noexcept
    : descriptor() {}

  /**
   * Constructor.
   */
  socket(const int fd) noexcept
    : descriptor(fd) {}

  /**
   * Copy constructor.
   */
  socket(const socket& other) /* may throw */
    : descriptor(other) {}

  /**
   * Move constructor.
   */
  socket(socket&& other) noexcept
    : descriptor(std::move(other)) {}

  /**
   * Destructor.
   */
  ~socket() noexcept {}
};

////////////////////////////////////////////////////////////////////////////////

#endif /* POSIXXX_SOCKET_H */
