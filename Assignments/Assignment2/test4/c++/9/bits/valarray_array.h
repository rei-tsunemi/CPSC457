// The template and inlines for the -*- C++ -*- internal _Array helper class.

// Copyright (C) 1997-2019 Free Software Foundation, Inc.
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

/** @file bits/valarray_array.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{valarray}
 */

// Written by Gabriel Dos Reis <Gabriel.Dos-Reis@DPTMaths.ENS-Cachan.Fr>

#ifndef _VALARRAY_ARRAY_H
#define _VALARRAY_ARRAY_H 1

#pragma GCC system_header

#include <bits/c++config.h>
#include <bits/cpp_type_traits.h>
#include <cstdlib>
#include <new>

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  //
  // Helper functions on raw pointers
  //

  // We get memory the old fashioned way
  template<typename _Tp>
    _Tp*
    __valarray_get_storage(size_t) __attribute__((__malloc__));

  template<typename _Tp>
    inline _Tp*
    __valarray_get_storage(size_t __n)
    { return static_cast<_Tp*>(operator new(__n * sizeof(_Tp))); }

  // Return memory to the system
  inline void
  __valarray_release_memory(void* __p)
  { operator delete(__p); }

  // Turn a raw-memory into an array of _Tp filled with _Tp()
  // This is required in 'valarray<T> v(n);'
  template<typename _Tp, bool>
    struct _Array_default_ctor
    {
      // Please note that this isn't exception safe.  But
      // valarrays aren't required to be exception safe.
      inline static void
      _S_do_it(_Tp* __b, _Tp* __e)
      {
	while (__b != __e)
	  new(__b++) _Tp();
      }
    };

  template<typename _Tp>
    struct _Array_default_ctor<_Tp, true>
    {
      // For fundamental types, it suffices to say 'memset()'
      inline static void
      _S_do_it(_Tp* __b, _Tp* __e)
      { __builtin_memset(__b, 0, (__e - __b) * sizeof(_Tp)); }
    };

  template<typename _Tp>
    inline void
    __valarray_default_construct(_Tp* __b, _Tp* __e)
    {
      _Array_default_ctor<_Tp, __is_scalar<_Tp>::__value>::_S_do_it(__b, __e);
    }

  // Turn a raw-memory into an array of _Tp filled with __t
  // This is the required in valarray<T> v(n, t).  Also
  // used in valarray<>::resize().
  template<typename _Tp, bool>
    struct _Array_init_ctor
    {
      // Please note that this isn't exception safe.  But
      // valarrays aren't required to be exception safe.
      inline static void
      _S_do_it(_Tp* __b, _Tp* __e, const _Tp __t)
      {
	while (__b != __e)
	  new(__b++) _Tp(__t);
      }
    };

  template<typename _Tp>
    struct _Array_init_ctor<_Tp, true>
    {
      inline static void
      _S_do_it(_Tp* __b, _Tp* __e, const _Tp __t)
      {
	while (__b != __e)
	  *__b++ = __t;
      }
    };

  template<typename _Tp>
    inline void
    __valarray_fill_construct(_Tp* __b, _Tp* __e, const _Tp __t)
    {
      _Array_init_ctor<_Tp, __is_trivial(_Tp)>::_S_do_it(__b, __e, __t);
    }

  //
  // copy-construct raw array [__o, *) from plain array [__b, __e)
  // We can't just say 'memcpy()'
  //
  template<typename _Tp, bool>
    struct _Array_copy_ctor
    {
      // Please note that this isn't exception safe.  But
      // valarrays aren't required to be exception