/* This is free and unencumbered software released into the public domain. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "error.h"
#include "sysv_segment.h"

#include <cassert>      /* for assert() */
#include <cerrno>       /* for errno */
#include <sys/types.h>  /* for key_t */
#include <sys/ipc.h>    /* for shm*() on BSD */
#include <sys/shm.h>    /* for struct shmid_ds, shm*() */

using namespace posix;

/**
 * @see http://pubs.opengroup.org/onlinepubs/9699919799/functions/shmget.html
 * @see http://man7.org/linux/man-pages/man2/shmget.2.html
 * @see http://www.freebsd.org/cgi/man.cgi?query=shmget&sektion=2
 * @see http://developer.apple.com/library/mac/documentation/Darwin/Reference/ManPages/man2/shmget.2.html
 */
sysv_segment
sysv_segment::create(const key_t key,
                     const std::size_t size,
                     const int flags) {
  int shmid;
  if ((shmid = shmget(key, size, IPC_CREAT | flags)) == -1) {
    switch (errno) {
      case EINVAL: /* Invalid argument */
        throw posix::invalid_argument();
      case ENOMEM: /* Cannot allocate memory in kernel */
      case ENOSPC: /* No space left on device */
        throw posix::fatal_error(errno);
      default:
        throw posix::runtime_error(errno);
    }
  }
  return sysv_segment(shmid);
}

/**
 * @see http://pubs.opengroup.org/onlinepubs/9699919799/functions/shmget.html
 * @see http://man7.org/linux/man-pages/man2/shmget.2.html
 * @see http://www.freebsd.org/cgi/man.cgi?query=shmget&sektion=2
 * @see http://developer.apple.com/library/mac/documentation/Darwin/Reference/ManPages/man2/shmget.2.html
 */
sysv_segment
sysv_segment::open(const key_t key) {
  int shmid;
  if ((shmid = shmget(key, 0, 0)) == -1) {
    switch (errno) {
      case EINVAL: /* Invalid argument */
        throw posix::invalid_argument();
      case ENOMEM: /* Cannot allocate memory in kernel */
        throw posix::fatal_error(errno);
      default:
        assert(errno != ENOSPC);
        throw posix::runtime_error(errno);
    }
  }
  return sysv_segment(shmid);
}

sysv_segment::~sysv_segment() noexcept {
  if (_addr) {
    if (shmdt(_addr) == -1) {
      /* Ignore any errors from shmdt(). */
    }
    _addr = nullptr;
  }
  if (_id) {
#if 0 // TODO: think this through.
    if (shmctl(_id, IPC_RMID, nullptr) == -1) {
      /* Ignore any errors from shmctl(). */
    }
#endif
    _id = -1;
  }
}

/**
 * @see http://pubs.opengroup.org/onlinepubs/9699919799/functions/shmat.html
 * @see http://man7.org/linux/man-pages/man2/shmat.2.html
 * @see http://www.freebsd.org/cgi/man.cgi?query=shmat&sektion=2
 * @see http://developer.apple.com/library/mac/documentation/Darwin/Reference/ManPages/man2/shmat.2.html
 */
void*
sysv_segment::attach(const int flags) {
  if (!_addr) {
    if ((_addr = shmat(_id, nullptr, flags)) == reinterpret_cast<void*>(-1)) {
      switch (errno) {
        case EINVAL: /* Invalid argument */
          throw posix::invalid_argument();
        case EMFILE: /* Too many open segments */
        case ENOMEM: /* Cannot allocate memory in kernel */
          throw posix::fatal_error(errno);
        default:
          throw posix::runtime_error(errno);
      }
    }
    assert(_addr != nullptr);
  }
  return _addr;
}

/**
 * @see http://pubs.opengroup.org/onlinepubs/9699919799/functions/shmdt.html
 * @see http://man7.org/linux/man-pages/man2/shmdt.2.html
 * @see http://www.freebsd.org/cgi/man.cgi?query=shmdt&sektion=2
 * @see http://developer.apple.com/library/mac/documentation/Darwin/Reference/ManPages/man2/shmdt.2.html
 */
void
sysv_segment::detach() {
  if (_addr) {
    if (shmdt(_addr) == -1) {
      switch (errno) {
        case EINVAL: /* Invalid argument */
          throw posix::invalid_argument();
        default:
          throw posix::runtime_error(errno);
      }
    }
    _addr = nullptr;
  }
}

/**
 * @see http://pubs.opengroup.org/onlinepubs/9699919799/functions/shmctl.html
 * @see http://man7.org/linux/man-pages/man2/shmctl.2.html
 * @see http://www.freebsd.org/cgi/man.cgi?query=shmctl&sektion=2
 * @see http://developer.apple.com/library/mac/documentation/Darwin/Reference/ManPages/man2/shmctl.2.html
 */
void
sysv_segment::remove() {
  if (_id) {
    if (shmctl(_id, IPC_RMID, nullptr) == -1) {
      switch (errno) {
        case EINVAL: /* Invalid argument */
          throw posix::invalid_argument();
        default:
          assert(errno != EFAULT);
          throw posix::runtime_error(errno);
      }
    }
    _id = -1;
  }
}

void
sysv_segment::lock() {
#ifdef SHM_LOCK
  if (shmctl(_id, SHM_LOCK, nullptr) == -1) {
    switch (errno) {
      case EINVAL: /* Invalid argument */
        throw posix::invalid_argument();
      case ENOMEM: /* Cannot allocate memory in kernel */
        throw posix::fatal_error(errno);
      default:
        assert(errno != EFAULT);
        throw posix::runtime_error(errno);
    }
  }
#else
  throw posix::runtime_error(ENOSYS); /* Not implemented */
#endif
}

void
sysv_segment::unlock() {
#ifdef SHM_UNLOCK
  if (shmctl(_id, SHM_UNLOCK, nullptr) == -1) {
    switch (errno) {
      case EINVAL: /* Invalid argument */
        throw posix::invalid_argument();
      default:
        assert(errno != ENOMEM);
        assert(errno != EFAULT);
        throw posix::runtime_error(errno);
    }
  }
#else
  throw posix::runtime_error(ENOSYS); /* Not implemented */
#endif
}
