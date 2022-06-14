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

/** @file parallel/balanced_quicksort.h
 *  @brief Implementation of a dynamically load-balanced parallel quicksort.
 *
 *  It works in-place and needs only logarithmic extra memory.
 *  The algorithm is similar to the one proposed in
 *
 *  P. Tsigas and Y. Zhang.
 *  A simple, fast parallel implementation of quicksort and
 *  its performance evaluation on SUN enterprise 10000.
 *  In 11th Euromicro Conference on Parallel, Distributed and
 *  Network-Based Processing, page 372, 2003.
 *
 *  This file is a GNU parallel extension to the Standard C++ Library.
 */

// Written by Johannes Singler.

#ifndef _GLIBCXX_PARALLEL_BALANCED_QUICKSORT_H
#define _GLIBCXX_PARALLEL_BALANCED_QUICKSORT_H 1

#include <parallel/basic_iterator.h>
#include <bits/stl_algo.h>
#include <bits/stl_function.h>

#include <parallel/settings.h>
#include <parallel/partition.h>
#include <parallel/random_number.h>
#include <parallel/queue.h>

#if _GLIBCXX_PARALLEL_ASSERTIONS
#include <parallel/checkers.h>
#ifdef _GLIBCXX_HAVE_UNISTD_H
#include <unistd.h>
#endif
#endif

namespace __gnu_parallel
{
  /** @brief Information local to one thread in the parallel quicksort run. */
  template<typename _RAIter>
    struct _QSBThreadLocal
    {
      typedef std::iterator_traits<_RAIter> _TraitsType;
      typedef typename _TraitsType::difference_type _DifferenceType;

      /** @brief Continuous part of the sequence, described by an
      iterator pair. */
      typedef std::pair<_RAIter, _RAIter> _Piece;

      /** @brief Initial piece to work on. */
      _Piece _M_initial;

      /** @brief Work-stealing queue. */
      _RestrictedBoundedConcurrentQueue<_Piece> _M_leftover_parts;

      /** @brief Number of threads involved in this algorithm. */
      _ThreadIndex _M_num_threads;

      /** @brief Pointer to a counter of elements left over to sort. */
      volatile _DifferenceType* _M_elements_leftover;

      /** @brief The complete sequence to sort. */
      _Piece _M_global;

      /** @brief Constructor.
       *  @param __queue_size size of the work-stealing queue. */
      _QSBThreadLocal(int __queue_size) : _M_leftover_parts(__queue_size) { }
    };

  /** @brief Balanced quicksort divide step.
    *  @param __begin Begin iterator of subsequence.
    *  @param __end End iterator of subsequence.
    *  @param __comp Comparator.
    *  @param __num_threads Number of threads that are allowed to work on
    *  this part.
    *  @pre @c (__end-__begin)>=1 */
  template<typename _RAIter, typename _Compare>
    typename std::iterator_traits<_RAIter>::difference_type
    __qsb_divide(_RAIter __begin, _RAIter __end,
		 _Compare __comp, _ThreadIndex __num_threads)
    {
      _GLIBCXX_PARALLEL_ASSERT(__num_threads > 0);

      typedef std::iterator_traits<_RAIter> _TraitsType;
      typedef typename _TraitsType::value_type _ValueType;
      typedef typename _TraitsType::difference_type _DifferenceType;

      _RAIter __pivot_pos =
	__median_of_three_iterators(__begin, __begin + (__end - __begin) / 2,
				    __end  - 1, __comp)