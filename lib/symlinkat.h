/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_SYMLINKAT_H
#define POSIXXX_SYMLINKAT_H

/**
 * A public domain implementation of the POSIX.1-2008 symlinkat() function.
 *
 * @see http://pubs.opengroup.org/onlinepubs/9699919799/functions/symlink.html
 */

#ifdef __cplusplus
extern "C" {
#endif

int symlinkat(const char* oldpath, int newdirfd, const char* newpath);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* POSIXXX_SYMLINKAT_H */
