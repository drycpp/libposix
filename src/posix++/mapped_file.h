/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_MAPPED_FILE_H
#define POSIXXX_MAPPED_FILE_H

#ifndef __cplusplus
#error "<posix++/mapped_file.h> requires a C++ compiler"
#endif

////////////////////////////////////////////////////////////////////////////////

#include "file.h"

#include <cstring> /* for std::strlen() */
#include <string>  /* for std::string */

namespace posix {
  class mapped_file;
  class appendable_mapped_file;
}

////////////////////////////////////////////////////////////////////////////////

class posix::mapped_file : public posix::file {
public:
  /**
   * Constructor.
   */
  using file::file;

  static mapped_file open(const pathname& pathname, int flags, mode mode = 0);

  static mapped_file open(const directory& directory, const pathname& pathname, int flags, mode mode = 0);
};

////////////////////////////////////////////////////////////////////////////////

class posix::appendable_mapped_file : public posix::mapped_file {
public:
  /**
   * Constructor.
   */
  using mapped_file::mapped_file;

  static appendable_mapped_file open(const pathname& pathname, int flags, mode mode = 0);

  static appendable_mapped_file open(const directory& directory, const pathname& pathname, int flags, mode mode = 0);

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
