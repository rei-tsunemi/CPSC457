// Debugging support implementation -*- C++ -*-

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

/** @file debug/functions.h
 *  This file is a GNU debug extension to the Standard C++ Library.
 */

#ifndef _GLIBCXX_DEBUG_FUNCTIONS_H
#define _GLIBCXX_DEBUG_FUNCTIONS_H 1

#include <bits/move.h>		// for __addressof
#include <bits/stl_function.h>	// for less

#if __cplusplus >= 201103L
# include <bits/stl_iterator.h>	// for __miter_base
# include <type_traits>		// for is_lvalue_reference and conditional.
#endif

#include <debug/helper_functions.h>
#include <debug/formatter.h>

namespace __gnu_debug
{
  template<typename _Sequence>
    struct _Insert_range_from_self_is_safe
    { enum { __value = 0 }; };

  template<typename _Sequence>
    struct _Is_contiguous_sequence : std::__false_type { };

  // An arbitrary iterator pointer is not singular.
  inline bool
  __check_singular_aux(const void*) { return false; }

  // We may have an iterator that derives from _Safe_iterator_base but isn't
  // a _Safe_iterator.
  template<typename _Iterator>
    inline bool
    __check_singular(const _Iterator& __x)
    { return __check_singular_aux(std::__addressof(__x)); }

  /** Non-NULL pointers are nonsingular. */
  template<typename _Tp>
    inline bool
    __check_singular(const _Tp* __ptr)
    { return __ptr == 0; }

  /* Checks that [first, last) is a valid range, and then returns
   * __first. This routine is useful when we can't use a separate
   * assertion statement because, e.g., we are in a constructor.
  */
  template<typename _InputIterator>
    inline _InputIterator
    __check_valid_range(const _InputIterator& __first,
			const _InputIterator& __last,
			const char* __file,
			unsigned int __line,
			const char* __function)
    {
      __glibcxx_check_valid_range_at(__first, __last,
				     __file, __line, __function);
      return __first;
    }

  /* Handle the case where __other is a pointer to _Sequence::value_type. */
  template<typename _Iterator, typename _Sequence, typename _Category>
    inline bool
    __foreign_iterator_aux4(
	const _Safe_iterator<_Iterator, _Sequence, _Category>& __it,
	const typename _Sequence::value_type* __other)
    {
      typedef const typename _Sequence::value_type* _PointerType;
      typedef std::less<_PointerType> _Less;
#if __cplusplus >= 201103L
      constexpr _Less __l{};
#else
      const _Less __l = _Less();
#endif
      const _Sequence* __seq = __it._M_get_sequence();
      const _PointerType __begin = std::__addressof(*__seq->_M_base().begin());
      const _PointerType __end = std::__addressof(*(__seq->_M_base().end()-1));

      // Check whether __other points within the contiguous storage.
      return __l(__other, __begin) || __l(__end, __other);
    }

  /* Fallback overload for when we can't tell, assume it is valid. */
  template<typename _Iterator, typename _Sequence, typename _Category>
    inline bool
    __foreign_iterator_aux4(
	const _Safe_iterator<_Iterator, _Sequence, _Category>&, ...)
    { return true; }

  /* Handle sequences with contiguous storage */
  template<typename _Iterator, ty