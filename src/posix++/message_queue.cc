/* This is free and unencumbered software released into the public domain. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "error.h"
#include "message_queue.h"

#include <algorithm> /* for std::min() */
#include <cassert>   /* for assert() */
#include <cerrno>    /* for errno */
#include <cstring>   /* for std::memcpy() */
#include <ctime>     /* for std::time(), mq_timed*() */
#include <fcntl.h>   /* for O_* */
#include <mqueue.h>  /* for mq_*() */

using namespace posix;

static_assert(sizeof(mqd_t) <= sizeof(int),
  "sizeof(mqd_t) > sizeof(int)");

message_queue
message_queue::open(const std::string& name,
                    const int flags,
                    const mode mode) {
  assert(!name.empty());

  return open(name.c_str(), flags, mode, nullptr);
}

message_queue
message_queue::open(const std::string& name,
                    const int flags,
                    const mode mode,
                    const message_queue_attr& attributes) {
  assert(!name.empty());

  return open(name.c_str(), flags, mode, &attributes);
}

/**
 * @see http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_open.html
 */
message_queue
message_queue::open(const char* const name,
                    const int flags,
                    const mode mode,
                    const message_queue_attr* const attributes) {
  assert(name);

  mq_attr attr;
  if (attributes) {
    attr.mq_flags   = attributes->flags;
    attr.mq_maxmsg  = attributes->maxmsg;
    attr.mq_msgsize = attributes->msgsize;
  }

  mqd_t mqd;
  if ((mqd = mq_open(name, flags, mode, attributes ? &attr : nullptr)) == -1) {
    throw_error("mq_open", "\"%s\", 0x%x, 0%o, %s",
      name, static_cast<unsigned int>(flags), static_cast<unsigned int>(mode), "attr");
  }

  return message_queue(mqd);
}

/**
 * @see http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_unlink.html
 */
void
message_queue::unlink(const char* const queue_name) {
  assert(queue_name);

  if (mq_unlink(queue_name) == -1) {
    throw_error("mq_unlink", "\"%s\"", queue_name);
  }
}

message_queue::~message_queue() noexcept {
  close();
}

/**
 * @see http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_close.html
 */
void
message_queue::close() noexcept {
  if (valid()) {
    if (mq_close(static_cast<mqd_t>(_fd)) == -1) {
      /* Ignore any errors from mq_close(). */
    }
    _fd = -1;
  }
}

/**
 * @see http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_receive.html
 */
std::size_t
message_queue::recv(void* message_data,
                    const std::size_t message_size,
                    unsigned int* message_priority,
                    const std::int64_t recv_timeout) {
  assert(message_data);
  assert(recv_timeout >= -1);

  char buffer[_message_size];

  ssize_t rc;
  if (recv_timeout == -1) {
    rc = mq_receive(fd(), buffer, _message_size, message_priority);
  }
  else {
    const struct timespec timeout = {
      .tv_sec  = std::time(nullptr) + (recv_timeout / 1000),
      .tv_nsec = (recv_timeout % 1000) * 1000000,
    };
    rc = mq_timedreceive(fd(), buffer, _message_size, message_priority, &timeout);
  }

  if (rc == -1) {
    throw_error((recv_timeout == -1) ? "mq_receive" : "mq_timedreceive"); // TODO: annotate with more information
  }

  std::memcpy(message_data, buffer, std::min(message_size, _message_size));

  return static_cast<std::size_t>(rc);
}

/**
 * @see http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_send.html
 */
void
message_queue::send(const void* const message_data,
                    const std::size_t message_size,
                    const unsigned int message_priority,
                    const std::int64_t send_timeout) {
  assert(message_data);
  assert(send_timeout >= -1);

  int rc;
  if (send_timeout == -1) {
    rc = mq_send(fd(), reinterpret_cast<const char*>(message_data), message_size, message_priority);
  }
  else {
    const struct timespec timeout = {
      .tv_sec  = std::time(nullptr) + (send_timeout / 1000),
      .tv_nsec = (send_timeout % 1000) * 1000000,
    };
    rc = mq_timedsend(fd(), reinterpret_cast<const char*>(message_data), message_size, message_priority, &timeout);
  }

  if (rc == -1) {
    throw_error((send_timeout == -1) ? "mq_send" : "mq_timedsend"); // TODO: annotate with more information
  }
}
