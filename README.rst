***********************************
libposix++: C++14 wrapper for POSIX
***********************************

.. image:: https://api.travis-ci.org/dryproject/libposix.svg?branch=master
   :target: https://travis-ci.org/dryproject/libposix
   :alt: Travis CI build status

.. image:: https://scan.coverity.com/projects/3229/badge.svg
   :target: https://scan.coverity.com/projects/3229
   :alt: Coverity Scan build status

Features
========

* Provides lightweight C++11 abstractions encapsulating core POSIX interfaces.
* Targets Linux 3.x+, FreeBSD 9.x+, and Mac OS X 10.7+ (in that order).
* No runtime dependencies other than the system's C++ standard library.
* No build prerequisites beyond the Autotools toolchain and a C++11 compiler.
* Compatible with Clang and GCC, or any standard C++11 implementation.
* 100% free and unencumbered `public domain <http://unlicense.org/>`_ software,
  usable in any context and for any purpose.

Design Principles
=================

* Provides zero-cost abstractions wrapping file descriptors.
* Ensures that file descriptors are opened with ``O_CLOEXEC``.
* Mitigates various race conditions entailed by the POSIX interfaces.
* Encapsulates ``EINTR`` and ``EAGAIN`` error handling.
* Differentiates logic errors, runtime errors, and fatal errors.
* Avoids the transitive inclusion of POSIX headers where possible.

Caveats
=======

**Caveat utilitor**: the project's top-level ``posix`` namespace may have to
be changed to something else in the future, as the C++ standard now frowns
upon using it:

   C++11 §17.6.4.2.2 [namespace.posix]

   The behavior of a C++ program is undefined if it adds declarations or
   definitions to namespace posix or to a namespace within namespace posix
   unless otherwise specified. The namespace posix is reserved for use by
   ISO/IEC 9945 and other POSIX standards.

Error Handling
==============

This library makes a careful distinction between three different classes of
error conditions, all deriving from the ``posix::error`` base class:

* **Logic errors**, represented by ``posix::logic_error``. Errors of this
  class are thrown due to programming mistakes where the POSIX interfaces
  are being used in violation of documented preconditions. A common strategy
  for handling this class of error conditions is to abort the program with a
  core dump, facilitating introspection to locate and remedy the bug.
* **Fatal errors**, represented by ``posix::fatal_error``. Errors of this
  class are thrown due to the exhaustion of critical system resources, such
  as available memory (``ENOMEM``) and disk space (``ENOSPC``), or due to
  attempts to exceed applicable system resource limits, such as the maximum
  number of open file descriptors (``EMFILE`` and ``ENFILE``). A typical
  strategy for handling this class of error conditions is to terminate the
  program with a descriptive error message. More robust programs and shared
  libraries may implement more complicated strategies, such as retrying the
  top-level operation after first letting most of the call stack unwind in
  order to free up scarce resources.
* **Runtime errors**, represented by ``posix::runtime_error``. Errors of
  this class are thrown as a matter of course to indicate various
  exceptional conditions such as missing files (``ENOENT``) or insufficient
  privileges (``EPERM`` and ``EACCES``). These conditions are generally
  recoverable and robust programs will take care to catch and handle them.

.. note::

   The distinction between logic errors and runtime errors mirrors that
   found in the C++11 standard library, where the ``<stdexcept>`` header
   defines the standard exception base classes ``std::logic_error`` and
   ``std::runtime_error``.

The following example demonstrates error handling with ``libposix++``::

   try {
     // ... code goes here ...
   }
   catch (const posix::logic_error& error) {
     /* Handle programmer errors such as precondition violations: */
     std::abort();
   }
   catch (const posix::fatal_error& error) {
     /* Handle fatal errors such as system resource exhaustion: */
     std::fprintf(stderr, "Fatal error: %s\n", error.what());
     std::exit(EX_OSERR);
   }
   catch (const posix::runtime_error& error) {
     /* Handle any runtime errors that were expected at this point: */
     switch (error.number()) {
       case EPERM:  /* Permission denied */
       case EACCES: /* No such file or directory */
       case ENOENT: /* Operation not permitted */
         // ... code goes here ...
         break;
       default: throw;
     }
   }

Build Prerequisites
===================

* Clang_ (>= 3.2) or GCC_ (>= 4.8)
* Autoconf_ (>= 2.68)
* Automake_ (>= 1.11)
* Libtool_ (>= 2.2)

.. note::

   Older releases may work, but are not actively tested for.

.. _Clang:    http://clang.llvm.org/
.. _GCC:      http://gcc.gnu.org/
.. _Autoconf: http://www.gnu.org/software/autoconf/
.. _Automake: http://www.gnu.org/software/automake/
.. _Libtool:  http://www.gnu.org/software/libtool/

Installation
============

Installation on Unix
--------------------

::

   $ ./autogen.sh
   $ ./configure                        # on Linux
   $ ./configure --with-stdlib=libc++   # on FreeBSD / Mac OS X
   $ make
   $ sudo make install
   $ sudo ldconfig                      # on Linux

Elsewhere
=========

Author
======

This project is part of the `DRY <http://dryproject.org/>`_ initiative.

* `Arto Bendiken <https://github.com/bendiken>`_ - http://ar.to/

Donations
=========

If you found this software useful and would like to encourage its
maintenance and further development, please consider making a $5 donation
to the author(s) via Gratipay_ or Bitcoin_.

.. _Gratipay: https://gratipay.com/bendiken/
.. _Bitcoin:  bitcoin:1LevW42qPf44ynzrPrkvQpvuchfxgHvr9w?label=libposix.dryproject.org&message=Donation

License
=======

This is free and unencumbered public domain software. For more information,
see http://unlicense.org/ or the accompanying ``UNLICENSE`` file.
