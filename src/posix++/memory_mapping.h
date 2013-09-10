/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_MEMORY_MAPPING_H
#define POSIXXX_MEMORY_MAPPING_H

#include <cstddef> /* for std::size_t */

namespace posix {
  struct descriptor;
  class memory_mapping;
}

/**
 * Represents a POSIX memory mapping.
 *
 * @see http://en.wikipedia.org/wiki/Memory-mapped_file
 */
class posix::memory_mapping {
public:
  /**
   * Default constructor.
   */
  memory_mapping() noexcept = delete;

  /**
   * Constructor.
   */
  memory_mapping(const descriptor& descriptor);

  /**
   * Constructor.
   */
  memory_mapping(const descriptor& descriptor,
                 const std::size_t size,
                 const std::size_t offset = 0);

  /**
   * Constructor.
   */
  memory_mapping(int fd);

  /**
   * Constructor.
   */
  memory_mapping(int fd, std::size_t size, std::size_t offset = 0);

  /**
   * Constructor.
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
   * Destructor.
   */
  ~memory_mapping() noexcept;

  /**
   * Copy assignment operator.
   */
  memory_mapping& operator=(const memory_mapping& other) noexcept = delete;

  /**
   * Move assignment operator.
   */
  memory_mapping& operator=(memory_mapping&& other) noexcept = default;

  /**
   * Returns a pointer to the mapped memory.
   */
  inline const void* data() const noexcept {
    return _data;
  }

  /**
   * Returns a pointer to the mapped memory.
   */
  inline void* data() noexcept {
    return _data;
  }

  /**
   * Returns the byte size of the mapping.
   */
  inline std::size_t size() const noexcept {
    return _size;
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

protected:
  void* _data{nullptr};
  std::size_t _size{0};
};

#endif /* POSIXXX_MEMORY_MAPPING_H */
