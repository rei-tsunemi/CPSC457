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

/** @file parallel/algo.h
 *  @brief Parallel STL function calls corresponding to the stl_algo.h header.
 *
 *  The functions defined here mainly do case switches and
 *  call the actual parallelized versions in other files.
 *  Inlining policy: Functions that basically only contain one function call,
 *  are declared inline.
 *  This file is a GNU parallel extension to the Standard C++ Library.
 */

// Written by Johannes Singler and Felix Putze.

#ifndef _GLIBCXX_PARALLEL_ALGO_H
#define _GLIBCXX_PARALLEL_ALGO_H 1

#include <parallel/algorithmfwd.h>
#include <bits/stl_algobase.h>
#include <bits/stl_algo.h>
#include <parallel/iterator.h>
#include <parallel/base.h>
#include <parallel/sort.h>
#include <parallel/workstealing.h>
#include <parallel/par_loop.h>
#include <parallel/omp_loop.h>
#include <parallel/omp_loop_static.h>
#include <parallel/for_each_selectors.h>
#include <parallel/for_each.h>
#include <parallel/find.h>
#include <parallel/find_selectors.h>
#include <parallel/search.h>
#include <parallel/random_shuffle.h>
#include <parallel/partition.h>
#include <parallel/merge.h>
#include <parallel/unique_copy.h>
#include <parallel/set_operations.h>

namespace std _GLIBCXX_VISIBILITY(default)
{
namespace __parallel
{
  // Sequential fallback
  template<typename _IIter, typename _Function>
    inline _Function
    for_each(_IIter __begin, _IIter __end, _Function __f,
	     __gnu_parallel::sequential_tag)
    { return _GLIBCXX_STD_A::for_each(__begin, __end, __f); }

  // Sequential fallback for input iterator case
  template<typename _IIter, typename _Function, typename _IteratorTag>
    inline _Function
    __for_each_switch(_IIter __begin, _IIter __end, _Function __f,
		      _IteratorTag)
    { return for_each(__begin, __end, __f, __gnu_parallel::sequential_tag()); }

  // Parallel algorithm for random access iterators
  template<typename _RAIter, typename _Function>
    _Function
    __for_each_switch(_RAIter __begin, _RAIter __end,
		      _Function __f, random_access_iterator_tag,
		      __gnu_parallel::_Parallelism __parallelism_tag)
    {
      if (_GLIBCXX_PARALLEL_CONDITION(
	    static_cast<__gnu_parallel::_SequenceIndex>(__end - __begin)
	    >= __gnu_parallel::_Settings::get().for_each_minimal_n
	    && __gnu_parallel::__is_parallel(__parallelism_tag)))
	{
	  bool __dummy;
	  __gnu_parallel::__for_each_selector<_RAIter> __functionality;

	  return __gnu_parallel::
	    __for_each_template_random_access(
	      __begin, __end, __f, __functionality,
	      __gnu_parallel::_DummyReduct(), true, __dummy, -1,
	      __parallelism_tag);
	}
      else
	return for_each(__begin, __end, __f, __gnu_parallel::sequential_tag());
    }

  // Public interface
  template<typename _Iterator, typename _Function>
    inline _Function
    for_each(_Iterator __begin, _Iterator __end, _Function __f,
	     __gnu_parallel::_Parallelism __parallelism_tag)
    {
      return __for_each_switch(__begin, __end, __f,
			       std::__iterator_category(__begin),
			       __parallelism_tag);
    }

  te