// Support for concurrent programing -*- C++ -*-

// Copyright (C) 2003-2019 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

/** @file ext/concurrence.h
 *  This file is a GNU extension to the Standard C++ Library.
 */

#ifndef _CONCURRENCE_H
#define _CONCURRENCE_H 1

#pragma GCC system_header

#include <exception>
#include <bits/gthr.h> 
#include <bits/functexcept.h>
#include <bits/cpp_type_traits.h>
#include <ext/type_traits.h>

namespace __gnu_cxx _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  // Available locking policies:
  // _S_single    single-threaded code that doesn't need to be locked.
  // _S_mutex     multi-threaded code that requires additional support
  //              from gthr.h or abstraction layers in concurrence.h.
  // _S_atomic    multi-threaded code using atomic operations.
  enum _Lock_policy { _S_single, _S_mutex, _S_atomic }; 

  // Compile time constant that indicates prefered locking policy in
  // the current configuration.
  static const _Lock_policy __default_lock_policy = 
#ifndef __GTHREADS
  _S_single;
#elif defined _GLIBCXX_HAVE_ATOMIC_LOCK_POLICY
  _S_atomic;
#else
  _S_mutex;
#endif

  // NB: As this is used in libsupc++, need to only depend on
  // exception. No stdexception classes, no use of std::string.
  class __concurrence_lock_error : public std::exception
  {
  public:
    virtual char const*
    what() const throw()
    { return "__gnu_cxx::__concurrence_lock_error"; }
  };

  class __concurrence_unlock_error : public std::exception
  {
  public:
    virtual char const*
    what() const throw()
    { return "__gnu_cxx::__concurrence_unlock_error"; }
  };

  class __concurrence_broadcast_error : public std::exception
  {
  public:
    virtual char const*
    what() const throw()
    { return "__gnu_cxx::__concurrence_broadcast_error"; }
  };

  class __concurrence_wait_error : public std::exception
  {
  public:
    virtual char const*
    what() const throw()
    { return "__gnu_cxx::__concurrence_wait_error"; }
  };

  // Substitute for concurrence_error object in the case of -fno-exceptions.
  inline void
  __throw_concurrence_lock_error()
  { _GLIBCXX_THROW_OR_ABORT(__concurrence_lock_error()); }

  inline void
  __throw_concurrence_unlock_error()
  { _GLIBCXX_THROW_OR_ABORT(__concurrence_unlock_error()); }

#ifdef __GTHREAD_HAS_COND
  inline void
  __throw_concurrence_broadcast_error()
  { _GLIBCXX_THROW_OR_ABORT(__concurrence_broadcast_error()); }

  inline void
  __throw_concurrence_wait_error()
  { _GLIBCXX_THROW_OR_ABORT(__concurrence_wait_error()); }
#endif
 
  class __mutex 
  {
  private:
#if __GTHREADS && defined __GTHREAD_MUTEX_INIT
    __gthread_mutex_t _M_mutex = __GTHREAD_MUTEX_INIT;
#else
    __gthread_mutex_t _M_mutex;
#endif

    __mutex(const __mutex&);
    __mutex& operator=(const __mutex&);

  public:
    __mutex() 
    { 
#if __GTHREADS && ! defined __GTHREAD_MUTEX_INIT
      if (__gthread_active_p())
	__GTHREAD_MUTEX_INIT_FUNCTION(&_M_mutex);
#endif
    }

#if __GTHREADS && ! defined __GTHREAD_MUTEX_INIT
    ~__mutex() 
    { 
      if (__gthread_active_p())
	__gthread_mutex_destroy(&_M