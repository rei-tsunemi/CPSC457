// Numeric functions implementation -*- C++ -*-

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
 * Copyright (c) 1996,1997
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

/** @file bits/stl_numeric.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{numeric}
 */

#ifndef _STL_NUMERIC_H
#define _STL_NUMERIC_H 1

#include <bits/concept_check.h>
#include <debug/debug.h>
#include <bits/move.h> // For _GLIBCXX_MOVE


namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  /** @defgroup numeric_ops Generalized Numeric operations
   *  @ingroup algorithms
   */

#if __cplusplus >= 201103L
  /**
   *  @brief  Create a range of sequentially increasing values.
   *
   *  For each element in the range @p [first,last) assigns @p value and
   *  increments @p value as if by @p ++value.
   *
   *  @param  __first  Start of range.
   *  @param  __last  End of range.
   *  @param  __value  Starting value.
   *  @return  Nothing.
   *  @ingroup numeric_ops
   */
  template<typename _ForwardIterator, typename _Tp>
    void
    iota(_ForwardIterator __first, _ForwardIterator __last, _Tp __value)
    {
      // concept requirements
      __glibcxx_function_requires(_Mutable_ForwardIteratorConcept<
				  _ForwardIterator>)
      __glibcxx_function_requires(_ConvertibleConcept<_Tp,
	    typename iterator_traits<_ForwardIterator>::value_type>)
      __glibcxx_requires_valid_range(__first, __last);

      for (; __first != __last; ++__first)
	{
	  *__first = __value;
	  ++__value;
	}
    }
#endif

_GLIBCXX_END_NAMESPACE_VERSION

_GLIBCXX_BEGIN_NAMESPACE_ALGO

#if __cplusplus > 201703L
// _GLIBCXX_RESOLVE_LIB_DEFECTS
// DR 2055. std::move in std::accumulate and other algorithms
# define _GLIBCXX_MOVE_IF_20(_E) std::move(_E)
#else
# define _GLIBCXX_MOVE_IF_20(_E) _E
#endif

  /// @addtogroup numeric_ops
  /// @{

  /**
   *  @brief  Accumulate values in a range.
   *
   *  Accumulates the values in the range [first,last) using operator+().  The
   *  initial value is