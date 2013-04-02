/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_FILE_H
#define POSIXXX_FILE_H

#include "descriptor.h"

#include <cstddef> /* for std::size_t */

namespace posix {
  class file;
}

/**
 * Represents a POSIX file.
 *
 * @see http://pubs.opengroup.org/onlinepubs/009695399/basedefs/xbd_chap03.html#tag_03_163
 */
class posix::file : public posix::descriptor {
  public:
    /**
     * Constructor.
     */
    file(const int fd) : descriptor(fd) {}

    /**
     * ...
     */
    std::size_t size() const;
};

#endif /* POSIXXX_FILE_H */
