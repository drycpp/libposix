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
     * Returns the integer identifier for this process.
     */
    inline process_id id() const {
      return _id;
    }

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
