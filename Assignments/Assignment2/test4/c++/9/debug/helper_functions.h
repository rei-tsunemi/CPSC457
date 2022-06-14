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

/** @file debug/helper_functions.h
 *  This file is a GNU debug extension to the Standard C++ Library.
 */

#ifndef _GLIBCXX_DEBUG_HELPER_FUNCTIONS_H
#define _GLIBCXX_DEBUG_HELPER_FUNCTIONS_H 1

#include <bits/stl_iterator_base_types.h>	// for iterator_traits,
						// categories and _Iter_base
#include <bits/cpp_type_traits.h>		// for __is_integer

#include <bits/stl_pair.h>			// for pair

namespace __gnu_debug
{
  template<typename _Iterator, typename _Sequence, typename _Category>
    class _Safe_iterator;

#if __cplusplus >= 201103L
  template<typename _Iterator, typename _Sequence>
    class _Safe_local_iterator;
#endif

  /** The precision to which we can calculate the distance between
   *  two iterators.
   */
  enum _Distance_precision
    {
      __dp_none,	// Not even an iterator type
      __dp_equality,	//< Can compare iterator equality, only
      __dp_sign,	//< Can determine equality and ordering
      __dp_exact	//< Can determine distance precisely
    };

  template<typename _Iterator,
	   typename = typename std::__is_integer<_Iterator>::__type>
    struct _Distance_traits
    {
    private:
      typedef
      typename std::iterator_traits<_Iterator>::difference_type _ItDiffType;

      template<typename _DiffType,
	       typename = typename std::__is_void<_DiffType>::__type>
	struct _DiffTraits
	{ typedef _DiffType __type; };

      template<typename _DiffType>
	struct _DiffTraits<_DiffType, std::__true_type>
	{ typedef std::ptrdiff_t __type; };

      typedef typename _DiffTraits<_ItDiffType>::__type _DiffType;

    public:
      typedef std::pair<_DiffType, _Distance_precision> __type;
    };

  template<typename _Integral>
    struct _Distance_traits<_Integral, std::__true_type>
    { typedef std::pair<std::ptrdiff_t, _Distance_precision> __type; };

  /** Determine the distance between two iterators with some known
   *	precision.
  */
  template<typename _Iterator>
    inline typename _Distance_traits<_Iterator>::__type
    __get_distance(_Iterator __lhs, _Iterator __rhs,
		   std::random_access_iterator_tag)
    { return std::make_pair(__rhs - __lhs, __dp_exact); }

  template<typename _Iterator>
    inline typename _Distance_traits<_Iterator>::__type
    __get_distance(_Iterator __lhs, _Iterator __rhs,
		   std::input_iterator_tag)
    {
      if (__lhs == __rhs)
	return std::make_pair(0, __dp_exact);

      return std::make_pair(1, __dp_equality);
    }

  template<typename _Iterator>
    inline typename _Distance_traits<_Iterator>::__type
    __get_distance(_Iterator __lhs, _Iterator __rhs)
    { return __get_distance(__lhs, __rhs, std::__iterator_category(__lhs)); }

  /** We say that integral types for a valid range, and defer to other
   *  routines to realize what to do with integral types instead of
   *  iterators.
  */
  template<typename _Integral>
    inline bool
    __valid_range_aux(_Integral, _Integral,
		      typename _Distance_traits<_Integral>::__type& __dist,
		      std::__true_type)
    {
      __dist = s