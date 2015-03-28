/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_GROUP_H
#define POSIXXX_GROUP_H

#ifndef __cplusplus
#error "<posix++/group.h> requires a C++ compiler"
#endif

////////////////////////////////////////////////////////////////////////////////

namespace posix {
  class group;
  using group_id = unsigned int;
}

////////////////////////////////////////////////////////////////////////////////

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
  group(const group_id id) noexcept
    : _id(id) {}

  /**
   * Returns the integer identifier for this group.
   *
   * @see http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap03.html#tag_03_189
   */
  inline group_id id() const noexcept {
    return _id;
  }

protected:
  /**
   * @see http://pubs.opengroup.org/onlinepubs/009695399/basedefs/xbd_chap03.html#tag_03_188
   */
  group_id _id = 0;
};


////////////////////////////////////////////////////////////////////////////////

#endif /* POSIXXX_GROUP_H */
