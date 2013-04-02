/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_PATHNAME_H
#define POSIXXX_PATHNAME_H

#include <string> /* for std::string */

namespace posix {
  class pathname;
}

class posix::pathname {
  public:
    /**
     * Constructor.
     */
    pathname(const std::string& path) : _path(path) {}

    /**
     * Constructor.
     */
    pathname(const char* path) : _path(path) {}

  protected:
    std::string _path;
};

#endif /* POSIXXX_PATHNAME_H */
