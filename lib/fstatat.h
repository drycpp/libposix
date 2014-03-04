/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_FSTATAT_H
#define POSIXXX_FSTATAT_H

/**
 * A public domain implementation of the POSIX.1-2008 fstatat() function.
 *
 * @see http://pubs.opengroup.org/onlinepubs/9699919799/functions/fstatat.html
 */

#include <sys/stat.h>

#ifdef __cplusplus
extern "C" {
#endif

#define AT_SYMLINK_NOFOLLOW 0x100

int fstatat(int dirfd, const char* pathname, struct stat* buf, int flags);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* POSIXXX_FSTATAT_H */
