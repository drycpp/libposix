dnl configure.ac                                        -*- Autoconf -*-
dnl
dnl Process this file with `autoconf` to produce a configure script.
dnl
dnl This is free and unencumbered software released into the public domain.
AC_PREREQ([2.68])

dnl Define version information:
m4_define([VERSION_MAJOR],
  m4_esyscmd([cut -d'.' -f1 VERSION | tr -d '\n']))
m4_define([VERSION_MINOR],
  m4_esyscmd([cut -d'.' -f2 VERSION | tr -d '\n']))
m4_define([VERSION_PATCH],
  m4_esyscmd([cut -d'.' -f3 VERSION | tr -d '\n']))
m4_define([VERSION_STRING],
  m4_esyscmd([git describe --dirty --always | tr -d '\n']))

dnl Define package information:
AC_INIT([libposix++], [VERSION_STRING],
  [arto@bendiken.net], [libposix++],
  [https://github.com/dryproject/libposixxx])

dnl Configure Autoconf:
AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_SRCDIR([src/posix++.h])
AC_CONFIG_AUX_DIR([etc/aclocal])
AC_CONFIG_MACRO_DIR([etc/aclocal])
AC_CONFIG_LIBOBJ_DIR([lib])

dnl Configure Automake:
AM_INIT_AUTOMAKE([foreign -Wall -Werror dist-bzip2 subdir-objects nostdinc])
AM_SILENT_RULES([yes])

dnl Check for programs:
AC_PROG_CC(clang gcc cc)
AC_PROG_CC_C99
AM_PROG_CC_C_O
AC_PROG_CPP
AC_PROG_CXX(clang++ g++ c++)
AC_PROG_CXXCPP
AC_USE_SYSTEM_EXTENSIONS
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])
LT_INIT
AC_LANG([C++])
AX_CXX_COMPILE_STDCXX_11([noext])

dnl Check for configuration options:
# --with-stdlib=libstdc++|libc++
AC_ARG_WITH([stdlib],
  [AS_HELP_STRING([--with-stdlib=LIB], [specify the C++ standard library to use [default=system]])],
  [], [with_stdlib=system])
AS_IF([test "x$with_stdlib" != "xsystem"],
  [CXXFLAGS="$CXXFLAGS -stdlib=$with_stdlib"
   LDFLAGS="$LDFLAGS -stdlib=$with_stdlib"])
# --enable-debug/--disable-debug
AC_ARG_ENABLE([debug],
  [AS_HELP_STRING([--enable-debug], [build with debugging support [default=no]])],
  [], [enable_debug=no])
AS_IF([test "x$enable_debug" != "xno"],
  [AC_DEFINE([DEBUG], [1], [Enable debugging support.])],
  [AC_DEFINE([NDEBUG], [1], [Disable assertions.])])
AM_CONDITIONAL([ENABLE_DEBUG], [test "x$enable_debug" != "xno"])
# --enable-unicode/--disable-unicode
AC_ARG_ENABLE([unicode],
  [AS_HELP_STRING([--disable-unicode], [omit support for Unicode strings])])
AS_IF([test "x$enable_unicode" == "xno"],
  [AC_DEFINE([DISABLE_UNICODE], 1, [Define to disable Unicode string support.])])
# --enable-mqueue/--disable-mqueue
AC_ARG_ENABLE([mqueue],
  [AS_HELP_STRING([--disable-mqueue], [omit support for POSIX message queues])])
AS_IF([test "x$enable_mqueue" == "xno"], [
  AC_DEFINE([DISABLE_MQUEUE], 1, [Define to disable POSIX message queue support.])])
AM_CONDITIONAL([DISABLE_MQUEUE], [test "x$enable_mqueue" == "xno"])
# --enable-semaphore/--disable-semaphore
AC_ARG_ENABLE([semaphore],
  [AS_HELP_STRING([--disable-semaphore], [omit support for POSIX/SysV semaphores])])
AS_IF([test "x$enable_semaphore" == "xno"], [
  AC_DEFINE([DISABLE_SEMAPHORE], 1, [Define to disable POSIX/SysV semaphore support.])])
AM_CONDITIONAL([DISABLE_SEMAPHORE], [test "x$enable_semaphore" == "xno"])
# --enable-stdio/--disable-stdio
AC_ARG_ENABLE([stdio],
  [AS_HELP_STRING([--disable-stdio], [omit support for POSIX standard I/O])])
AS_IF([test "x$enable_stdio" == "xno"], [
  AC_DEFINE([DISABLE_STDIO], 1, [Define to disable POSIX standard I/O support.])])
AM_CONDITIONAL([DISABLE_STDIO], [test "x$enable_stdio" == "xno"])
# --enable-socket/--disable-socket
AC_ARG_ENABLE([socket],
  [AS_HELP_STRING([--disable-socket], [omit support for POSIX sockets])])
AS_IF([test "x$enable_socket" == "xno"], [
  AC_DEFINE([DISABLE_SOCKET], 1, [Define to disable POSIX socket support.])])
AM_CONDITIONAL([DISABLE_SOCKET], [test "x$enable_socket" == "xno"])
# --enable-sysv/--disable-sysv
AC_ARG_ENABLE([sysv],
  [AS_HELP_STRING([--disable-sysv], [omit support for SysV features])])
AS_IF([test "x$enable_sysv" == "xno"], [
  AC_DEFINE([DISABLE_SYSV], 1, [Define to disable SysV feature support.])])
AM_CONDITIONAL([DISABLE_SYSV], [test "x$enable_sysv" == "xno"])

dnl Check for libraries:

dnl Check for header files:
AC_LANG_PUSH([C])
AC_HEADER_ASSERT
AC_HEADER_STDBOOL
AC_CHECK_HEADERS_ONCE([mqueue.h])
AC_LANG_POP([C])

dnl Check for types:

dnl Check for structures:

dnl Check for compiler characteristics:
AC_CANONICAL_HOST
AM_CPPFLAGS="$AM_CPPFLAGS -I\$(top_srcdir)/src -iquote \$(srcdir)"
AM_CXXFLAGS="$AM_CXXFLAGS -Wall -Wextra -pipe"
AM_LDFLAGS="$AM_LDFLAGS"
AS_CASE([$host_os], [linux*], [AM_LDFLAGS="$AM_LDFLAGS -pthread -lrt"], [])
TEST_CPPFLAGS="$AM_CPPFLAGS -DCATCH_CONFIG_MAIN"
TEST_CXXFLAGS="$AM_CXXFLAGS"
TEST_LDFLAGS="$AM_LDFLAGS"
AC_SUBST([AM_CPPFLAGS])
AC_SUBST([AM_CXXFLAGS])
AC_SUBST([AM_LDFLAGS])
AC_SUBST([TEST_CPPFLAGS])
AC_SUBST([TEST_CXXFLAGS])
AC_SUBST([TEST_LDFLAGS])

dnl Check for library functions:
AC_CHECK_FUNCS_ONCE([accept4])
AC_REPLACE_FUNCS([fdopendir fstatat linkat mkdirat mkfifoat openat readlinkat renameat symlinkat unlinkat])

dnl Check for system services:

dnl Generate output:
AC_CONFIG_FILES([Makefile lib/Makefile src/Makefile src/posix++/Makefile test/Makefile])
AC_SUBST([PACKAGE_VERSION_MAJOR], ["VERSION_MAJOR"])
AC_SUBST([PACKAGE_VERSION_MINOR], ["VERSION_MINOR"])
AC_SUBST([PACKAGE_VERSION_PATCH], ["VERSION_PATCH"])
AC_CONFIG_FILES([src/posix++/version.h])
AH_BOTTOM([#include "libcompat.h"])
AC_OUTPUT
