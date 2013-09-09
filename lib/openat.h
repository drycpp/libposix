/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_OPENAT_H
#define POSIXXX_OPENAT_H

/**
 * A public domain implementation of the POSIX.1-2008 openat() function.
 *
 * @see http://pubs.opengroup.org/onlinepubs/9699919799/functions/open.html
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * A special `dirfd` value used to indicate that `openat()` should operate
 * relative to the current working directory.
 */
#ifndef AT_FDCWD
#define AT_FDCWD -100
#endif

/**
 * Opens a file relative to a directory file descriptor.
 *
 * @param dirfd
 * @param pathname
 * @param flags
 * @param mode
 * @return a new file descriptor, or -1 on error
 * @see http://pubs.opengroup.org/onlinepubs/9699919799/functions/open.html
 */
int openat(int dirfd, const char* pathname, int flags, ...);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* POSIXXX_OPENAT_H */
