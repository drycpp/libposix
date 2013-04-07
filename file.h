/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_FILE_H
#define POSIXXX_FILE_H

#include "descriptor.h"

#include <cstddef> /* for std::size_t */

namespace posix {
  class directory;
  class pathname;

  using file_mode = unsigned int;
  class file;
}

/**
 * Represents a POSIX file.
 *
 * @see http://pubs.opengroup.org/onlinepubs/009695399/basedefs/xbd_chap03.html#tag_03_163
 */
class posix::file : public posix::descriptor {
  public:
    static file create(const pathname& pathname, file_mode mode);

    static file create(const directory& directory, const pathname& pathname, file_mode mode);

    static file open(const pathname& pathname, int flags);

    static file open(const directory& directory, const pathname& pathname, int flags);

    /**
     * Constructor.
     */
    file(const int fd) : descriptor(fd) {}

    /**
     * ...
     */
    std::size_t size() const;

  protected:
    static file open(int dirfd, const pathname& pathname, int flags, file_mode mode = 0);
};

#endif /* POSIXXX_FILE_H */
