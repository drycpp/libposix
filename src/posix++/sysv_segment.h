/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_SYSV_SEGMENT_H
#define POSIXXX_SYSV_SEGMENT_H

namespace posix {
  class sysv_segment;
}

/**
 * Represents a System V shared memory segment.
 */
class posix::sysv_segment {
  public:
    /**
     * Constructor.
     */
    sysv_segment(const int id) : _id(id) {}

    /**
     * Returns the integer identifier for this sysv_segment.
     */
    inline int id() const {
      return _id;
    }

  protected:
    int _id = -1;
};

#endif /* POSIXXX_SYSV_SEGMENT_H */
