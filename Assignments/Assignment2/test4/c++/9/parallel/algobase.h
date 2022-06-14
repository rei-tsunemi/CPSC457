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

/** @file parallel/algobase.h
 *  @brief Parallel STL function calls corresponding to the
 *  stl_algobase.h header.  The functions defined here mainly do case
 *  switches and call the actual parallelized versions in other files.
 *  Inlining policy: Functions that basically only contain one
 *  function call, are declared inline.
 *  This file is a GNU parallel extension to the Standard C++ Library.
 */

// Written by Johannes Singler and Felix Putze.

#ifndef _GLIBCXX_PARALLEL_ALGOBASE_H
#define _GLIBCXX_PARALLEL_ALGOBASE_H 1

#include <bits/stl_algobase.h>
#include <parallel/base.h>
#include <parallel/algorithmfwd.h>
#include <parallel/find.h>
#include <parallel/find_selectors.h>

namespace std _GLIBCXX_VISIBILITY(default)
{
namespace __parallel
{
  // NB: equal and lexicographical_compare require mismatch.

  // Sequential fallback
  template<typename _IIter1, typename _IIter2>
    inline pair<_IIter1, _IIter2>
    mismatch(_IIter1 __begin1, _IIter1 __end1, _IIter2 __begin2,
             __gnu_parallel::sequential_tag)
    { return _GLIBCXX_STD_A::mismatch(__begin1, __end1, __begin2); }

  // Sequential fallback
  template<typename _IIter1, typename _IIter2, typename _Predicate>
    inline pair<_IIter1, _IIter2>
    mismatch(_IIter1 __begin1, _IIter1 __end1, _IIter2 __begin2,
             _Predicate __pred, __gnu_parallel::sequential_tag)
    { return _GLIBCXX_STD_A::mismatch(__begin1, __end1, __begin2, __pred); }

  // Sequential fallback for input iterator case
  template<typename _IIter1, typename _IIter2,
           typename _Predicate, typename _IteratorTag1, typename _IteratorTag2>
    inline pair<_IIter1, _IIter2>
    __mismatch_switch(_IIter1 __begin1, _IIter1 __end1, _IIter2 __begin2,
                      _Predicate __pred, _IteratorTag1, _IteratorTag2)
    { return _GLIBCXX_STD_A::mismatch(__begin1, __end1, __begin2, __pred); }

  // Parallel mismatch for random access iterators
  template<typename _RAIter1, typename _RAIter2, typename _Predicate>
    pair<_RAIter1, _RAIter2>
    __mismatch_switch(_RAIter1 __begin1, _RAIter1 __end1,
                      _RAIter2 __begin2, _Predicate __pred, 
                      random_access_iterator_tag, random_access_iterator_tag)
    {
      if (_GLIBCXX_PARALLEL_CONDITION(true))
        {
          _RAIter1 __res =
            __gnu_parallel::__find_template(__begin1, __end1, __begin2, __pred,
                                            __gnu_parallel::
                                            __mismatch_selector()).first;
          return make_pair(__res , __begin2 + (__res - __begin1));
        }
      else
        return _GLIBCXX_STD_A::mismatch(__begin1, __end1, __begin2, __pred);
    }

  // Public interface
  template<typename _IIter1, typename _IIter2>
    inline pair<_IIter1, _IIter2>
    mismatch(_IIter1 __begin1, _IIter1 __end1, _IIter2 __begin2)
    {
      typedef __gnu_parallel::_EqualTo<
	typename std::iterator_traits<_IIter1>::value_type,
	typename std::iterator_traits<_IIter2>::value_type> _EqualTo;

      retu