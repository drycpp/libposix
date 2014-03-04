/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_MKFIFOAT_H
#define POSIXXX_MKFIFOAT_H

/**
 * A public domain implementation of the POSIX.1-2008 mkfifoat() function.
 *
 * @see http://pubs.opengroup.org/onlinepubs/9699919799/functions/mkfifo.html
 */

#include <sys/stat.h>

#ifdef __cplusplus
extern "C" {
#endif

int mkfifoat(int dirfd, const char* pathname, mode_t mode);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* POSIXXX_MKFIFOAT_H */
