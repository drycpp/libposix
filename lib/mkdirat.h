/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_MKDIRAT_H
#define POSIXXX_MKDIRAT_H

/**
 * A public domain implementation of the POSIX.1-2008 mkdirat() function.
 *
 * @see http://pubs.opengroup.org/onlinepubs/9699919799/functions/mkdir.html
 */

#include <sys/stat.h>

#ifdef __cplusplus
extern "C" {
#endif

int mkdirat(int dirfd, const char* pathname, mode_t mode);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* POSIXXX_MKDIRAT_H */
