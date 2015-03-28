/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_NAMED_PIPE_H
#define POSIXXX_NAMED_PIPE_H

#ifndef __cplusplus
#error "<posix++/named_pipe.h> requires a C++ compiler"
#endif

////////////////////////////////////////////////////////////////////////////////

#include "descriptor.h"
#include "mode.h"

#include <utility> /* for std::move(), std::pair */

namespace posix {
  class directory;
  class named_pipe;
  class pathname;
}

////////////////////////////////////////////////////////////////////////////////

/**
 * Represents a POSIX named pipe (aka FIFO).
 *
 * @see http://en.wikipedia.org/wiki/Named_pipe
 */
class posix::named_pipe : public posix::descriptor {
public:
  static void create(const pathname& pathname, mode mode);

  static void create(const directory& directory, const pathname& pathname, mode mode);

  static named_pipe open(const pathname& pathname, int flags);

  static named_pipe open(const directory& directory, const pathname& pathname, int flags);

  /**
   * Default constructor.
   */
  named_pipe() = delete;

  /**
   * Constructor.
   */
  named_pipe(const int fd) noexcept
    : descriptor(fd) {}

  /**
   * Copy constructor.
   */
  named_pipe(const named_pipe& other) = default; /* may throw */

  /**
   * Move constructor.
   */
  named_pipe(named_pipe&& other) noexcept = default;

  /**
   * Destructor.
   */
  ~named_pipe() noexcept = default;

protected:
  static void create(int dirfd, const char* pathname, mode mode = 0);
  static named_pipe open(int dirfd, const char* pathname, int flags);
};

////////////////////////////////////////////////////////////////////////////////

#endif /* POSIXXX_NAMED_PIPE_H */
