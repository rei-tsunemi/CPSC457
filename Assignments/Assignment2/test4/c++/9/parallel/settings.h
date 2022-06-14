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

/** @file parallel/settings.h
 *  @brief Runtime settings and tuning parameters, heuristics to decide
 *  whether to use parallelized algorithms.
 *  This file is a GNU parallel extension to the Standard C++ Library.
 *
 *  @section parallelization_decision 
 *  The decision whether to run an algorithm in parallel.
 *
 *  There are several ways the user can switch on and __off the parallel
 *  execution of an algorithm, both at compile- and run-time.
 *
 *  Only sequential execution can be forced at compile-time.  This
 *  reduces code size and protects code parts that have
 *  non-thread-safe side effects.
 *
 *  Ultimately, forcing parallel execution at compile-time makes
 *  sense.  Often, the sequential algorithm implementation is used as
 *  a subroutine, so no reduction in code size can be achieved.  Also,
 *  the machine the program is run on might have only one processor
 *  core, so to avoid overhead, the algorithm is executed
 *  sequentially.
 *
 *  To force sequential execution of an algorithm ultimately at
 *  compile-time, the user must add the tag
*  gnu_parallel::sequential_tag() to the end of the parameter list,
 *  e. g.
 *
 *  \code
 *  std::sort(__v.begin(), __v.end(), __gnu_parallel::sequential_tag());
 *  \endcode
 *
 *  This is compatible with all overloaded algorithm variants.  No
 *  additional code will be instantiated, at all.  The same holds for
 *  most algorithm calls with iterators not providing random access.
 *
 *  If the algorithm call is not forced to be executed sequentially
 *  at compile-time, the decision is made at run-time.
 *  The global variable __gnu_parallel::_Settings::algorithm_strategy
 *  is checked. _It is a tristate variable corresponding to:
 *
 *  a. force_sequential, meaning the sequential algorithm is executed.
*  b. force_parallel, meaning the parallel algorithm is executed.
*  c. heuristic
 *
 *  For heuristic, the parallel algorithm implementation is called
 *  only if the input size is sufficiently large.  For most
 *  algorithms, the input size is the (combined) length of the input
*  sequence(__s).  The threshold can be set by the user, individually
 *  for each algorithm.  The according variables are called
*  gnu_parallel::_Settings::[algorithm]_minimal_n .
 *
 *  For some of the algorithms, there are even more tuning options,
 *  e. g. the ability to choose from multiple algorithm variants.  See
 *  below for details.
 */

// Written by Johannes Singler and Felix Putze.

#ifndef _GLIBCXX_PARALLEL_SETTINGS_H
#define _GLIBCXX_PARALLEL_SETTINGS_H 1

#include <parallel/types.h>

/** 
  * @brief Determine at compile(?)-time if the parallel variant of an
  * algorithm should be called.
  * @param __c A condition that is convertible to bool that is overruled by
  * __gnu_parallel::_Settings::algorithm_strategy. Usually a decision
  * based on the input size.
  */
#define _GLIBCXX_PARALLEL_CONDITION(__c) \
  (__gnu_parallel::_Settings::get().algorithm_strategy \
    != __gnu_parallel::force_sequential \
  && ((__gnu_parallel::__ge