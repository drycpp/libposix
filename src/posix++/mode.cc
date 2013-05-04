/* This is free and unencumbered software released into the public domain. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "mode.h"

#include <sys/types.h> /* for mode_t */

using namespace posix;

static_assert(sizeof(mode_t) == sizeof(mode), "sizeof(mode_t) != sizeof(mode)");
