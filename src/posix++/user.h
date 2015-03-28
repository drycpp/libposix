/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_USER_H
#define POSIXXX_USER_H

#ifndef __cplusplus
#error "<posix++/user.h> requires a C++ compiler"
#endif

////////////////////////////////////////////////////////////////////////////////

namespace posix {
  class user;
  using user_id = unsigned int;
}

////////////////////////////////////////////////////////////////////////////////

/**
 * Represents a POSIX user.
 *
 * @see http://pubs.opengroup.org/onlinepubs/009695399/basedefs/xbd_chap03.html#tag_03_424
 */
class posix::user {
public:
  /**
   * Constructor.
   */
  user(const user_id id) noexcept
    : _id(id) {}

  /**
   * Returns the integer identifier for this user.
   *
   * @see http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap03.html#tag_03_430
   */
  inline user_id id() const noexcept {
    return _id;
  }

  /**
   * Determines whether this is the superuser (aka `root`).
   */
  inline bool is_root() const noexcept {
    return _id == 0;
  }

protected:
  /**
   * @see http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap03.html#tag_03_430
   */
  user_id _id = 0;
};

////////////////////////////////////////////////////////////////////////////////

#endif /* POSIXXX_USER_H */
