/* This is free and unencumbered software released into the public domain. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "local_socket.h"

#include "pathname.h"

#include <cassert>      /* for assert() */
#include <cerrno>       /* for errno */
#include <sys/socket.h> /* for AF_LOCAL, socket() */
#include <system_error> /* for std::system_error */

using namespace posix;

local_socket
local_socket::open(const pathname& pathname) {
  assert(!pathname.empty());

  return local_socket(-1); // TODO
}
