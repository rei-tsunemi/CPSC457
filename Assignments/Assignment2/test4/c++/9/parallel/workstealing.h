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

/** @file parallel/workstealing.h
 *  @brief Parallelization of embarrassingly parallel execution by
 *  means of work-stealing.
 *
 *  Work stealing is described in
 *
 *  R. D. Blumofe and C. E. Leiserson.
 *  Scheduling multithreaded computations by work stealing.
 *  Journal of the ACM, 46(5):720-748, 1999.
 *
 *  This file is a GNU parallel extension to the Standard C++ Library.
 */

// Written by Felix Putze.

#ifndef _GLIBCXX_PARALLEL_WORKSTEALING_H
#define _GLIBCXX_PARALLEL_WORKSTEALING_H 1

#include <parallel/parallel.h>
#include <parallel/random_number.h>
#include <parallel/compatibility.h>

namespace __gnu_parallel
{

#define _GLIBCXX_JOB_VOLATILE volatile

  /** @brief One __job for a certain thread. */
  template<typename _DifferenceTp>
    struct _Job
    {
      typedef _DifferenceTp _DifferenceType;

      /** @brief First element.
       *
       *  Changed by owning and stealing thread. By stealing thread,
       *  always incremented. */
      _GLIBCXX_JOB_VOLATILE _DifferenceType _M_first;

      /** @brief Last element.
       *
       *  Changed by owning thread only. */
      _GLIBCXX_JOB_VOLATILE _DifferenceType _M_last;

      /** @brief Number of elements, i.e. @c _M_last-_M_first+1.
       *
       *  Changed by owning thread only. */
      _GLIBCXX_JOB_VOLATILE _DifferenceType _M_load;
    };

  /** @brief Work stealing algorithm for random access iterators.
    *
    *  Uses O(1) additional memory. Synchronization at job lists is
    *  done with atomic operations.
    *  @param __begin Begin iterator of element sequence.
    *  @param __end End iterator of element sequence.
    *  @param __op User-supplied functor (comparator, predicate, adding
    *  functor, ...).
    *  @param __f Functor to @a process an element with __op (depends on
    *  desired functionality, e. g. for std::for_each(), ...).
    *  @param __r Functor to @a add a single __result to the already
    *  processed elements (depends on functionality).
    *  @param __base Base value for reduction.
    *  @param __output Pointer to position where final result is written to
    *  @param __bound Maximum number of elements processed (e. g. for
    *  std::count_n()).
    *  @return User-supplied functor (that may contain a part of the result).
    */
  template<typename _RAIter,
           typename _Op,
           typename _Fu,
           typename _Red,
           typename _Result>
    _Op
    __for_each_template_random_access_workstealing(_RAIter __begin,
						   _RAIter __end, _Op __op,
						   _Fu& __f, _Red __r,
						   _Result __base,
						   _Result& __output,
      typename std::iterator_traits<_RAIter>::difference_type __bound)
    {
      _GLIBCXX_CALL(__end - __begin)

      typedef std::iterator_traits<_RAIter> _TraitsType;
      typedef typename _TraitsType::difference_type _DifferenceType;

      const _Settings& __s = _Settings::get();

      _DifferenceType __chunk_size =
          static_cast<_DifferenceType>(__s.workstealing_chunk_size);

      // How many jobs?
      _