/* This is free and unencumbered software released into the public domain. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "message_queue.h"

#include <cassert>      /* for assert() */
#include <cerrno>       /* for errno */
#include <fcntl.h>      /* for O_* */
#include <mqueue.h>     /* for mq_*() */
#include <system_error> /* for std::system_error */

using namespace posix;

static_assert(sizeof(mqd_t) <= sizeof(int),
  "sizeof(mqd_t) > sizeof(int)");

message_queue
message_queue::open(const std::string& name, const int flags, const mode mode) {
  assert(!name.empty());

  mqd_t mqd;
  if ((mqd = mq_open(name.c_str(), flags, mode, nullptr)) == -1) {
    switch (errno) {
      case EMFILE: /* Too many open files */
      case ENFILE: /* Too many open files in system */
      case ENOMEM: /* Cannot allocate memory in kernel */
        throw std::system_error(errno, std::system_category()); // FIXME
      default:
        throw std::system_error(errno, std::system_category());
    }
  }

  return message_queue(mqd);
}
