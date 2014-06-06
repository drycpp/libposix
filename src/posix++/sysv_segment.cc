/* This is free and unencumbered software released into the public domain. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "error.h"
#include "sysv_segment.h"

#include <cassert>      /* for assert() */
#include <cerrno>       /* for errno */
#include <cstdio>       /* for std::sprintf() */
#include <cstring>      /* for std::memset() */
#include <sys/types.h>  /* for key_t */
#include <sys/ipc.h>    /* for shm*() on BSD */
#include <sys/shm.h>    /* for struct shmid_ds, shm*() */

using namespace posix;

void
sysv_segment::for_each(std::function<void (sysv_segment segment)> callback) {
#ifdef __linux__
  struct shm_info shminfo;
  const int max_shmidx = shmctl(0, SHM_INFO, reinterpret_cast<struct shmid_ds*>(&shminfo));
  if (max_shmidx == -1) {
    throw_error("shmctl(SHM_INFO)");
  }

  for (int shmidx = 0; shmidx <= max_shmidx; shmidx++) {
    shmid_ds ds;
    const int shmid = shmctl(shmidx, SHM_STAT, &ds);
    if (shmid == -1) {
      switch (errno) {
        case EINVAL: /* Invalid argument */
        case EACCES: /* Permission denied */
          continue;
        default:
          assert(errno != EFAULT);
          throw_error("shmctl(SHM_STAT)");
      }
    }
    callback(sysv_segment(shmid));
  }
#else
  (void)callback; /* not used */
  throw_error(ENOSYS); /* Function not implemented */
#endif /* __linux__ */
}

sysv_segment
sysv_segment::create_unique(const std::size_t size,
                            const int flags) {
  return create(IPC_PRIVATE, size, IPC_EXCL | flags);
}

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
    throw_error("shmget");
  }
  return sysv_segment{shmid, nullptr, size};
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
    assert(errno != ENOSPC);
    throw_error("shmget");
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
  if (_id != -1) {
    _id = -1;
  }
}

std::size_t
sysv_segment::size() const {
  return _size ? _size : stat().shm_segsz;
}

std::size_t
sysv_segment::size() {
  return _size ? _size : (_size = stat().shm_segsz);
}

shmid_ds
sysv_segment::stat() const {
  shmid_ds ds;
  if (shmctl(_id, IPC_STAT, &ds) == -1) {
    assert(errno != EFAULT);
    throw_error("shmctl(IPC_STAT)");
  }
  return ds;
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
      throw_error("shmat");
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
      throw_error("shmdt");
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
  if (_id != -1) {
    if (shmctl(_id, IPC_RMID, nullptr) == -1) {
      assert(errno != EFAULT);
      if (errno != EINVAL && errno != EIDRM) {
#if 1
        char buffer[256];
        std::sprintf(buffer, "shmctl(%d, IPC_RMID, NULL)", _id);
        throw_error(buffer);
#else
        throw_error("shmctl(IPC_RMID)");
#endif
      }
    }
  }
}

void
sysv_segment::lock() {
#ifdef SHM_LOCK
  if (shmctl(_id, SHM_LOCK, nullptr) == -1) {
    assert(errno != EFAULT);
    throw_error("shmctl(SHM_LOCK)");
  }
#else
  throw_error(ENOSYS); /* Function not implemented */
#endif
}

void
sysv_segment::unlock() {
#ifdef SHM_UNLOCK
  if (shmctl(_id, SHM_UNLOCK, nullptr) == -1) {
    assert(errno != ENOMEM);
    assert(errno != EFAULT);
    throw_error("shmctl(SHM_UNLOCK)");
  }
#else
  throw_error(ENOSYS); /* Function not implemented */
#endif
}

void
sysv_segment::clear() noexcept {
  assert(is_attached());
  std::memset(data(), 0, size());
}
