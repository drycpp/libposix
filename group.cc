/* This is free and unencumbered software released into the public domain. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "group.h"

#include <cassert>      /* for assert() */
#include <cerrno>       /* for errno */
#include <grp.h>        /* for getgrnam(), getgrgid() */
#include <system_error> /* for std::system_error */

using namespace posix;
