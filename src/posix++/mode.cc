/* This is free and unencumbered software released into the public domain. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "mode.h"

#include <sys/types.h> /* for mode_t */

using namespace posix;

/*
 * x86-64 Linux 3.x:   uint32_t.
 * x86-64 Darwin 11.x: uint16_t.
 */
static_assert(sizeof(mode_t) <= sizeof(mode), "sizeof(mode_t) > sizeof(mode)");
