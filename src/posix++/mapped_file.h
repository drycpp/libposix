/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_MAPPED_FILE_H
#define POSIXXX_MAPPED_FILE_H

#ifndef __cplusplus
#error "<posix++/mapped_file.h> requires a C++ compiler"
#endif

////////////////////////////////////////////////////////////////////////////////

#include "file.h"
#include "memory_mapping.h"

#include <cstring> /* for std::strlen() */
#include <string>  /* for std::string */

namespace posix {
  class mapped_file;
  class appendable_mapped_file;
}

////////////////////////////////////////////////////////////////////////////////

/**
 * A memory-mapped file for random access.
 */
class posix::mapped_file : public posix::file {
protected:
  std::size_t _size;
  std::size_t _offset;
  memory_mapping _mapping;

protected:
  /**
   * Default constructor.
   */
  mapped_file() noexcept
    : file{}, _size{0}, _offset{0}, _mapping{nullptr, 0} {}

public:
  static mapped_file open(const pathname& pathname, int flags, mode mode = 0);

  static mapped_file open(const directory& directory, const pathname& pathname, int flags, mode mode = 0);

  /**
   * Constructor.
   */
  mapped_file(int dirfd, const char* pathname, int flags, mode mode = 0);

  /**
   * Move constructor.
   */
  mapped_file(mapped_file&& other) noexcept;

  /**
   * Checks whether this file's size is zero.
   */
  bool empty() const noexcept {
    return size() == 0;
  }

  /**
   * Returns the current file size in bytes.
   */
  std::size_t size() const noexcept {
    return _size;
  }

  /**
   * Returns the current file offset.
   */
  std::size_t offset() const noexcept {
    return _offset;
  }

  /**
   * Checks whether the current file offset is at or past EOF.
   */
  bool is_eof() const noexcept {
    return offset() >= size();
  }

  /**
   * Returns a pointer to the mapped memory.
   */
  template <typename T>
  T* data(const std::size_t offset = 0) noexcept {
    return _mapping.data<T>(offset);
  }

  /**
   * Returns a pointer to the mapped memory.
   */
  template <typename T>
  const T* data(const std::size_t offset = 0) const noexcept {
    return _mapping.data<T>(offset);
  }

  /**
   * Returns a pointer to the mapped memory.
   */
  std::uint8_t* data(const std::size_t offset = 0) noexcept {
    return _mapping.data(offset);
  }

  /**
   * Returns a pointer to the mapped memory.
   */
  const std::uint8_t* data(const std::size_t offset = 0) const noexcept {
    return _mapping.data(offset);
  }

  /**
   * Returns the byte at the given offset.
   */
  std::uint8_t operator[](const std::size_t offset) const noexcept {
    return _mapping[offset];
  }

  /**
   * Returns or changes the current file offset.
   */
  std::size_t seek(off_t offset, int whence = SEEK_SET);

  /**
   * Reads a line of text from this file.
   */
  std::size_t read_line(std::string& buffer);

  /**
   * Reads data from this file until the given separator character
   * is encountered.
   */
  std::size_t read_until(char separator, std::string& buffer);

  /**
   * Reads a character from this file.
   */
  std::size_t read(char& result);

  /**
   * Reads data from this file.
   */
  std::size_t read(void* buffer, std::size_t buffer_size);

  /**
   * Reads a string from this file.
   */
  std::string read();
};

////////////////////////////////////////////////////////////////////////////////

/**
 * A memory-mapped file for append-only writes.
 */
class posix::appendable_mapped_file : public posix::mapped_file {
public:
  static appendable_mapped_file open(const pathname& pathname, int flags, mode mode = 0);

  static appendable_mapped_file open(const directory& directory, const pathname& pathname, int flags, mode mode = 0);

  /**
   * Constructor.
   */
  using mapped_file::mapped_file;

  /**
   * @throws posix::runtime_error if an error occurs
   */
  template<class T>
  std::size_t append(const T& data) {
    const char* const c_str = data.c_str();
    return append(c_str, std::strlen(c_str));
  }

  /**
   * @throws posix::runtime_error if an error occurs
   */
  std::size_t append(const std::string& string) {
    return append(string.c_str(), string.size());
  }

  /**
   * @throws posix::runtime_error if an error occurs
   */
  std::size_t append(const char* data, std::size_t size);
};

////////////////////////////////////////////////////////////////////////////////

#endif /* POSIXXX_MAPPED_FILE_H */
