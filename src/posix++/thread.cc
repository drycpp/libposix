/* This is free and unencumbered software released into the public domain. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "thread.h"

#ifdef __linux__
#include <sys/syscall.h> /* for SYS_gettid */
#include <unistd.h>      /* for syscall() */
#endif

using namespace posix;

pid_t
posix::gettid() noexcept {
#ifdef __linux__
  return syscall(SYS_gettid);
#else
  return 0; /* not supported */
#endif
}
