/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_DIRECTORY_H
#define POSIXXX_DIRECTORY_H

#include <string> /* for std::string */

namespace posix {
  class directory {
    public:
      class iterator;

      directory(const std::string& pathname);

      directory(const std::string& pathname, int dirfd);

      ~directory();

      void close();

      iterator begin() const;

      iterator end() const;

      const iterator cbegin() const;

      const iterator cend() const;

      bool exists() const {
        return true;
      }

      int fd() const {
        return _fd;
      }

    protected:
      int _fd = -1;

      void open(int dirfd, const std::string& pathname);
  };

  class directory::iterator {
    public:
      iterator() {}

      iterator(const directory& dir);

      ~iterator();

      bool operator==(const iterator& other) {
        return !operator!=(other);
      }

      bool operator!=(const iterator& other);

      iterator& operator++();

      std::string operator*();

    protected:
      void* _dirp = nullptr;
  };
}

#endif /* POSIXXX_DIRECTORY_H */
