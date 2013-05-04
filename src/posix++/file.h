/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_FILE_H
#define POSIXXX_FILE_H

#include "descriptor.h"
#include "mode.h"

#include <cstddef> /* for std::size_t */
#include <utility> /* for std::move() */

namespace posix {
  class directory;
  class file;
  class pathname;
}

/**
 * Represents a POSIX file.
 *
 * @see http://pubs.opengroup.org/onlinepubs/009695399/basedefs/xbd_chap03.html#tag_03_163
 */
class posix::file : public posix::descriptor {
  public:
    static file create(const pathname& pathname, mode mode);

    static file create(const directory& directory, const pathname& pathname, mode mode);

    static file open(const pathname& pathname, int flags);

    static file open(const directory& directory, const pathname& pathname, int flags);

    /**
     * Constructor.
     */
    file(const int fd) noexcept
      : descriptor(fd) {}

    /**
     * Copy constructor.
     */
    file(const file& other) /* may throw */
      : descriptor(other) {}

    /**
     * Move constructor.
     */
    file(file&& other) noexcept
      : descriptor(std::move(other)) {}

    /**
     * Destructor.
     */
    ~file() noexcept {}

    /**
     * ...
     */
    std::size_t size() const;

  protected:
    static file open(int dirfd, const pathname& pathname, int flags, mode mode = 0);
};

#endif /* POSIXXX_FILE_H */
