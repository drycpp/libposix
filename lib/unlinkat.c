/* This is free and unencumbered software released into the public domain. */

#include "unlinkat.h"

#include <errno.h>  /* for ENOSYS, errno */
#include <unistd.h> /* for unlink() */

int
unlinkat(const int dirfd,
         const char* const pathname,
         const int flags) {
  (void)dirfd, (void)pathname, (void)flags;

  return errno = ENOSYS, -1; // TODO
}
