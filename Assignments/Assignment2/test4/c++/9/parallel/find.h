// -*- C++ -*-

// Copyright (C) 2007-2019 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License as published by the Free Software
// Foundation; either version 3, or (at your option) any later
// version.

// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

/** @file parallel/find.h
 *  @brief Parallel implementation base for std::find(), std::equal()
 *  and related functions.
 *  This file is a GNU parallel extension to the Standard C++ Library.
 */

// Written by Felix Putze and Johannes Singler.

#ifndef _GLIBCXX_PARALLEL_FIND_H
#define _GLIBCXX_PARALLEL_FIND_H 1

#include <bits/stl_algobase.h>

#include <parallel/features.h>
#include <parallel/parallel.h>
#include <parallel/compatibility.h>
#include <parallel/equally_split.h>

namespace __gnu_parallel
{
  /**
   *  @brief Parallel std::find, switch for different algorithms.
   *  @param __begin1 Begin iterator of first sequence.
   *  @param __end1 End iterator of first sequence.
   *  @param __begin2 Begin iterator of second sequence. Must have same
   *  length as first sequence.
   *  @param __pred Find predicate.
   *  @param __selector _Functionality (e. g. std::find_if(), std::equal(),...)
   *  @return Place of finding in both sequences.
   */
  template<typename _RAIter1,
	   typename _RAIter2,
	   typename _Pred,
           typename _Selector>
    inline std::pair<_RAIter1, _RAIter2>
    __find_template(_RAIter1 __begin1, _RAIter1 __end1,
		    _RAIter2 __begin2, _Pred __pred, _Selector __selector)
    {
      switch (_Settings::get().find_algorithm)
	{
	case GROWING_BLOCKS:
          return __find_template(__begin1, __end1, __begin2, __pred,
				 __selector, growing_blocks_tag());
	case CONSTANT_SIZE_BLOCKS:
          return __find_template(__begin1, __end1, __begin2, __pred,
				 __selector, constant_size_blocks_tag());
	case EQUAL_SPLIT:
          return __find_template(__begin1, __end1, __begin2, __pred,
				 __selector, equal_split_tag());
	default:
          _GLIBCXX_PARALLEL_ASSERT(false);
          return std::make_pair(__begin1, __begin2);
	}
    }

#if _GLIBCXX_FIND_EQUAL_SPLIT

  /**
   *  @brief Parallel std::find, equal splitting variant.
   *  @param __begin1 Begin iterator of first sequence.
   *  @param __end1 End iterator of first sequence.
   *  @param __begin2 Begin iterator of second sequence. Second __sequence
   *  must have same length as first sequence.
   *  @param __pred Find predicate.
   *  @param __selector _Functionality (e. g. std::find_if(), std::equal(),...)
   *  @return Place of finding in both sequences.
   */
  template<typename _RAIter1,
           typename _RAIter2,
           typename _Pred,
           typename _Selector>
    std::pair<_RAIter1, _RAIter2>
    __find_template(_RAIter1 __begin1, _RAIter1 __end1,
		    _RAIter2 __begin2, _Pred __pred,
		    _Selector __selector, equal_split_tag)
    {
      _GLIBCXX_CALL(__end1 - __begin1)

      typedef std::iterator_traits<_RAIter1> _TraitsType;
      typedef typename _TraitsType::difference_type _DifferenceType;
      typedef typename _TraitsType::value_type _ValueType;

      _DifferenceType __length = __end1 - __begin1;
      _DifferenceType __result = __length;
      _DifferenceType* __borders;

      omp_lock_t __result_lock;
      omp_init_lock(&__result_lock);

      _ThreadIndex