/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_FILE_H
#define POSIXXX_FILE_H

#include "descriptor.h"

#include <cstddef> /* for std::size_t */

namespace posix {
  class file;
}

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
