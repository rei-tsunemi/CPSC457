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

/** @file parallel/multiway_mergesort.h
 *  @brief Parallel multiway merge sort.
 *  This file is a GNU parallel extension to the Standard C++ Library.
 */

// Written by Johannes Singler.

#ifndef _GLIBCXX_PARALLEL_MULTIWAY_MERGESORT_H
#define _GLIBCXX_PARALLEL_MULTIWAY_MERGESORT_H 1

#include <vector>

#include <parallel/basic_iterator.h>
#include <bits/stl_algo.h>
#include <parallel/parallel.h>
#include <parallel/multiway_merge.h>

namespace __gnu_parallel
{
  /** @brief Subsequence description. */
  template<typename _DifferenceTp>
    struct _Piece
    {
      typedef _DifferenceTp _DifferenceType;

      /** @brief Begin of subsequence. */
      _DifferenceType _M_begin;

      /** @brief End of subsequence. */
      _DifferenceType _M_end;
    };

  /** @brief Data accessed by all threads.
   *
   *  PMWMS = parallel multiway mergesort */
  template<typename _RAIter>
    struct _PMWMSSortingData
    {
      typedef std::iterator_traits<_RAIter> _TraitsType;
      typedef typename _TraitsType::value_type _ValueType;
      typedef typename _TraitsType::difference_type _DifferenceType;

      /** @brief Number of threads involved. */
      _ThreadIndex _M_num_threads;

      /** @brief Input __begin. */
      _RAIter _M_source;

      /** @brief Start indices, per thread. */
      _DifferenceType* _M_starts;

      /** @brief Storage in which to sort. */
      _ValueType** _M_temporary;

      /** @brief Samples. */
      _ValueType* _M_samples;

      /** @brief Offsets to add to the found positions. */
      _DifferenceType* _M_offsets;

      /** @brief Pieces of data to merge @c [thread][__sequence] */
      std::vector<_Piece<_DifferenceType> >* _M_pieces;
  };

  /**
   *  @brief Select _M_samples from a sequence.
   *  @param __sd Pointer to algorithm data. _Result will be placed in
   *  @c __sd->_M_samples.
   *  @param __num_samples Number of _M_samples to select.
   */
  template<typename _RAIter, typename _DifferenceTp>
    void
    __determine_samples(_PMWMSSortingData<_RAIter>* __sd,
			_DifferenceTp __num_samples)
    {
      typedef std::iterator_traits<_RAIter> _TraitsType;
      typedef typename _TraitsType::value_type _ValueType;
      typedef _DifferenceTp _DifferenceType;

      _ThreadIndex __iam = omp_get_thread_num();

      _DifferenceType* __es = new _DifferenceType[__num_samples + 2];

      __equally_split(__sd->_M_starts[__iam + 1] - __sd->_M_starts[__iam], 
		      __num_samples + 1, __es);

      for (_DifferenceType __i = 0; __i < __num_samples; ++__i)
	::new(&(__sd->_M_samples[__iam * __num_samples + __i]))
	    _ValueType(__sd->_M_source[__sd->_M_starts[__iam]
				       + __es[__i + 1]]);

      delete[] __es;
    }

  /** @brief Split consistently. */
  template<bool __exact, typename _RAIter,
	   typename _Compare, typename _SortingPlacesIterator>
    struct _SplitConsistently
    { };

  /** @brief Split by exact splitting. */
  template<typename _RAIter, typename _Compare,
	   typename _SortingPlacesIterator>
    struct _SplitConsistently<true