/* This is free and unencumbered software released into the public domain. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "message_queue.h"

#include "error.h"

#include <cassert>      /* for assert() */
#include <cerrno>       /* for errno */
#include <fcntl.h>      /* for O_* */
#include <mqueue.h>     /* for mq_*() */

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
    attr.mq_curmsgs = attributes->curmsgs;
  }

  mqd_t mqd;
  if ((mqd = mq_open(name, flags, mode, attributes ? &attr : nullptr)) == -1) {
    switch (errno) {
      case EMFILE: /* Too many open files */
      case ENFILE: /* Too many open files in system */
      case ENOMEM: /* Cannot allocate memory in kernel */
        throw posix::error(errno); // FIXME
      default:
        throw posix::error(errno);
    }
  }

  return message_queue(mqd);
}

message_queue::~message_queue() noexcept {
  close();
}

void
message_queue::close() noexcept {
  if (valid()) {
    if (mq_close(static_cast<mqd_t>(_fd)) == -1) {
      /* Ignore any errors from mq_close(). */
    }
    _fd = -1;
  }
}
