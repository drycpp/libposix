/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_MESSAGE_QUEUE_H
#define POSIXXX_MESSAGE_QUEUE_H

#include "descriptor.h"

#include <cstddef> /* for std::size_t */

namespace posix {
  class message_queue;
}

class posix::message_queue : public posix::descriptor {
  public:
    /**
     * Constructor.
     */
    message_queue(const int fd) : descriptor(fd) {}
};

#endif /* POSIXXX_MESSAGE_QUEUE_H */
