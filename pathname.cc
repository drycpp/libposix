/* This is free and unencumbered software released into the public domain. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "posix++/pathname.h"

#include <cassert>      /* for assert() */
#include <cerrno>       /* for errno */
#include <system_error> /* for std::system_error */

using namespace posix;
