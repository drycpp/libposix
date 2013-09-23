/* This is free and unencumbered software released into the public domain. */

#include <posix++/error.h>
#include <posix++/sysv_segment.h>

#include <cstddef>  /* for std::size_t */
#include <cstdio>   /* for std::fwrite() */
#include <cstdlib>  /* for std::atoi() */
#include <unistd.h> /* for sysconf() */

static const std::size_t pagesize = sysconf(_SC_PAGESIZE);

int
main(int argc, char* argv[]) {
  /* For each command-line argument: */
  for (int i = 1; i < argc; i++) {
    /* Convert the argument to an integer: */
    const int arg = std::atoi(argv[i]);

    /* Attach to the SysV shared-memory segment: */
    posix::sysv_segment shm(arg);
    shm.attach();

    /* Write the first page to standard output: */
    std::fwrite(shm.data(), pagesize, 1, stdout);
  }
  return EXIT_SUCCESS;
}
