/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_FILE_H
#define POSIXXX_FILE_H

#include "descriptor.h"

namespace posix {
  class file;
}

class posix::file : public posix::descriptor {
  public:
    /**
     * Constructor.
     */
    file(const int fd) : posix::descriptor(fd) {}
};

#endif /* POSIXXX_FILE_H */
