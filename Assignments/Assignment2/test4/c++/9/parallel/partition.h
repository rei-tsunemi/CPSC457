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

/** @file parallel/partition.h
 *  @brief Parallel implementation of std::partition(),
 *  std::nth_element(), and std::partial_sort().
 *  This file is a GNU parallel extension to the Standard C++ Library.
 */

// Written by Johannes Singler and Felix Putze.

#ifndef _GLIBCXX_PARALLEL_PARTITION_H
#define _GLIBCXX_PARALLEL_PARTITION_H 1

#include <parallel/basic_iterator.h>
#include <parallel/sort.h>
#include <parallel/random_number.h>
#include <bits/stl_algo.h>
#include <parallel/parallel.h>

/** @brief Decide whether to declare certain variables volatile. */
#define _GLIBCXX_VOLATILE volatile

namespace __gnu_parallel
{
  /** @brief Parallel implementation of std::partition.
    *  @param __begin Begin iterator of input sequence to split.
    *  @param __end End iterator of input sequence to split.
    *  @param __pred Partition predicate, possibly including some kind
    *         of pivot.
    *  @param __num_threads Maximum number of threads to use for this task.
    *  @return Number of elements not fulfilling the predicate. */
  template<typename _RAIter, typename _Predicate>
    typename std::iterator_traits<_RAIter>::difference_type
    __parallel_partition(_RAIter __begin, _RAIter __end,
			 _Predicate __pred, _ThreadIndex __num_threads)
    {
      typedef std::iterator_traits<_RAIter> _TraitsType;
      typedef typename _TraitsType::value_type _ValueType;
      typedef typename _TraitsType::difference_type _DifferenceType;

      _DifferenceType __n = __end - __begin;

      _GLIBCXX_CALL(__n)

      const _Settings& __s = _Settings::get();

      // shared
      _GLIBCXX_VOLATILE _DifferenceType __left = 0, __right = __n - 1,
                                        __dist = __n,
                                        __leftover_left, __leftover_right,
                                        __leftnew, __rightnew;

      // just 0 or 1, but int to allow atomic operations
      int* __reserved_left = 0, * __reserved_right = 0;

      _DifferenceType __chunk_size = __s.partition_chunk_size;

      //at least two chunks per thread
      if (__dist >= 2 * __num_threads * __chunk_size)
#       pragma omp parallel num_threads(__num_threads)
	{
#         pragma omp single
	  {
	    __num_threads = omp_get_num_threads();
	    __reserved_left = new int[__num_threads];
	    __reserved_right = new int[__num_threads];

	    if (__s.partition_chunk_share > 0.0)
	      __chunk_size = std::max<_DifferenceType>
		(__s.partition_chunk_size, (double)__n 
		 * __s.partition_chunk_share / (double)__num_threads);
	    else
	      __chunk_size = __s.partition_chunk_size;
	  }

	  while (__dist >= 2 * __num_threads * __chunk_size)
	    {
#             pragma omp single
	      {
		_DifferenceType __num_chunks = __dist / __chunk_size;

		for (_ThreadIndex __r = 0; __r < __num_threads; ++__r)
		  {
		    __reserved_left [__r] = 0; // false
		    __reserved_right[__r] = 0; // false
		  }
		__leftover_left = 0;
		__leftover_right = 0;
	      } //implicit barrier

	      // Private.
	      _Difference