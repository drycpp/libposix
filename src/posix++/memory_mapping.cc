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

void*
memory_mapping::map(const int fd,
                    const std::size_t size,
                    const std::size_t offset) {

  const int prot = PROT_READ;

  const int flags = MAP_SHARED;

  void* const addr = ::mmap(nullptr, size, prot, flags, fd, offset);
  if (addr == MAP_FAILED) {
    switch (errno) {
      case ENOMEM:  /* Cannot allocate memory in kernel */
      case ENFILE:  /* Too many open files in system */
        throw posix::fatal_error(errno);
      case EBADF:   /* Bad file descriptor */
        throw posix::bad_descriptor();
      case EINVAL:  /* Invalid argument */
        throw posix::invalid_argument();
      default:
        throw posix::runtime_error(errno);
    }
  }

  return addr;
}

memory_mapping::memory_mapping(const descriptor& descriptor)
  : memory_mapping(descriptor.fd()) {}

memory_mapping::memory_mapping(const descriptor& descriptor,
                               const std::size_t size,
                               const std::size_t offset)
  : memory_mapping(descriptor.fd(), size, offset) {}

memory_mapping::memory_mapping(const int fd) {
  assert(fd >= 0);

  struct stat st;

  if (fstat(fd, &st) == -1) {
    switch (errno) {
      case ENOMEM: /* Cannot allocate memory in kernel */
        throw posix::fatal_error(errno);
      case EBADF:  /* Bad file descriptor */
        throw posix::bad_descriptor();
      default:
        assert(errno != EFAULT);
        throw posix::runtime_error(errno);
    }
  }

  _size = static_cast<std::size_t>(st.st_size);
  _data = map(fd, _size, 0);
}

memory_mapping::memory_mapping(const int fd,
                               const std::size_t size,
                               const std::size_t offset) {
  assert(fd >= 0);
  assert(size > 0);

  _size = size;
  _data = map(fd, size, offset);
}

memory_mapping::memory_mapping(void* const data,
                               const std::size_t size) noexcept
  : _data(data),
    _size(size) {

  assert(data != nullptr);
  assert(size > 0);
}

memory_mapping::~memory_mapping() noexcept {
  if (_data) {
    if (::munmap(_data, _size) == -1) {
      /* Ignore any errors from munmap(). */
    }
    _data = nullptr;
    _size = 0;
  }
}

bool
memory_mapping::readable() const noexcept {
  return true; // TODO
}

bool
memory_mapping::writable() const noexcept {
  return true; // TODO
}

bool
memory_mapping::executable() const noexcept {
  return true; // TODO
}
