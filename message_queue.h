/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_MESSAGE_QUEUE_H
#define POSIXXX_MESSAGE_QUEUE_H

#include "descriptor.h"
#include "mode.h"

#include <cstddef> /* for std::size_t */
#include <string>  /* for std::string */
#include <utility> /* for std::move() */

namespace posix { class message_queue; }

/**
 * Represents a POSIX message queue.
 *
 * @note Assume that message queue descriptors are actually file descriptors.
 *       This is the case on at least Linux and FreeBSD.
 * @see  http://pubs.opengroup.org/onlinepubs/009695399/basedefs/xbd_chap03.html#tag_03_224
 */
class posix::message_queue : public posix::descriptor {
  public:
    static message_queue open(const std::string& name, int flags, mode mode = 0);

    /**
     * Constructor.
     */
    message_queue(const int fd) : descriptor(fd) {}

    /**
     * Copy constructor.
     */
    message_queue(const message_queue& other) : descriptor(other) {}

    /**
     * Move constructor.
     */
    message_queue(message_queue&& other) : descriptor(std::move(other)) {}

    /**
     * Destructor.
     */
    ~message_queue() {}
};

#endif /* POSIXXX_MESSAGE_QUEUE_H */
