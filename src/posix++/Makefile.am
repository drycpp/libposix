AM_CPPFLAGS     = -I$(top_srcdir)/lib -I$(top_srcdir)/src -iquote $(srcdir)
AM_CXXFLAGS     = -Wall -Wextra -pipe
AM_LDFLAGS      =
LDADD           = libposix++.la $(top_srcdir)/lib/libcompat.a
lib_LTLIBRARIES = libposix++.la

libposix___la_SOURCES =   \
  descriptor.cc           \
  directory.cc            \
  error.cc                \
  feature.cc              \
  file.cc                 \
  group.cc                \
  mapped_file.cc          \
  memory_mapping.cc       \
  mode.cc                 \
  module.cc               \
  named_pipe.cc           \
  pathname.cc             \
  process.cc              \
  process_group.cc        \
  thread.cc               \
  user.cc                 \
  version.cc

base_pkgincludedir = $(includedir)/posix++

base_pkginclude_HEADERS = \
  descriptor.h            \
  directory.h             \
  error.h                 \
  feature.h               \
  file.h                  \
  group.h                 \
  mapped_file.h           \
  memory_mapping.h        \
  mode.h                  \
  module.h                \
  named_pipe.h            \
  pathname.h              \
  process.h               \
  process_group.h         \
  thread.h                \
  user.h                  \
  version.h

if !DISABLE_MQUEUE
  libposix___la_SOURCES   += message_queue.cc
  base_pkginclude_HEADERS += message_queue.h
endif

if !DISABLE_SEMAPHORE
  base_pkginclude_HEADERS += semaphore.h
endif

if !DISABLE_STDIO
  libposix___la_SOURCES   += stdio.cc
  base_pkginclude_HEADERS += stdio.h
endif

if !DISABLE_SOCKET
  libposix___la_SOURCES   += socket.cc local_socket.cc
  base_pkginclude_HEADERS += socket.h local_socket.h
endif

if !DISABLE_SYSV
  libposix___la_SOURCES   += sysv_segment.cc
  base_pkginclude_HEADERS += sysv_segment.h
endif
