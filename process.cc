/* This is free and unencumbered software released into the public domain. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "process.h"

#include <cassert>      /* for assert() */
#include <cerrno>       /* for errno */
#include <csignal>      /* for std::kill() */
#include <sys/types.h>  /* for pid_t */
#include <sys/wait.h>   /* for waitpid() */
#include <system_error> /* for std::system_error */

using namespace posix;

bool
process::alive() {
  int status;
  return _id ? !wait(status, WNOHANG) : false;
}

bool
process::wait(int& status, int options) {
  assert(_id != 0);

  for (;;) {
    const pid_t rc = waitpid(_id, &status, options);

    switch (rc) {
      case -1: /* an error condition occurred */
        switch (errno) {
          case EINTR: /* Interrupted system call */
            continue; /* try again */
          default:
            throw std::system_error(errno, std::system_category());
        }

      case 0:  /* the process hasn't changed state */
        return false;

      default: /* the process has changed state */
        return true;
    }
  }
}
