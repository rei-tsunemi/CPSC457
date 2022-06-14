// Deque implementation -*- C++ -*-

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
 * Copyright (c) 1997
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

/** @file bits/stl_deque.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{deque}
 */

#ifndef _STL_DEQUE_H
#define _STL_DEQUE_H 1

#include <bits/concept_check.h>
#include <bits/stl_iterator_base_types.h>
#include <bits/stl_iterator_base_funcs.h>
#if __cplusplus >= 201103L
#include <initializer_list>
#include <bits/stl_uninitialized.h> // for __is_bitwise_relocatable
#endif

#include <debug/assertions.h>

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION
_GLIBCXX_BEGIN_NAMESPACE_CONTAINER

  /**
   *  @brief This function controls the size of memory nodes.
   *  @param  __size  The size of an element.
   *  @return   The number (not byte size) of elements per node.
   *
   *  This function started off as a compiler kludge from SGI, but
   *  seems to be a useful wrapper around a repeated constant
   *  expression.  The @b 512 is tunable (and no other code needs to
   *  change), but no investigation has been done since inheriting the
   *  SGI code.  Touch _GLIBCXX_DEQUE_BUF_SIZE only if you know what
   *  you are doing, however: changing it breaks the binary
   *  compatibility!!
  */

#ifndef _GLIBCXX_DEQUE_BUF_SIZE
#define _GLIBCXX_DEQUE_BUF_SIZE 512
#endif

  _GLIBCXX_CONSTEXPR inline size_t
  __deque_buf_size(size_t __size)
  { return (__size < _GLIBCXX_DEQUE_BUF_SIZE
	    ? size_t(_GLIBCXX_DEQUE_BUF_SIZE / __size) : size_t(1)); }


  /**
   *  @brief A deque::iterator.
   *
   *  Quite a bit of intelligence here.  Much of the functionality of
   *  deque is actually passed off to this class.  A deque holds two
   *  of these internally, marking its valid range.  Access to
   *  elements is done as offsets of either of those two, relying on
   *  operator overloading in this class.
   *
   *  All the functions are op overloads except for _M_set_node.
  */
  template<typename _Tp