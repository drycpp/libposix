/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_THREAD_H
#define POSIXXX_THREAD_H

#ifndef __cplusplus
#error "<posix++/thread.h> requires a C++ compiler"
#endif

////////////////////////////////////////////////////////////////////////////////

#include <sys/types.h> /* for pid_t */

namespace posix {
  /**
   * @note This is Linux-specific and should not be used in portable programs.
   */
  pid_t gettid() noexcept;
}

////////////////////////////////////////////////////////////////////////////////

#endif /* POSIXXX_THREAD_H */
