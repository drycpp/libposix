AM_CPPFLAGS = -I$(top_srcdir)/lib -I$(top_srcdir)/src -iquote $(srcdir) $(BOOST_CPPFLAGS) -DBOOST_TEST_DYN_LINK
AM_CXXFLAGS = -Wall -Wextra -pipe
AM_LDFLAGS  = $(BOOST_LDFLAGS) $(BOOST_UNIT_TEST_FRAMEWORK_LIB)
LDADD       = $(top_srcdir)/src/posix++/libposix++.la

check_PROGRAMS =       \
  check_descriptor     \
  check_directory      \
  check_error          \
  check_feature        \
  check_file           \
  check_group          \
  check_memory_mapping \
  check_module         \
  check_pathname       \
  check_process        \
  check_process_group  \
  check_user           \
  check_version

if !DISABLE_MQUEUE
  check_PROGRAMS += check_message_queue
endif

if !DISABLE_STDIO
  check_PROGRAMS += check_stdio
endif

if !DISABLE_SOCKET
  check_PROGRAMS += check_socket check_local_socket
endif

if !DISABLE_SYSV
  check_PROGRAMS += check_sysv_segment
endif

TESTS = $(check_PROGRAMS)

AM_DEFAULT_SOURCE_EXT = .cc
