/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_PROCESS_H
#define POSIXXX_PROCESS_H

#ifndef __cplusplus
#error "<posix++/process.h> requires a C++ compiler"
#endif

////////////////////////////////////////////////////////////////////////////////

#include "user.h"

namespace posix {
  class group;
  using process_id = unsigned int;
  class process;
  class user;

  /**
   * Returns the current process ID.
   *
   * @note This function always succeeds.
   * @see http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap03.html#tag_03_296
   */
  process current_process() noexcept;

  /**
   * Returns the parent process ID of the current process.
   *
   * @note This function always succeeds.
   * @see http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap03.html#tag_03_266
   */
  process parent_process() noexcept;
}

////////////////////////////////////////////////////////////////////////////////

/**
 * Represents a POSIX process.
 *
 * @see http://pubs.opengroup.org/onlinepubs/009695399/basedefs/xbd_chap03.html#tag_03_289
 */
class posix::process {
public:
  /**
   * Constructor.
   */
  process(const process_id id) noexcept
    : _id(id) {}

  /**
   * Assigns a new identifier value to this process.
   */
  process& assign(const process_id id) {
    _id = id;
    return *this;
  }

  /**
   * Returns the integer identifier for this process.
   */
  inline process_id id() const noexcept {
    return _id;
  }

  /**
   * Returns the real user ID of this process.
   *
   * @note This method always succeeds.
   * @see http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap03.html#tag_03_311
   */
  user uid() const noexcept;

  /**
   * Returns the effective user ID of this process.
   *
   * @note This method always succeeds.
   * @see http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap03.html#tag_03_142
   */
  user euid() const noexcept;

  /**
   * Returns the real group ID of this process.
   *
   * @note This method always succeeds.
   * @see http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap03.html#tag_03_308
   */
  group gid() const noexcept;

  /**
   * Returns the effective group ID of this process.
   *
   * @note This method always succeeds.
   * @see http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap03.html#tag_03_141
   */
  group egid() const noexcept;

  /**
   * Determines whether this is a privileged process.
   *
   * @note This method always succeeds.
   */
  bool is_privileged() const noexcept {
    return euid().is_root();
  }

  /**
   * Checks whether this process is still alive.
   */
  bool alive() noexcept;

  /**
   * Waits for this process to terminate.
   */
  int wait();

  /**
   * Waits for this process to change state.
   *
   * @retval true if the process has changed state
   * @retval false if the process has not changed state
   */
  bool wait(int& status, int options);

  /**
   * Sends a signal to this process.
   */
  void signal(int signum);

protected:
  /**
   * The unique positive integer identifier representing a process during
   * its lifetime.
   *
   * @see http://pubs.opengroup.org/onlinepubs/009695399/basedefs/xbd_chap03.html#tag_03_294
   */
  process_id _id = 0;
};

////////////////////////////////////////////////////////////////////////////////

#endif /* POSIXXX_PROCESS_H */
