/* This is free and unencumbered software released into the public domain. */

#include "symlinkat.h"

#include <errno.h> /* for ENOSYS, errno */

int
symlinkat(const char* const oldpath,
          const int newdirfd,
          const char* const newpath) {
  return errno = ENOSYS, -1; // TODO
}
