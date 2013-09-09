/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_GROUP_H
#define POSIXXX_GROUP_H

namespace posix {
  class group;
  using group_id = unsigned int;
}

/**
 * Represents a POSIX group.
 *
 * @see http://pubs.opengroup.org/onlinepubs/009695399/basedefs/xbd_chap03.html#tag_03_187
 */
class posix::group {
public:
  /**
   * Constructor.
   */
  group(const group_id id) : _id(id) {}

  /**
   * Returns the integer identifier for this group.
   */
  inline group_id id() const {
    return _id;
  }

protected:
  /**
   * @see http://pubs.opengroup.org/onlinepubs/009695399/basedefs/xbd_chap03.html#tag_03_188
   */
  group_id _id = 0;
};

#endif /* POSIXXX_GROUP_H */
