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

/** @file parallel/random_shuffle.h
 *  @brief Parallel implementation of std::random_shuffle().
 *  This file is a GNU parallel extension to the Standard C++ Library.
 */

// Written by Johannes Singler.

#ifndef _GLIBCXX_PARALLEL_RANDOM_SHUFFLE_H
#define _GLIBCXX_PARALLEL_RANDOM_SHUFFLE_H 1

#include <limits>
#include <bits/stl_numeric.h>
#include <parallel/parallel.h>
#include <parallel/random_number.h>

namespace __gnu_parallel
{
  /** @brief Type to hold the index of a bin.
    *
    *  Since many variables of this type are allocated, it should be
    *  chosen as small as possible.
    */
  typedef unsigned short _BinIndex;

  /** @brief Data known to every thread participating in
      __gnu_parallel::__parallel_random_shuffle(). */
  template<typename _RAIter>
    struct _DRandomShufflingGlobalData
    {
      typedef std::iterator_traits<_RAIter> _TraitsType;
      typedef typename _TraitsType::value_type _ValueType;
      typedef typename _TraitsType::difference_type _DifferenceType;

      /** @brief Begin iterator of the __source. */
      _RAIter& _M_source;

      /** @brief Temporary arrays for each thread. */
      _ValueType** _M_temporaries;

      /** @brief Two-dimensional array to hold the thread-bin distribution.
       *
       *  Dimensions (_M_num_threads + 1) __x (_M_num_bins + 1). */
      _DifferenceType** _M_dist;

      /** @brief Start indexes of the threads' __chunks. */
      _DifferenceType* _M_starts;

      /** @brief Number of the thread that will further process the
          corresponding bin. */
      _ThreadIndex* _M_bin_proc;

      /** @brief Number of bins to distribute to. */
      int _M_num_bins;

      /** @brief Number of bits needed to address the bins. */
      int _M_num_bits;

      /** @brief Constructor. */
      _DRandomShufflingGlobalData(_RAIter& __source)
      : _M_source(__source) { }
    };

  /** @brief Local data for a thread participating in
      __gnu_parallel::__parallel_random_shuffle().
    */
  template<typename _RAIter, typename _RandomNumberGenerator>
    struct _DRSSorterPU
    {
      /** @brief Number of threads participating in total. */
      int _M_num_threads;

      /** @brief Begin index for bins taken care of by this thread. */
      _BinIndex _M_bins_begin;

      /** @brief End index for bins taken care of by this thread. */
      _BinIndex __bins_end;

      /** @brief Random _M_seed for this thread. */
      uint32_t _M_seed;

      /** @brief Pointer to global data. */
      _DRandomShufflingGlobalData<_RAIter>* _M_sd;
    };

  /** @brief Generate a random number in @c [0,2^__logp).
    *  @param __logp Logarithm (basis 2) of the upper range __bound.
    *  @param __rng Random number generator to use.
    */
  template<typename _RandomNumberGenerator>
    inline int
    __random_number_pow2(int __logp, _RandomNumberGenerator& __rng)
    { return __rng.__genrand_bits(__logp); }

  /** @brief Random shuffle code executed by each thread.
    *  @param __pus Array of thread-local data records. */
  tem