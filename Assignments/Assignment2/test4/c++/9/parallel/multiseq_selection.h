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

/** @file parallel/multiseq_selection.h
 *  @brief Functions to find elements of a certain global __rank in
 *  multiple sorted sequences.  Also serves for splitting such
 *  sequence sets.
 *
 *  The algorithm description can be found in 
 *
 *  P. J. Varman, S. D. Scheufler, B. R. Iyer, and G. R. Ricard.
 *  Merging Multiple Lists on Hierarchical-Memory Multiprocessors.
 *  Journal of Parallel and Distributed Computing, 12(2):171-177, 1991.
 *
 *  This file is a GNU parallel extension to the Standard C++ Library.
 */

// Written by Johannes Singler.

#ifndef _GLIBCXX_PARALLEL_MULTISEQ_SELECTION_H
#define _GLIBCXX_PARALLEL_MULTISEQ_SELECTION_H 1

#include <vector>
#include <queue>

#include <bits/stl_algo.h>

namespace __gnu_parallel
{
  /** @brief Compare __a pair of types lexicographically, ascending. */
  template<typename _T1, typename _T2, typename _Compare>
    class _Lexicographic
    : public std::binary_function<std::pair<_T1, _T2>,
				  std::pair<_T1, _T2>, bool>
    {
    private:
      _Compare& _M_comp;

    public:
      _Lexicographic(_Compare& __comp) : _M_comp(__comp) { }

      bool
      operator()(const std::pair<_T1, _T2>& __p1,
                 const std::pair<_T1, _T2>& __p2) const
      {
        if (_M_comp(__p1.first, __p2.first))
          return true;

        if (_M_comp(__p2.first, __p1.first))
          return false;

        // Firsts are equal.
        return __p1.second < __p2.second;
      }
    };

  /** @brief Compare __a pair of types lexicographically, descending. */
  template<typename _T1, typename _T2, typename _Compare>
    class _LexicographicReverse : public std::binary_function<_T1, _T2, bool>
    {
    private:
      _Compare& _M_comp;

    public:
      _LexicographicReverse(_Compare& __comp) : _M_comp(__comp) { }

      bool
      operator()(const std::pair<_T1, _T2>& __p1,
                 const std::pair<_T1, _T2>& __p2) const
      {
        if (_M_comp(__p2.first, __p1.first))
          return true;

        if (_M_comp(__p1.first, __p2.first))
          return false;

        // Firsts are equal.
        return __p2.second < __p1.second;
      }
    };

  /** 
   *  @brief Splits several sorted sequences at a certain global __rank,
   *  resulting in a splitting point for each sequence.
   *  The sequences are passed via a sequence of random-access
   *  iterator pairs, none of the sequences may be empty.  If there
   *  are several equal elements across the split, the ones on the
   *  __left side will be chosen from sequences with smaller number.
   *  @param __begin_seqs Begin of the sequence of iterator pairs.
   *  @param __end_seqs End of the sequence of iterator pairs.
   *  @param __rank The global rank to partition at.
   *  @param __begin_offsets A random-access __sequence __begin where the
   *  __result will be stored in. Each element of the sequence is an
   *  iterator that points to the first element on the greater part of
   *  the respective __sequence.
   *  @param __comp The ordering functor, defaults to std::le