/* This is free and unencumbered software released into the public domain. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "error.h"
#include "descriptor.h"
#include "memory_mapping.h"

#include <cassert>     /* for assert() */
#include <cerrno>      /* for errno */
#include <sys/mman.h>  /* for mmap(), munmap() */
#include <sys/stat.h>  /* for fstat() */
#include <sys/types.h> /* for struct stat */

using namespace posix;

////////////////////////////////////////////////////////////////////////////////

std::uint8_t*
memory_mapping::map(const int fd,
                    const std::size_t size,
                    const std::size_t offset) {
  assert(fd >= 0);
  assert(size > 0);

  const int prot = PROT_READ; // FIXME

  const int flags = MAP_SHARED;

  if (size == static_cast<std::size_t>(-1)) {
    struct stat st;
    if (fstat(fd, &st) == -1) {
      throw_error("fstat", "%d, %s", fd, "buffer");
    }
    _size = static_cast<std::size_t>(st.st_size) - offset;
  }
  else {
    _size = size;
  }

  void* const addr = ::mmap(nullptr, _size, prot, flags, fd, offset);
  if (addr == MAP_FAILED) {
    throw_error("mmap", "%p, %zu, 0x%x, 0x%x, %d, %zu",
      nullptr, _size, static_cast<unsigned int>(prot),
      static_cast<unsigned int>(flags), fd, offset);
  }

  return reinterpret_cast<std::uint8_t*>(addr);
}

////////////////////////////////////////////////////////////////////////////////

memory_mapping::memory_mapping(const descriptor& descriptor)
  : memory_mapping{descriptor.fd()} {}

memory_mapping::memory_mapping(const descriptor& descriptor,
                               const std::size_t size,
                               const std::size_t offset)
  : memory_mapping{descriptor.fd(), size, offset} {}

memory_mapping::memory_mapping(const int fd)
  : _size{0},
    _data{map(fd, static_cast<std::size_t>(-1), 0)} {}

memory_mapping::memory_mapping(const int fd,
                               const std::size_t size,
                               const std::size_t offset)
  : _size{size},
    _data{map(fd, size, offset)} {}

memory_mapping::memory_mapping(void* const data,
                               const std::size_t size) noexcept
  : _size{size},
    _data{reinterpret_cast<std::uint8_t*>(data)} {}

memory_mapping::~memory_mapping() noexcept {
  if (_data) {
    if (::munmap(reinterpret_cast<void*>(_data), _size) == -1) {
      /* Ignore any errors from munmap() here. */
    }
    _data = nullptr;
    _size = 0;
  }
}

////////////////////////////////////////////////////////////////////////////////

bool
memory_mapping::readable() const noexcept {
  return _data != nullptr; // TODO
}

bool
memory_mapping::writable() const noexcept {
  return _data != nullptr; // TODO
}

bool
memory_mapping::executable() const noexcept {
  return _data != nullptr; // TODO
}
