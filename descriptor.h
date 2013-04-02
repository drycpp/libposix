/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_DESCRIPTOR_H
#define POSIXXX_DESCRIPTOR_H

#include <utility> /* for std::swap() */

namespace posix {
  struct descriptor;
}

/**
 * Represents a POSIX file descriptor.
 */
struct posix::descriptor {
  public:
    /**
     * Default constructor. The descriptor is invalid after construction.
     */
    descriptor() {}

    /**
     * Constructor.
     */
    descriptor(const int fd) {
      _fd = fd;
    }

    /**
     * Copy constructor.
     */
    descriptor(const descriptor& other);

    /**
     * Move constructor.
     */
    descriptor(descriptor&& other) : descriptor() {
      std::swap(_fd, other._fd);
    }

    /**
     * Destructor. Invokes `close()` if the descriptor is valid.
     */
    ~descriptor();

    /**
     * Copy assignment operator.
     */
    descriptor& operator=(descriptor other) {
      std::swap(_fd, other._fd);
      return *this;
    }

    /**
     * Move assignment operator.
     */
    descriptor& operator=(descriptor&& other) {
      if (this != &other) {
        close();
        std::swap(_fd, other._fd);
      }
      return *this;
    }

    bool operator==(const descriptor& other) const {
      return _fd == other._fd;
    }

    bool operator!=(const descriptor& other) const {
      return !operator==(other);
    }

    int operator*() const {
      return _fd;
    }

    explicit operator bool() const {
      return valid();
    }

    /**
     * Returns the underlying native integer descriptor.
     */
    inline int fd() const {
      return _fd;
    }

    /**
     * Checks whether this descriptor is valid.
     */
    inline bool valid() const {
      return _fd >= 0;
    }

    bool readable() const;

    bool writable() const;

    /**
     * Returns the file descriptor flags.
     */
    int flags() const;

    /**
     * Returns the file status flags.
     */
    int status() const;

    /**
     * Closes the descriptor.
     */
    void close();

  protected:
    int _fd = -1;
};

#endif /* POSIXXX_DESCRIPTOR_H */
