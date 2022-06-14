// vector<bool> specialization -*- C++ -*-

// Copyright (C) 2001-2019 Free Software Foundation, Inc.
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

/*
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Hewlett-Packard Company makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 *
 * Copyright (c) 1996-1999
 * Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 */

/** @file bits/stl_bvector.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{vector}
 */

#ifndef _STL_BVECTOR_H
#define _STL_BVECTOR_H 1

#if __cplusplus >= 201103L
#include <initializer_list>
#include <bits/functional_hash.h>
#endif

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION
_GLIBCXX_BEGIN_NAMESPACE_CONTAINER

  typedef unsigned long _Bit_type;
  enum { _S_word_bit = int(__CHAR_BIT__ * sizeof(_Bit_type)) };

  struct _Bit_reference
  {
    _Bit_type * _M_p;
    _Bit_type _M_mask;

    _Bit_reference(_Bit_type * __x, _Bit_type __y)
    : _M_p(__x), _M_mask(__y) { }

    _Bit_reference() _GLIBCXX_NOEXCEPT : _M_p(0), _M_mask(0) { }

#if __cplusplus >= 201103L
    _Bit_reference(const _Bit_reference&) = default;
#endif

    operator bool() const _GLIBCXX_NOEXCEPT
    { return !!(*_M_p & _M_mask); }

    _Bit_reference&
    operator=(bool __x) _GLIBCXX_NOEXCEPT
    {
      if (__x)
	*_M_p |= _M_mask;
      else
	*_M_p &= ~_M_mask;
      return *this;
    }

    _Bit_reference&
    operator=(const _Bit_reference& __x) _GLIBCXX_NOEXCEPT
    { return *this = bool(__x); }

    bool
    operator==(const _Bit_reference& __x) const
    { return bool(*this) == bool(__x); }

    bool
    operator<(const _Bit_reference& __x) const
    { return !bool(*this) && bool(__x); }

    void
    flip() _GLIBCXX_NOEXCEPT
    { *_M_p ^= _M_mask; }
  };

#if __cplusplus >= 201103L
  inline void
  swap(_Bit_reference __x, _Bit_reference __y) noexcept
  {
    bool __tmp = __x;
    __x = __y;
    __y = __tmp;
  }

  inline void
  swap(_Bit_reference __x, bool& __y) noexcept
  {
    bool __tmp = __x;
    __x = __y;
    __y = __tmp;
  }

  inline void
  swap(bool& __x, _Bit_reference __y) noexcept
  {
    bool __tmp = __x;
    __x = __y;
    __y = __tmp;
  }
#endif

  struct _Bit_iterator_base
  