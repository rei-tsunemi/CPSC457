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

/** @file parallel/base.h
 *  @brief Sequential helper functions.
 *  This file is a GNU parallel extension to the Standard C++ Library.
 */

// Written by Johannes Singler.

#ifndef _GLIBCXX_PARALLEL_BASE_H
#define _GLIBCXX_PARALLEL_BASE_H 1

#include <bits/c++config.h>
#include <bits/stl_function.h>
#include <omp.h>
#include <parallel/features.h>
#include <parallel/basic_iterator.h>
#include <parallel/parallel.h>

// Parallel mode namespaces.

/**
 * @namespace std::__parallel
 * @brief GNU parallel code, replaces standard behavior with parallel behavior.
 */
namespace std _GLIBCXX_VISIBILITY(default) 
{ 
  namespace __parallel { } 
}

/**
 * @namespace __gnu_parallel
 * @brief GNU parallel code for public use.
 */
namespace __gnu_parallel
{
  // Import all the parallel versions of components in namespace std.
  using namespace std::__parallel;
}

/**
 * @namespace __gnu_sequential
 * @brief GNU sequential classes for public use.
 */
namespace __gnu_sequential 
{ 
  // Import whatever is the serial version.
#ifdef _GLIBCXX_PARALLEL
  using namespace std::_GLIBCXX_STD_A;
#else
  using namespace std;
#endif   
}


namespace __gnu_parallel
{
  // NB: Including this file cannot produce (unresolved) symbols from
  // the OpenMP runtime unless the parallel mode is actually invoked
  // and active, which imples that the OpenMP runtime is actually
  // going to be linked in.
  inline _ThreadIndex
  __get_max_threads() 
  { 
    _ThreadIndex __i = omp_get_max_threads();
    return __i > 1 ? __i : 1; 
  }


  inline bool 
  __is_parallel(const _Parallelism __p) { return __p != sequential; }


  /** @brief Calculates the rounded-down logarithm of @c __n for base 2.
   *  @param __n Argument.
   *  @return Returns 0 for any argument <1.
   */
  template<typename _Size>
    inline _Size
    __rd_log2(_Size __n)
    {
      _Size __k;
      for (__k = 0; __n > 1; __n >>= 1)
        ++__k;
      return __k;
    }

  /** @brief Encode two integers into one gnu_parallel::_CASable.
   *  @param __a First integer, to be encoded in the most-significant @c
   *  _CASable_bits/2 bits.
   *  @param __b Second integer, to be encoded in the least-significant
   *  @c _CASable_bits/2 bits.
   *  @return value encoding @c __a and @c __b.
   *  @see __decode2
   */
  inline _CASable
  __encode2(int __a, int __b)     //must all be non-negative, actually
  {
    return (((_CASable)__a) << (_CASable_bits / 2)) | (((_CASable)__b) << 0);
  }

  /** @brief Decode two integers from one gnu_parallel::_CASable.
   *  @param __x __gnu_parallel::_CASable to decode integers from.
   *  @param __a First integer, to be decoded from the most-significant
   *  @c _CASable_bits/2 bits of @c __x.
   *  @param __b Second integer, to be encoded in the least-significant
   *  @c _CASable_bits/2 bits of @c __x.
   *  @see __encode2
   */
  inline void
  __decode2(_CASable __x, int& __a, int& __b)
  {
    __a = (int)((__x >> (_CASable_bits / 2)) & _CASable_mask);
    __b = (int)((__x >>               0 ) & _CASable_mask);
  }

  //ne