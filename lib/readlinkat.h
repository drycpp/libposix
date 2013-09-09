/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_READLINKAT_H
#define POSIXXX_READLINKAT_H

/**
 * A public domain implementation of the POSIX.1-2008 readlinkat() function.
 *
 * @see http://pubs.opengroup.org/onlinepubs/9699919799/functions/readlink.html
 */

#include <stddef.h> /* for size_t */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * A special `dirfd` value used to indicate that `readlinkat()` should operate
 * relative to the current working directory.
 */
#ifndef AT_FDCWD
#define AT_FDCWD     -100
#endif

/**
 * Reads a symlink relative to a directory file descriptor.
 *
 * @param dirfd
 * @param pathname
 * @param buffer
 * @param buffer_size
 * @return the number of bytes placed in `buffer`, or -1 on error
 * @see http://pubs.opengroup.org/onlinepubs/9699919799/functions/readlink.html
 */
int readlinkat(int dirfd, const char* pathname, char* buffer, size_t buffer_size);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* POSIXXX_READLINKAT_H */
