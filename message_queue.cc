/* This is free and unencumbered software released into the public domain. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "posix++/message_queue.h"

#include <cassert>      /* for assert() */
#include <cerrno>       /* for errno */
#include <fcntl.h>      /* for O_* */
#include <mqueue.h>     /* for mq_*() */
#include <system_error> /* for std::system_error */
#include <unistd.h>     /* for lseek() */

using namespace posix;
