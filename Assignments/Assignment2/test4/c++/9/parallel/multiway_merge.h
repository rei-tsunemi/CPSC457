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

/** @file parallel/multiway_merge.h
*  @brief Implementation of sequential and parallel multiway merge.
*
*  Explanations on the high-speed merging routines in the appendix of
*
*  P. Sanders.
*  Fast priority queues for cached memory.
*  ACM Journal of Experimental Algorithmics, 5, 2000.
*
*  This file is a GNU parallel extension to the Standard C++ Library.
*/

// Written by Johannes Singler and Manuel Holtgrewe.

#ifndef _GLIBCXX_PARALLEL_MULTIWAY_MERGE_H
#define _GLIBCXX_PARALLEL_MULTIWAY_MERGE_H

#include <vector>

#include <bits/stl_algo.h>
#include <parallel/features.h>
#include <parallel/parallel.h>
#include <parallel/losertree.h>
#include <parallel/multiseq_selection.h>
#if _GLIBCXX_PARALLEL_ASSERTIONS
#include <parallel/checkers.h>
#endif

/** @brief Length of a sequence described by a pair of iterators. */
#define _GLIBCXX_PARALLEL_LENGTH(__s) ((__s).second - (__s).first)

namespace __gnu_parallel
{
  template<typename _RAIter1, typename _RAIter2, typename _OutputIterator,
	   typename _DifferenceTp, typename _Compare>
    _OutputIterator
    __merge_advance(_RAIter1&, _RAIter1, _RAIter2&, _RAIter2,
		    _OutputIterator, _DifferenceTp, _Compare);

  /** @brief _Iterator wrapper supporting an implicit supremum at the end
   *         of the sequence, dominating all comparisons.
   *
   * The implicit supremum comes with a performance cost.
   *
   * Deriving from _RAIter is not possible since
   * _RAIter need not be a class.
   */
  template<typename _RAIter, typename _Compare>
    class _GuardedIterator
    {
    private:
      /** @brief Current iterator __position. */
      _RAIter _M_current;

      /** @brief End iterator of the sequence. */
      _RAIter _M_end;

      /** @brief _Compare. */
      _Compare& __comp;

    public:
      /** @brief Constructor. Sets iterator to beginning of sequence.
      *  @param __begin Begin iterator of sequence.
      *  @param __end End iterator of sequence.
      *  @param __comp Comparator provided for associated overloaded
      *  compare operators. */
      _GuardedIterator(_RAIter __begin, _RAIter __end, _Compare& __comp)
      : _M_current(__begin), _M_end(__end), __comp(__comp)
      { }

      /** @brief Pre-increment operator.
      *  @return This. */
      _GuardedIterator<_RAIter, _Compare>&
      operator++()
      {
	++_M_current;
	return *this;
      }

      /** @brief Dereference operator.
      *  @return Referenced element. */
      typename std::iterator_traits<_RAIter>::value_type&
      operator*()
      { return *_M_current; }

      /** @brief Convert to wrapped iterator.
      *  @return Wrapped iterator. */
      operator _RAIter()
      { return _M_current; }

      /** @brief Compare two elements referenced by guarded iterators.
       *  @param __bi1 First iterator.
       *  @param __bi2 Second iterator.
       *  @return @c true if less. */
      friend bool
      operator<(_GuardedIterator<_RAIter, _Compare>& __bi1,
		_GuardedIterator<_RAIter, _Compare>& __bi2)
      {
	if (__bi1._M_current == __bi