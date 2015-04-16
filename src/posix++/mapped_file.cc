/* This is free and unencumbered software released into the public domain. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "directory.h"
#include "error.h"
#include "mapped_file.h"
#include "pathname.h"

#include <algorithm>  /* for std::max(), std::min() */
#include <cassert>    /* for assert() */
#include <cerrno>     /* for errno */
#include <cstring>    /* for std::memmove() */
#include <fcntl.h>    /* for AT_FDCWD */
#include <unistd.h>   /* for _SC_PAGE_SIZE, sysconf() */
#include <utility>    /* for std::swap() */

#ifdef __linux__
#include <sys/mman.h> /* for MREMAP_MAYMOVE */
#endif

using namespace posix;

////////////////////////////////////////////////////////////////////////////////

mapped_file
mapped_file::open(const pathname& pathname,
                  const int flags,
                  const mode mode) {

  return mapped_file{AT_FDCWD, pathname.c_str(), flags, mode};
}

mapped_file
mapped_file::open(const directory& directory,
                  const pathname& pathname,
                  const int flags,
                  const mode mode) {

  return mapped_file{directory.fd(), pathname.c_str(), flags, mode};
}

////////////////////////////////////////////////////////////////////////////////

namespace {
  static std::size_t system_page_size() {
    return static_cast<std::size_t>(::sysconf(_SC_PAGE_SIZE));
  }
}

mapped_file::mapped_file(const int dirfd,
                         const char* const pathname,
                         int flags,
                         const mode mode)
  : file{dirfd, pathname, flags, mode},
    _size{file::size()},
    _offset{file::seek(0, SEEK_CUR)},
    _mapping{*this, std::max(_size, system_page_size())} {

  assert(_mapping.data());
}

mapped_file::mapped_file(mapped_file&& other) noexcept
  : mapped_file{} {

  std::swap(_fd, other._fd);
  std::swap(_size, other._size);
  std::swap(_offset, other._offset);
  std::swap(_mapping, other._mapping);
}

mapped_file&
mapped_file::operator=(mapped_file&& other) noexcept {
  if (this != &other) {
    std::swap(_fd, other._fd);
    std::swap(_size, other._size);
    std::swap(_offset, other._offset);
    std::swap(_mapping, other._mapping);
  }
  return *this;
}

////////////////////////////////////////////////////////////////////////////////

void
mapped_file::sync() {
  if (_mapping.writable()) {
    file::sync();
  }

  std::size_t new_size = file::size();
  if (new_size != _size) {
    _size = new_size;
    new_size = std::max(_size, system_page_size());
#ifdef __linux__
    _mapping.remap(new_size, MREMAP_MAYMOVE);
#else
    throw_error(ENOSYS); // TODO: _mapping = ...
#endif
  }
}

std::size_t
mapped_file::seek(const off_t offset,
                  const int whence) {

  switch (whence) {
    case SEEK_CUR: {
      _offset += offset; // TODO: error handling
      break;
    }
    case SEEK_SET: {
      _offset = file::seek(offset, whence);
      break;
    }
    case SEEK_END:
    default: {
      _offset = file::seek(offset, whence);
      if (_offset > _mapping.size()) {
        // TODO: _mapping.extend();
      }
      break;
    }
  }
  return _offset;
}

std::size_t
mapped_file::read_line(std::string& buffer) {
  return read_until('\n', buffer); /* read_until() updates _offset */
}

std::size_t
mapped_file::read_until(const char separator,
                        std::string& buffer) {
  std::size_t byte_count = 0;
  char byte;
  while (read(byte)) { /* read() updates _offset */
    byte_count++;
    if (byte == separator)
      break; /* all done */
    buffer.push_back(byte);
  }
  return byte_count;
}

std::size_t
mapped_file::read(char& result) {
  if (is_eof()) {
    return 0; /* EOF */
  }
  result = *_mapping.data<char>(_offset);
  _offset++;
  return 1;
}

std::size_t
mapped_file::read(void* const buffer,
                  const std::size_t buffer_size) {
  assert(buffer != nullptr);
  if (is_eof()) {
    return 0; /* EOF */
  }
  const auto length = std::min(buffer_size, _size - _offset);
  std::memmove(buffer, _mapping.data(_offset), length);
  _offset += length;
  return length;
}

std::string
mapped_file::read() {
  std::string buffer;
  if (!is_eof()) {
    const auto length = _size - _offset;
    buffer.append(_mapping.data<char>(_offset), length);
    _offset += length;
    assert(is_eof());
  }
  return buffer;
}

////////////////////////////////////////////////////////////////////////////////

appendable_mapped_file
appendable_mapped_file::open(const pathname& pathname,
                             const int flags,
                             const mode mode) {

  return appendable_mapped_file{AT_FDCWD, pathname.c_str(), flags, mode};
}

appendable_mapped_file
appendable_mapped_file::open(const directory& directory,
                             const pathname& pathname,
                             const int flags,
                             const mode mode) {

  return appendable_mapped_file{directory.fd(), pathname.c_str(), flags, mode};
}

////////////////////////////////////////////////////////////////////////////////

appendable_mapped_file::appendable_mapped_file(appendable_mapped_file&& other) noexcept
  : appendable_mapped_file{} {

  std::swap(_fd, other._fd);
  std::swap(_size, other._size);
  std::swap(_offset, other._offset);
  std::swap(_mapping, other._mapping);
}

appendable_mapped_file&
appendable_mapped_file::operator=(appendable_mapped_file&& other) noexcept {
  if (this != &other) {
    std::swap(_fd, other._fd);
    std::swap(_size, other._size);
    std::swap(_offset, other._offset);
    std::swap(_mapping, other._mapping);
  }
  return *this;
}

////////////////////////////////////////////////////////////////////////////////

std::size_t
appendable_mapped_file::append(const void* const data,
                               const std::size_t size) {

  const auto offset = seek(0, SEEK_END);

  write(data, size);
  _offset += size;

  return offset;
}
