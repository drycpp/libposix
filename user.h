/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_USER_H
#define POSIXXX_USER_H

namespace posix {
  typedef unsigned int user_id;
  class user;
}

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
    user(const user_id id) : _id(id) {}

    /**
     * Returns the integer identifier for this user.
     */
    inline user_id id() const {
      return _id;
    }

  protected:
    /**
     * @see http://pubs.opengroup.org/onlinepubs/009695399/basedefs/xbd_chap03.html#tag_03_425
     */
    user_id _id = 0;
};

#endif /* POSIXXX_USER_H */
