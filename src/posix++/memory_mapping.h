/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_MEMORY_MAPPING_H
#define POSIXXX_MEMORY_MAPPING_H

#include <cstddef> /* for std::size_t */
#include <cstdint> /* for std::uint8_t */

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
   *
   * @throws posix::error on failure
   */
  memory_mapping(const descriptor& descriptor);

  /**
   * Constructor.
   *
   * @throws posix::error on failure
   */
  memory_mapping(const descriptor& descriptor,
                 const std::size_t size,
                 const std::size_t offset = 0);

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
   * Returns the byte size of the mapping.
   */
  inline std::size_t size() const noexcept {
    return _size;
  }

  /**
   * Returns a pointer to the mapped memory.
   */
  template <typename T>
  inline const T* data(const std::size_t offset = 0) const noexcept {
    return reinterpret_cast<const T*>(_data + offset);
  }

  /**
   * Returns a pointer to the mapped memory.
   */
  template <typename T>
  inline T* data(const std::size_t offset = 0) noexcept {
    return reinterpret_cast<T*>(_data + offset);
  }

  /**
   * Returns a pointer to the mapped memory.
   */
  inline const std::uint8_t* data(const std::size_t offset = 0) const noexcept {
    return _data + offset;
  }

  /**
   * Returns a pointer to the mapped memory.
   */
  inline std::uint8_t* data(const std::size_t offset = 0) noexcept {
    return _data + offset;
  }

  /**
   * ...
   */
  explicit operator bool() const noexcept {
    return _data != nullptr;
  }

  /**
   * Returns the byte at the given offset.
   */
  inline std::uint8_t operator[](const std::size_t offset) const noexcept {
    return _data[offset];
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
  std::uint8_t* _data{nullptr};
  std::size_t _size{0};

  std::uint8_t* map(int fd, std::size_t size, std::size_t offset);
};

#endif /* POSIXXX_MEMORY_MAPPING_H */
