/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_FDOPENDIR_H
#define POSIXXX_FDOPENDIR_H

/**
 * A public domain implementation of the POSIX.1-2008 fdopendir() function.
 *
 * @see http://pubs.opengroup.org/onlinepubs/9699919799/functions/opendir.html
 */

#include <dirent.h> /* for DIR */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Opens the directory associated with a file descriptor.
 *
 * @param fd
 * @return a pointer to the directory stream, or `NULL` on error
 * @see http://pubs.opengroup.org/onlinepubs/9699919799/functions/opendir.html
 */
DIR* fdopendir(int fd);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* POSIXXX_FDOPENDIR_H */
