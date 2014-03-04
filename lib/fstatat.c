/* This is free and unencumbered software released into the public domain. */

#include "fstatat.h"

#include <errno.h> /* for ENOSYS, errno */

int
fstatat(const int dirfd,
        const char* const pathname,
        struct stat* const buf,
        const int flags) {
  return errno = ENOSYS, -1; // TODO
}
