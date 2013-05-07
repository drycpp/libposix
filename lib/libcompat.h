/* This is free and unencumbered software released into the public domain. */

#ifndef LIBCOMPAT_H
#define LIBCOMPAT_H

#ifndef HAVE_FDOPENDIR
#include "fdopendir.h"
#endif

#ifndef HAVE_OPENAT
#include "openat.h"
#endif

#endif /* LIBCOMPAT_H */
