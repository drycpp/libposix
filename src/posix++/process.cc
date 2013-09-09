/* This is free and unencumbered software released into the public domain. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "process.h"

#include "error.h"

#include <cassert>      /* for assert() */
#include <cerrno>       /* for errno */
#include <csignal>      /* for SIG*, kill() */
#include <sys/types.h>  /* for pid_t */
#include <sys/wait.h>   /* for waitpid() */

using namespace posix;

bool
process::alive() {
  int status;
  return _id ? !wait(status, WNOHANG) : false;
}

int
process::wait() {
  assert(_id > 0);

  int status = 0;
  pid_t rc;
  do {
    rc = waitpid(_id, &status, 0);
  } while (!rc || (rc == -1 && errno == EINTR));
  return status;
}

bool
process::wait(int& status, const int options) {
  assert(_id > 0);

  for (;;) {
    const pid_t rc = waitpid(_id, &status, options);

    switch (rc) {
      case -1: /* an error condition occurred */
        switch (errno) {
          case EINTR: /* Interrupted system call */
            continue; /* try again */
          default:
            throw posix::runtime_error(errno);
        }

      case 0:  /* the process hasn't changed state */
        return false;

      default: /* the process has changed state */
        return true;
    }
  }
}

void
process::signal(const int signum) {
  assert(_id > 0);
  assert(signum >= 0);

  if (kill(_id, signum) == -1) {
    switch (errno) {
      default:
        throw posix::runtime_error(errno);
    }
  }
}
