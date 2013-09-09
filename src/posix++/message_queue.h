/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_MESSAGE_QUEUE_H
#define POSIXXX_MESSAGE_QUEUE_H

#include "descriptor.h"
#include "mode.h"

#include <cstddef> /* for std::size_t */
#include <string>  /* for std::string */
#include <utility> /* for std::move() */

namespace posix {
  class message_queue;
  struct message_queue_attr;
}

/**
 * @see http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/mqueue.h.html
 */
struct posix::message_queue_attr {
  long flags   = 0;
  long maxmsg  = 0;
  long msgsize = 0;
  long curmsgs = 0;
};

/**
 * Represents a POSIX message queue.
 *
 * @note Assume that message queue descriptors are actually file descriptors.
 *       This is the case on at least Linux and FreeBSD.
 * @see  http://pubs.opengroup.org/onlinepubs/009695399/basedefs/xbd_chap03.html#tag_03_224
 */
class posix::message_queue : public posix::descriptor {
public:
  static message_queue open(const std::string& name,
    int flags, mode mode = 0);

  static message_queue open(const std::string& name,
    int flags, mode mode, const message_queue_attr& attributes);

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

protected:
  static message_queue open(const char* name,
    int flags, mode mode, const message_queue_attr* attributes);
};

#endif /* POSIXXX_MESSAGE_QUEUE_H */
