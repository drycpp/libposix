/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_PROCESS_GROUP_H
#define POSIXXX_PROCESS_GROUP_H

#ifndef __cplusplus
#error "<posix++/process_group.h> requires a C++ compiler"
#endif

////////////////////////////////////////////////////////////////////////////////

namespace posix {
  using process_group_id = unsigned int;
  class process_group;
}

/**
 * Represents a POSIX process group.
 *
 * @see http://pubs.opengroup.org/onlinepubs/009695399/basedefs/xbd_chap03.html#tag_03_290
 */
class posix::process_group {
public:
  /**
   * Constructor.
   */
  process_group(const process_group_id id) : _id(id) {}

  /**
   * Returns the integer identifier for this process group.
   */
  inline process_group_id id() const {
    return _id;
  }

protected:
  /**
   * The unique positive integer identifier representing a process group
   * during its lifetime.
   *
   * @see http://pubs.opengroup.org/onlinepubs/009695399/basedefs/xbd_chap03.html#tag_03_291
   */
  process_group_id _id = 0;
};

#endif /* POSIXXX_PROCESS_GROUP_H */
