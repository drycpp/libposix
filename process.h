/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_PROCESS_H
#define POSIXXX_PROCESS_H

namespace posix {
  using process_id = unsigned int;
  class process;
}

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
    process(const process_id id) : _id(id) {}

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
    inline process_id id() const {
      return _id;
    }

    /**
     * Checks whether this process is still alive.
     */
    bool alive();

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

#endif /* POSIXXX_PROCESS_H */
