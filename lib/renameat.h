/* This is free and unencumbered software released into the public domain. */

#ifndef POSIXXX_RENAMEAT_H
#define POSIXXX_RENAMEAT_H

/**
 * A public domain implementation of the POSIX.1-2008 renameat() function.
 *
 * @see http://pubs.opengroup.org/onlinepubs/9699919799/functions/rename.html
 */

#ifdef __cplusplus
extern "C" {
#endif

int renameat(
  int olddirfd, const char* oldpath,
  int newdirfd, const char* newpath);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* POSIXXX_RENAMEAT_H */
