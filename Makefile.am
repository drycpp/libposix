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
  message_queue.cc        \
  module.cc               \
  version.cc

base_pkgincludedir = $(includedir)/posix++

base_pkginclude_HEADERS = \
  descriptor.h            \
  directory.h             \
  feature.h               \
  file.h                  \
  message_queue.h         \
  module.h                \
  version.h
