/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_SEMAPHORE_H
#define POSIXXX_SEMAPHORE_H

#ifndef __cplusplus
#error "<posix++/semaphore.h> requires a C++ compiler"
#endif

////////////////////////////////////////////////////////////////////////////////

/**
 * @file
 *
 * @see http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap04.html#tag_04_16
 */

#include "error.h"

#include <semaphore.h> /* for sem_t, sem_*() */

namespace posix {
  class unnamed_semaphore;
}

////////////////////////////////////////////////////////////////////////////////

/**
 * An unnamed POSIX semaphore.
 *
 * @note Instances of this class are neither movable nor copyable.
 *
 * @see http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/semaphore.h.html
 */
class posix::unnamed_semaphore final {
  sem_t _state;

public:
  /**
   * Default constructor.
   */
  unnamed_semaphore() : unnamed_semaphore{0} {}

  /**
   * Constructor.
   *
   * @param value the initial state of this semaphore (0 or 1)
   * @param is_shared whether this semaphore is shared between processes
   *
   * @see http://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_init.html
   */
  unnamed_semaphore(unsigned int value,
                    bool is_shared = false) {
    if (sem_init(&_state, is_shared ? 1 : 0, value) == -1) {
      throw_error("sem_init");
    }
  }

  /**
   * Copy constructor.
   */
  unnamed_semaphore(const unnamed_semaphore& other) noexcept = delete;

  /**
   * Move constructor.
   */
  unnamed_semaphore(unnamed_semaphore&& other) noexcept = delete;

  /**
   * Destructor.
   */
  ~unnamed_semaphore() noexcept {
    try { destroy(); } catch (const posix::error&) {}
  }

  /**
   * Copy assignment operator.
   */
  unnamed_semaphore& operator=(const unnamed_semaphore& other) noexcept = delete;

  /**
   * Move assignment operator.
   */
  unnamed_semaphore& operator=(unnamed_semaphore&& other) noexcept = delete;

  /**
   * Returns the current value of this semaphore.
   *
   * @see http://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_getvalue.html
   */
  int value() const {
    int sval = 0;
    if (sem_getvalue(const_cast<sem_t*>(&_state), &sval) == -1) {
      throw_error("sem_getvalue");
    }
    return sval;
  }

  /**
   * Destroys this semaphore.
   *
   * @note This method is *not* idempotent.
   * @see http://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_destroy.html
   */
  void destroy() {
    if (sem_destroy(&_state) == -1) {
      throw_error("sem_destroy");
    }
  }

  /**
   * Locks this semaphore.
   *
   * @see http://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_wait.html
   */
  void wait() {
    for (;;) {
      if (sem_wait(&_state) == -1) {
        if (errno != EINTR) {
          throw_error("sem_wait");
        }
      }
      else {
        break;
      }
    }
  }

  /**
   * Attempts to lock this semaphore, without blocking.
   *
   * @retval true if the function succeeded in locking this semaphore
   * @retval false otherwise
   * @see http://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_trywait.html
   */
  bool try_wait() {
    for (;;) {
      if (sem_trywait(&_state) == -1) {
        if (errno == EAGAIN) {
          return false;
        }
        if (errno != EINTR) {
          throw_error("sem_trywait");
        }
      }
      else {
        return true;
      }
    }
  }

  /**
   * Unlocks this semaphore.
   *
   * @see http://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_post.html
   */
  void notify() {
    if (sem_post(&_state) == -1) {
      throw_error("sem_post");
    }
  }
};

////////////////////////////////////////////////////////////////////////////////

#endif /* POSIXXX_SEMAPHORE_H */
