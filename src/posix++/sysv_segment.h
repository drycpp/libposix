/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_SYSV_SEGMENT_H
#define POSIXXX_SYSV_SEGMENT_H

#include <cstddef>    /* for std::size_t */
#include <cstdint>    /* for std::uint8_t */
#include <functional> /* for std::function */
#include <sys/ipc.h>  /* for key_t */
#include <sys/shm.h>  /* for shmid_ds */

namespace posix {
  class sysv_segment;
}

/**
 * Represents a System V shared memory segment.
 *
 * @note Instances of this class are movable, but not copyable.
 * @see http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap03.html#tag_03_342
 */
class posix::sysv_segment {
public:
  /**
   * Applies a function to every existing segment in the system.
   *
   * @throws posix::error on failure
   * @note This operation is Linux-specific.
   */
  static void for_each(std::function<void (sysv_segment segment)> callback);

  /**
   * Creates a new segment.
   *
   * @throws posix::error on failure
   */
  static sysv_segment create(key_t key, std::size_t size, int flags = 0);

  /**
   * Opens an existing segment.
   *
   * @throws posix::error on failure
   */
  static sysv_segment open(key_t key);

  /**
   * Default constructor.
   */
  sysv_segment() noexcept = delete;

  /**
   * Constructor.
   */
  sysv_segment(const int shmid,
               void* const shmaddr = nullptr)
    : _id(shmid),
      _addr(shmaddr) {}

  /**
   * Copy constructor.
   */
  sysv_segment(const sysv_segment& other) noexcept = delete;

  /**
   * Move constructor.
   */
  sysv_segment(sysv_segment&& other) noexcept = default;

  /**
   * Destructor.
   */
  ~sysv_segment() noexcept;

  /**
   * Copy assignment operator.
   */
  sysv_segment& operator=(const sysv_segment& other) noexcept = delete;

  /**
   * Move assignment operator.
   */
  sysv_segment& operator=(sysv_segment&& other) noexcept = default;

  /**
   * Determines whether the segment is attached.
   */
  inline bool is_attached() const noexcept {
    return _addr;
  }

  /**
   * Alias for `is_attached()`.
   *
   * @copydetails is_attached()
   */
  inline bool is_mapped() const noexcept {
    return is_attached();
  }

  /**
   * Returns the integer identifier for this segment.
   */
  inline int id() const noexcept {
    return _id;
  }

  /**
   * Returns a pointer to the attached memory segment.
   *
   * @pre `is_attached()` must be `true`.
   */
  template <typename T>
  inline const T* data(const std::size_t offset = 0) const noexcept {
    return reinterpret_cast<const T*>(data(offset));
  }

  /**
   * Returns a pointer to the attached memory segment.
   *
   * @pre `is_attached()` must be `true`.
   */
  template <typename T>
  inline T* data(const std::size_t offset = 0) noexcept {
    return reinterpret_cast<T*>(data(offset));
  }

  /**
   * Returns a pointer to the attached memory segment.
   *
   * @pre `is_attached()` must be `true`.
   */
  inline const std::uint8_t* data(const std::size_t offset = 0) const noexcept {
    return reinterpret_cast<const std::uint8_t*>(_addr) + offset;
  }

  /**
   * Returns a pointer to the attached memory segment.
   *
   * @pre `is_attached()` must be `true`.
   */
  inline std::uint8_t* data(const std::size_t offset = 0) noexcept {
    return reinterpret_cast<std::uint8_t*>(_addr) + offset;
  }

  /**
   * Returns information from the associated kernel data structure.
   *
   * @throws posix::error on failure
   */
  shmid_ds stat() const;

  /**
   * Attaches this segment to the process address space.
   *
   * @throws posix::error on failure
   * @note This operation is idempotent.
   */
  void* attach(int flags = 0);

  /**
   * Detaches this segment from the process address space.
   *
   * @throws posix::error on failure
   * @note This operation is idempotent.
   */
  void detach();

  /**
   * Marks this segment to be destroyed. The segment will only actually be
   * removed after the last process detaches from it.
   *
   * @throws posix::error on failure
   * @note This operation is idempotent.
   */
  void remove();

  /**
   * Locks this segment into resident memory.
   *
   * @throws posix::error on failure
   * @note This operation is Linux-specific.
   */
  void lock();

  /**
   * Unlocks this segment, allowing it to be swapped out.
   *
   * @throws posix::error on failure
   * @note This operation is Linux-specific.
   */
  void unlock();

protected:
  int _id = -1;
  void* _addr = nullptr;
};

#endif /* POSIXXX_SYSV_SEGMENT_H */
