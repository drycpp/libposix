/* This is free and unencumbered software released into the public domain. */

#include "readlinkat.h"

#include <errno.h>  /* for ENOSYS, errno */
#include <unistd.h> /* for readlink() */

int
readlinkat(const int dirfd,
           const char* const pathname,
           char* const buffer,
           const size_t buffer_size) {
  (void)dirfd, (void)pathname, (void)buffer, (void)buffer_size;

  return errno = ENOSYS, -1; // TODO
}
