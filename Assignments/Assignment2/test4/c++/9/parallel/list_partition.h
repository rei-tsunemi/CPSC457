// -*- C++ -*-

// Copyright (C) 2007-2019 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute __it and/or modify __it under the terms
// of the GNU General Public License as published by the Free Software
// Foundation; either version 3, or (at your option) any later
// version.

// This library is distributed in the hope that __it will be useful, but
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

/** @file parallel/list_partition.h
 *  @brief _Functionality to split __sequence referenced by only input
 *  iterators.
 *  This file is a GNU parallel extension to the Standard C++ Library.
 */

// Written by Leonor Frias Moya and Johannes Singler.

#ifndef _GLIBCXX_PARALLEL_LIST_PARTITION_H
#define _GLIBCXX_PARALLEL_LIST_PARTITION_H 1

#include <parallel/parallel.h>
#include <vector>

namespace __gnu_parallel
{
  /** @brief Shrinks and doubles the ranges.
   *  @param __os_starts Start positions worked on (oversampled).
   *  @param __count_to_two Counts up to 2.
   *  @param __range_length Current length of a chunk.
   *  @param __make_twice Whether the @c __os_starts is allowed to be
   *  grown or not
   */
  template<typename _IIter>
    void
    __shrink_and_double(std::vector<_IIter>& __os_starts,
			size_t& __count_to_two, size_t& __range_length,
			const bool __make_twice)
    {
      ++__count_to_two;
      if (!__make_twice || __count_to_two < 2)
        __shrink(__os_starts, __count_to_two, __range_length);
      else
        {
          __os_starts.resize((__os_starts.size() - 1) * 2 + 1);
          __count_to_two = 0;
        }
    }

  /** @brief Combines two ranges into one and thus halves the number of ranges.
   *  @param __os_starts Start positions worked on (oversampled).
   *  @param __count_to_two Counts up to 2.
   *  @param __range_length Current length of a chunk. */
  template<typename _IIter>
    void
    __shrink(std::vector<_IIter>& __os_starts, size_t& __count_to_two,
	     size_t& __range_length)
    {
      for (typename std::vector<_IIter>::size_type __i = 0;
           __i <= (__os_starts.size() / 2); ++__i)
        __os_starts[__i] = __os_starts[__i * 2];
      __range_length *= 2;
    }

  /** @brief Splits a sequence given by input iterators into parts of
   * almost equal size
   *
   *  The function needs only one pass over the sequence.
   *  @param __begin Begin iterator of input sequence.
   *  @param __end End iterator of input sequence.
   *  @param __starts Start iterators for the resulting parts, dimension
   *  @c __num_parts+1. For convenience, @c __starts @c [__num_parts]
   *  contains the end iterator of the sequence.
   *  @param __lengths Length of the resulting parts.
   *  @param __num_parts Number of parts to split the sequence into.
   *  @param __f Functor to be applied to each element by traversing __it
   *  @param __oversampling Oversampling factor. If 0, then the
   *  partitions will differ in at most 
   *  \f$\sqrt{\mathrm{end} - \mathrm{begin}}\f$ 
   *  elements. Otherwise, the ratio between the
   *  longest and the shortest part is bounded by
   *  \f$1/(\mathrm{oversampling} \cdot \mathrm{num\_parts})\f$
   *  @return Length of the whole sequence.
   */
  template<typename _IIter, typename _FunctorType>
    size_t
    list_partition(const _IIter __begin, const _IIter __end,
                   _IIter* __starts, size_t* __lengths, const int __num_parts,
                   _FunctorType& __f, int __oversampling 