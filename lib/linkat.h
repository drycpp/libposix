/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_LINKAT_H
#define POSIXXX_LINKAT_H

/**
 * A public domain implementation of the POSIX.1-2008 linkat() function.
 *
 * @see http://pubs.opengroup.org/onlinepubs/9699919799/functions/link.html
 */

#ifdef __cplusplus
extern "C" {
#endif

int linkat(
  int olddirfd, const char* oldpath,
  int newdirfd, const char* newpath,
  int flags);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* POSIXXX_LINKAT_H */
