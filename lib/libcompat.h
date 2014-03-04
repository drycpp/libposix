/* This is free and unencumbered software released into the public domain. */

#ifndef LIBCOMPAT_H
#define LIBCOMPAT_H

#ifdef __APPLE__
#include <sys/syslimits.h> /* for PATH_MAX */
#endif

#ifndef HAVE_FDOPENDIR
#include "fdopendir.h"
#endif

#ifndef HAVE_FSTATAT
#include "fstatat.h"
#endif

#ifndef HAVE_LINKAT
#include "linkat.h"
#endif

#ifndef HAVE_MKDIRAT
#include "mkdirat.h"
#endif

#ifndef HAVE_MKFIFOAT
#include "mkfifoat.h"
#endif

#ifndef HAVE_OPENAT
#include "openat.h"
#endif

#ifndef HAVE_READLINKAT
#include "readlinkat.h"
#endif

#ifndef HAVE_RENAMEAT
#include "renameat.h"
#endif

#ifndef HAVE_SYMLINKAT
#include "symlinkat.h"
#endif

#ifndef HAVE_UNLINKAT
#include "unlinkat.h"
#endif

#endif /* LIBCOMPAT_H */
