/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_MEMORY_MAPPING_H
#define POSIXXX_MEMORY_MAPPING_H

#ifndef __cplusplus
#error "<posix++/memory_mapping.h> requires a C++ compiler"
#endif

////////////////////////////////////////////////////////////////////////////////

#include <cstddef> /* for std::size_t */
#include <cstdint> /* for std::uint8_t */

namespace posix {
  struct descriptor;
  class memory_mapping;
}


////////////////////////////////////////////////////////////////////////////////

/**
 * Represents a POSIX memory mapping.
 *
 * @see http://en.wikipedia.org/wiki/Memory-mapped_file
 */
class posix::memory_mapping {
protected:
  std::size_t _size;
  std::uint8_t* _data;

protected:
  std::uint8_t* map(int fd, std::size_t size, std::size_t offset);

public:
  /**
   * Default constructor.
   */
  memory_mapping() noexcept = delete;

  /**
   * Constructor.
   *
   * @throws posix::error on failure
   */
  memory_mapping(const descriptor& descriptor);

  /**
   * Constructor.
   *
   * @throws posix::error on failure
   */
  memory_mapping(const descriptor& descriptor, std::size_t size, std::size_t offset = 0);

  /**
   * Constructor.
   *
   * @throws posix::error on failure
   */
  memory_mapping(int fd);

  /**
   * Constructor.
   *
   * @throws posix::error on failure
   */
  memory_mapping(int fd, std::size_t size, std::size_t offset = 0);

  /**
   * Constructor.
   *
   * @throws posix::error on failure
   */
  memory_mapping(void* data, std::size_t size) noexcept;

  /**
   * Copy constructor.
   */
  memory_mapping(const memory_mapping& other) noexcept = delete;

  /**
   * Move constructor.
   */
  memory_mapping(memory_mapping&& other) noexcept = default;

  /**
   * Copy assignment operator.
   */
  memory_mapping& operator=(const memory_mapping& other) noexcept = delete;

  /**
   * Move assignment operator.
   */
  memory_mapping& operator=(memory_mapping&& other) noexcept = default;

  /**
   * Destructor.
   */
  ~memory_mapping() noexcept;

  /**
   * Expands or shrinks this mapping.
   *
   * @note Not available on all platforms.
   */
  void remap(std::size_t new_size, int flags = 0);

  /**
   * Returns the byte size of the mapping.
   */
  std::size_t size() const noexcept {
    return _size;
  }

  /**
   * Returns a pointer to the mapped memory.
   */
  template <typename T>
  T* data(const std::size_t offset = 0) noexcept {
    return reinterpret_cast<T*>(_data + offset);
  }

  /**
   * Returns a pointer to the mapped memory.
   */
  template <typename T>
  const T* data(const std::size_t offset = 0) const noexcept {
    return reinterpret_cast<const T*>(_data + offset);
  }

  /**
   * Returns a pointer to the mapped memory.
   */
  std::uint8_t* data(const std::size_t offset = 0) noexcept {
    return _data + offset;
  }

  /**
   * Returns a pointer to the mapped memory.
   */
  const std::uint8_t* data(const std::size_t offset = 0) const noexcept {
    return _data + offset;
  }

  /**
   * Returns the byte at the given offset.
   */
  std::uint8_t operator[](const std::size_t offset) const noexcept {
    return _data[offset];
  }

  /**
   * ...
   */
  explicit operator bool() const noexcept {
    return _data != nullptr;
  }

  /**
   * Checks whether this memory mapping is readable.
   */
  bool readable() const noexcept;

  /**
   * Checks whether this memory mapping is writable.
   */
  bool writable() const noexcept;

  /**
   * Checks whether this memory mapping is writable.
   */
  bool executable() const noexcept;
};

////////////////////////////////////////////////////////////////////////////////

#endif /* POSIXXX_MEMORY_MAPPING_H */
