/* This is free and unencumbered software released into the public domain. */

#include "fdopendir.h"

#include <errno.h>  /* for ENOSYS, errno */
#include <stddef.h> /* for NULL */

DIR*
fdopendir(const int fd) {
  (void)fd;

  return errno = ENOSYS, NULL; // TODO
}
