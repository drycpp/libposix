/* This is free and unencumbered software released into the public domain. */

#include "linkat.h"

#include <errno.h> /* for ENOSYS, errno */

int
linkat(const int olddirfd,
       const char* const oldpath,
       const int newdirfd,
       const char* const newpath,
       const int flags) {
  return errno = ENOSYS, -1; // TODO
}
