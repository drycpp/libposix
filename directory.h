/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_DIRECTORY_H
#define POSIXXX_DIRECTORY_H

#include "descriptor.h"

#include <string>  /* for std::string */
#include <utility> /* for std::move() */

namespace posix {
  class directory;
  class pathname;
}

/**
 * Represents a POSIX directory.
 *
 * @see http://pubs.opengroup.org/onlinepubs/009695399/basedefs/xbd_chap03.html#tag_03_128
 */
class posix::directory : public posix::descriptor {
  public:
    class iterator;

    static directory open(const pathname& pathname);

    static directory open(const directory& directory, const pathname& pathname);

    /**
     * Constructor.
     */
    directory(const int fd) : descriptor(fd) {}

    /**
     * Copy constructor.
     */
    directory(const directory& other) : descriptor(other) {}

    /**
     * Move constructor.
     */
    directory(directory&& other) : descriptor(std::move(other)) {}

    /**
     * Destructor.
     */
    ~directory() noexcept {}

    iterator begin() const;

    iterator end() const;

    const iterator cbegin() const;

    const iterator cend() const;

  protected:
    static directory open(int dirfd, const pathname& pathname);
};

class posix::directory::iterator {
  public:
    iterator() {}

    iterator(const directory& dir);

    ~iterator() noexcept;

    bool operator==(const iterator& other) {
      return !operator!=(other);
    }

    bool operator!=(const iterator& other);

    iterator& operator++();

    std::string operator*();

  protected:
    void* _dirp = nullptr;
};

#endif /* POSIXXX_DIRECTORY_H */
