/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_UNLINKAT_H
#define POSIXXX_UNLINKAT_H

/**
 * A public domain implementation of the POSIX.1-2008 unlinkat() function.
 *
 * @see http://pubs.opengroup.org/onlinepubs/9699919799/functions/unlink.html
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * A special `dirfd` value used to indicate that `unlinkat()` should operate
 * relative to the current working directory.
 */
#ifndef AT_FDCWD
#define AT_FDCWD     -100
#endif

#ifndef AT_REMOVEDIR
#define AT_REMOVEDIR 0x200
#endif

/**
 * Removes a file or subdirectory relative to a directory file descriptor.
 *
 * @param dirfd
 * @param pathname
 * @param flags
 * @return zero on success, or -1 on error
 * @see http://pubs.opengroup.org/onlinepubs/9699919799/functions/unlink.html
 */
int unlinkat(int dirfd, const char* pathname, int flags);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* POSIXXX_UNLINKAT_H */
