AM_CPPFLAGS     = -I$(top_srcdir)/lib -I$(top_srcdir)/src -iquote $(srcdir)
AM_CXXFLAGS     = -Wall -Wextra -pipe
AM_LDFLAGS      = -rdynamic -Xlinker '-z,relro' -Xlinker '--as-needed'
LDADD           = libposix++.la
lib_LTLIBRARIES = libposix++.la

libposix___la_SOURCES =   \
  descriptor.cc           \
  directory.cc            \
  feature.cc              \
  file.cc                 \
  group.cc                \
  message_queue.cc        \
  module.cc               \
  pathname.cc             \
  process.cc              \
  process_group.cc        \
  user.cc                 \
  version.cc

base_pkgincludedir = $(includedir)/posix++

base_pkginclude_HEADERS = \
  descriptor.h            \
  directory.h             \
  feature.h               \
  file.h                  \
  group.h                 \
  message_queue.h         \
  module.h                \
  pathname.h              \
  process.h               \
  process_group.h         \
  user.h                  \
  version.h
