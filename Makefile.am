AM_CPPFLAGS     = -I$(top_srcdir)/lib -I$(top_srcdir)/src -iquote $(srcdir)
AM_CXXFLAGS     = -Wall -Wextra -pipe
AM_LDFLAGS      = -rdynamic -Xlinker '-z,relro' -Xlinker '--as-needed' -lrt
LDADD           = libposix++.la
lib_LTLIBRARIES = libposix++.la

libposix___la_SOURCES =   \
  descriptor.cc           \
  directory.cc            \
  feature.cc              \
  file.cc                 \
  group.cc                \
  local_socket.cc         \
  message_queue.cc        \
  mode.cc                 \
  module.cc               \
  pathname.cc             \
  process.cc              \
  process_group.cc        \
  socket.cc               \
  sysv_segment.cc         \
  user.cc                 \
  version.cc

base_pkgincludedir = $(includedir)/posix++

base_pkginclude_HEADERS = \
  descriptor.h            \
  directory.h             \
  feature.h               \
  file.h                  \
  group.h                 \
  local_socket.h          \
  message_queue.h         \
  mode.h                  \
  module.h                \
  pathname.h              \
  process.h               \
  process_group.h         \
  socket.h                \
  sysv_segment.h          \
  user.h                  \
  version.h
