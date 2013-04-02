/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_DESCRIPTOR_H
#define POSIXXX_DESCRIPTOR_H

#include <utility> /* for std::swap() */

namespace posix { struct descriptor; }

/**
 * Represents a POSIX file descriptor.
 *
 * @see http://pubs.opengroup.org/onlinepubs/009695399/basedefs/xbd_chap03.html#tag_03_165
 * @see http://en.wikipedia.org/wiki/File_descriptor
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
      }
      std::swap(_fd, other._fd);
      return *this;
    }

    /**
     * Assigns a new value to this descriptor.
     */
    descriptor& assign(const int fd) {
      close();
      _fd = fd;
      return *this;
    }

    /**
     * ...
     */
    bool operator==(const descriptor& other) const {
      return _fd == other._fd;
    }

    /**
     * ...
     */
    bool operator!=(const descriptor& other) const {
      return !operator==(other);
    }

    /**
     * Returns the underlying native integer descriptor.
     */
    int operator*() const {
      return _fd;
    }

    /**
     * ...
     */
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

    /**
     * Checks whether this descriptor is readable.
     */
    bool readable() const;

    /**
     * Checks whether this descriptor is writable.
     */
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
     * Closes this descriptor.
     *
     * @note this method is idempotent
     */
    void close();

  protected:
    /**
     * The underlying native integer descriptor.
     */
    int _fd = -1;
};

#endif /* POSIXXX_DESCRIPTOR_H */
