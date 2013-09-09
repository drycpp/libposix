/* This is free and unencumbered software released into the public domain. */

#include "openat.h"

#include <errno.h>    /* for ENOSYS, errno */
#include <sys/stat.h> /* for mode_t */

int
openat(const int dirfd,
       const char* const pathname,
       const int flags, ...) {
  (void)dirfd, (void)pathname, (void)flags;

  return errno = ENOSYS, -1; // TODO
}
