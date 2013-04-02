/* This is free and unencumbered software released into the public domain. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "user.h"

#include <cassert>      /* for assert() */
#include <cerrno>       /* for errno */
#include <pwd.h>        /* for getpwnam(), getpwuid() */
#include <system_error> /* for std::system_error */

using namespace posix;
