/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_MESSAGE_QUEUE_H
#define POSIXXX_MESSAGE_QUEUE_H

#ifndef __cplusplus
#error "<posix++/message_queue.h> requires a C++ compiler"
#endif

////////////////////////////////////////////////////////////////////////////////

#include "descriptor.h"
#include "mode.h"

#include <cstddef> /* for std::size_t */
#include <cstdint> /* for std::int64_t */
#include <string>  /* for std::string */
#include <utility> /* for std::move() */

namespace posix {
  class message_queue;
  struct message_queue_attr;
}

////////////////////////////////////////////////////////////////////////////////

/**
 * @see http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/mqueue.h.html
 */
struct posix::message_queue_attr {
  long flags;
  long maxmsg;
  long msgsize;
};

////////////////////////////////////////////////////////////////////////////////

/**
 * Represents a POSIX message queue.
 *
 * @note Assumes that message queue descriptors are actually file descriptors.
 *       This is the case on at least Linux and FreeBSD.
 * @see  http://pubs.opengroup.org/onlinepubs/009695399/basedefs/xbd_chap03.html#tag_03_224
 */
class posix::message_queue : public posix::descriptor {
  const std::size_t _message_size = 8192; // FIXME

public:
  static message_queue open(const std::string& queue_name,
    int flags, mode mode = 0);

  static message_queue open(const std::string& queue_name,
    int flags, mode mode, const message_queue_attr& attributes);

  /**
   * Removes a message queue.
   *
   * @param queue_name the name of the queue to remove
   * @pre `queue_name` must not be `nullptr`.
   */
  static void unlink(const char* queue_name);

  /**
   * Default constructor.
   */
  message_queue() = delete;

  /**
   * Constructor.
   */
  message_queue(const int fd) noexcept
    : descriptor(fd) {}

  /**
   * Copy constructor.
   */
  message_queue(const message_queue& other) /* may throw */
    : descriptor(other) {}

  /**
   * Move constructor.
   */
  message_queue(message_queue&& other) noexcept
    : descriptor(std::move(other)) {}

  /**
   * Destructor.
   */
  ~message_queue() noexcept;

  /**
   * Closes this message queue.
   *
   * @note this method is idempotent
   */
  void close() noexcept;

  /**
   * Receives a message from this message queue.
   *
   * @param message_data the message data
   * @param message_size the message size in bytes
   * @param message_priority the message priority (optionally)
   * @param recv_timeout the receive timeout in milliseconds, or -1 for a blocking receive
   * @pre `message_data` must not be `nullptr`.
   */
  std::size_t recv(void* message_data,
    std::size_t message_size,
    unsigned int* message_priority = nullptr,
    std::int64_t recv_timeout = -1);

  /**
   * Sends a message to this message queue.
   *
   * @param message_data the message data
   * @param message_size the message size in bytes
   * @param message_priority the message priority (defaults to zero)
   * @param send_timeout the send timeout in milliseconds, or -1 for a blocking send
   * @pre `message_data` must not be `nullptr`.
   */
  void send(const void* message_data,
    std::size_t message_size,
    unsigned int message_priority = 0,
    std::int64_t send_timeout = -1);

protected:
  static message_queue open(const char* name,
    int flags, mode mode, const message_queue_attr* attributes);
};

////////////////////////////////////////////////////////////////////////////////

#endif /* POSIXXX_MESSAGE_QUEUE_H */
