// <algorithm> Forward declarations  -*- C++ -*-

// Copyright (C) 2007-2019 Free Software Foundation, Inc.
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

/** @file bits/algorithmfwd.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{algorithm}
 */

#ifndef _GLIBCXX_ALGORITHMFWD_H
#define _GLIBCXX_ALGORITHMFWD_H 1

#pragma GCC system_header

#include <bits/c++config.h>
#include <bits/stl_pair.h>
#include <bits/stl_iterator_base_types.h>
#if __cplusplus >= 201103L
#include <initializer_list>
#endif

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  /*
    adjacent_find
    all_of (C++11)
    any_of (C++11)
    binary_search
    clamp (C++17)
    copy
    copy_backward
    copy_if (C++11)
    copy_n (C++11)
    count
    count_if
    equal
    equal_range
    fill
    fill_n
    find
    find_end
    find_first_of
    find_if
    find_if_not (C++11)
    for_each
    generate
    generate_n
    includes
    inplace_merge
    is_heap (C++11)
    is_heap_until (C++11)
    is_partitioned (C++11)
    is_sorted (C++11)
    is_sorted_until (C++11)
    iter_swap
    lexicographical_compare
    lower_bound
    make_heap
    max
    max_element
    merge
    min
    min_element
    minmax (C++11)
    minmax_element (C++11)
    mismatch
    next_permutation
    none_of (C++11)
    nth_element
    partial_sort
    partial_sort_copy
    partition
    partition_copy (C++11)
    partition_point (C++11)
    pop_heap
    prev_permutation
    push_heap
    random_shuffle
    remove
    remove_copy
    remove_copy_if
    remove_if
    replace
    replace_copy
    replace_copy_if
    replace_if
    reverse
    reverse_copy
    rotate
    rotate_copy
    search
    search_n
    set_difference
    set_intersection
    set_symmetric_difference
    set_union
    shuffle (C++11)
    sort
    sort_heap
    stable_partition
    stable_sort
    swap
    swap_ranges
    transform
    unique
    unique_copy
    upper_bound
  */

  /**
   * @defgroup algorithms Algorithms
   *
   * Components for performing algorithmic operations. Includes
   * non-modifying sequence, modifying (mutating) sequence, sorting,
   * searching, merge, partition, heap, set, minima, maxima, and
   * permutation operations.
   */

  /**
   * @defgroup mutating_algorithms Mutating
   * @ingroup algorithms
   */

  /**
   * @defgroup non_mutating_algorithms Non-Mutating
   * @ingroup algorithms
   */

  /**
   * @defgroup sorting_algorithms Sorting
   * @ingroup algorithms
   */

  /**
   * @defgroup set_algorithms Set Operations
   * @ingroup sorting_algorithms
   *
   * These algorithms are common set operations performed on sequences
   * that are already sorted. The number of comparisons will be
   * linear.
   */

  /**
   * @defgroup binary_search_algorithms Binary Search
   * @ingroup sorting_algorithms
   *
   * These algorithms are variations of a classic binary search, and
   * all assume that the sequence being searched is already sorted.
   *
   * The number of comparisons will be logarithmic (and as few as
   * possible).  The number of steps thro