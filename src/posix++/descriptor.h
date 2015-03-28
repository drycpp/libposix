/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_DESCRIPTOR_H
#define POSIXXX_DESCRIPTOR_H

#ifndef __cplusplus
#error "<posix++/descriptor.h> requires a C++ compiler"
#endif

////////////////////////////////////////////////////////////////////////////////

#include "mode.h"

#include <cstddef> /* for std::size_t */
#include <set>     /* for std::set */
#include <string>  /* for std::string */
#include <utility> /* for std::swap() */

namespace posix {
  struct descriptor;
  class group;
  class user;
}

////////////////////////////////////////////////////////////////////////////////

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
  descriptor() noexcept {}

  /**
   * Constructor.
   */
  descriptor(const int fd) noexcept {
    _fd = fd;
  }

  /**
   * Copy constructor.
   */
  descriptor(const descriptor& other); /* may throw */

  /**
   * Move constructor.
   */
  descriptor(descriptor&& other) noexcept
    : descriptor{} {
    std::swap(_fd, other._fd);
  }

  /**
   * Copy assignment operator.
   */
  descriptor& operator=(descriptor other) noexcept {
    std::swap(_fd, other._fd);
    return *this;
  }

  /**
   * Move assignment operator.
   */
  descriptor& operator=(descriptor&& other) noexcept {
    if (this != &other) {
      close();
    }
    std::swap(_fd, other._fd);
    return *this;
  }

  /**
   * Destructor. Invokes `close()` if the descriptor is valid.
   */
  ~descriptor() noexcept;

  /**
   * Assigns a new value to this descriptor.
   */
  descriptor& assign(const int fd) noexcept {
    close();
    _fd = fd;
    return *this;
  }

  /**
   * Releases the ownership of the native integer descriptor and returns
   * it.
   *
   * @post This descriptor is in an invalid state.
   */
  int release() noexcept {
    const int fd = _fd;
    _fd = -1;
    return fd;
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
  inline int fd() const noexcept {
    return _fd;
  }

  /**
   * Checks whether this descriptor is valid.
   */
  inline bool valid() const noexcept {
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
   * Returns the state of the close-on-exec flag.
   */
  bool cloexec() const;

  /**
   * Sets or clears the close-on-exec flag.
   */
  void cloexec(bool state);

  /**
   * Duplicates this descriptor.
   *
   * @note Preserves the state of the close-on-exec flag in the copy.
   */
  descriptor dup() const;

  /**
   * ...
   */
  int fcntl(int cmd) const;

  /**
   * ...
   */
  int fcntl(int cmd, int arg);

  /**
   * ...
   */
  int fcntl(int cmd, void* arg);

  /**
   * ...
   */
  void chown(const user& user, const group& group);

  /**
   * ...
   */
  void chmod(const mode mode);

  /**
   * @see http://pubs.opengroup.org/onlinepubs/9699919799/functions/poll.html
   */
  bool poll(short events, short* revents = nullptr, int timeout = -1);

  /**
   * Writes a line to this descriptor.
   */
  inline void write_line(const std::string& string) {
    write_line(string.c_str());
  }

  /**
   * Writes a line to this descriptor.
   */
  void write_line(const char* data);

  /**
   * Writes a string to this descriptor.
   */
  inline void write(const std::string& string) {
    write(string.c_str(), string.size());
  }

  /**
   * Writes data to this descriptor.
   */
  void write(const char* data);

  /**
   * Writes a character to this descriptor.
   */
  void write(char c);

  /**
   * Writes data to this descriptor.
   *
   * Will either write all the given data, or throw an error.
   *
   * Retries the operation automatically in case an `EINTR`
   * (interrupted system call) error is encountered.
   */
  void write(const void* data, std::size_t size);

  /**
   * Reads lines of text from this descriptor until EOF.
   */
  std::size_t read_lines(std::set<std::string>& result) const;

  /**
   * Reads a line of text from this descriptor.
   */
  std::size_t read_line(std::string& buffer) const;

  /**
   * Reads data from this descriptor until the given separator character
   * is encountered.
   */
  std::size_t read_until(char separator, std::string& buffer) const;

  /**
   * Reads a character from this descriptor.
   */
  std::size_t read(char& result) const;

  /**
   * Reads data from this descriptor.
   */
  std::size_t read(void* buffer, std::size_t buffer_size) const;

  /**
   * Reads a string from this descriptor.
   */
  std::string read() const;

  /**
   * ...
   */
  void sync();

  /**
   * Closes this descriptor.
   *
   * @note this method is idempotent
   */
  void close() noexcept;

protected:
  /**
   * The underlying native integer descriptor.
   */
  int _fd = -1;
};

////////////////////////////////////////////////////////////////////////////////

#endif /* POSIXXX_DESCRIPTOR_H */
