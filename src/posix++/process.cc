/* This is free and unencumbered software released into the public domain. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "error.h"
#include "group.h"
#include "process.h"
#include "user.h"

#include <cassert>      /* for assert() */
#include <cerrno>       /* for errno */
#include <csignal>      /* for SIG*, kill() */
#include <sys/types.h>  /* for gid_t, pid_t, uid_t */
#include <sys/wait.h>   /* for waitpid() */
#include <unistd.h>     /* for get*gid(), get*pid(), get*uid() */

using namespace posix;

process
current_process() noexcept {
  return process(getpid());
}

process
parent_process() noexcept {
  return process(getppid());
}

user
process::uid() const noexcept {
  return user(getuid());
}

user
process::euid() const noexcept {
  return user(geteuid());
}

group
process::gid() const noexcept {
  return group(getgid());
}

group
process::egid() const noexcept {
  return group(getegid());
}

bool
process::alive() noexcept {
  int status;
  try {
    return _id ? !wait(status, WNOHANG) : false;
  }
  catch (const posix::runtime_error&) {
    return false; /* ECHILD */
  }
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
            throw_error("waitpid", "%u, %s, 0x%x",
              _id, "status", static_cast<unsigned int>(options));
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
    throw_error("kill", "%u, %d", _id, signum);
  }
}
