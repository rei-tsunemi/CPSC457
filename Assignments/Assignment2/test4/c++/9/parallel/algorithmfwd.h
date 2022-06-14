// <parallel/algorithm> Forward declarations -*- C++ -*-

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

/** @file parallel/algorithmfwd.h
 *  This file is a GNU parallel extension to the Standard C++ Library.
 */

#ifndef _GLIBCXX_PARALLEL_ALGORITHMFWD_H
#define _GLIBCXX_PARALLEL_ALGORITHMFWD_H 1

#pragma GCC system_header

#include <parallel/tags.h>
#include <parallel/settings.h>

namespace std _GLIBCXX_VISIBILITY(default)
{
namespace __parallel
{
  template<typename _FIter>
    _FIter
    adjacent_find(_FIter, _FIter);

  template<typename _FIter>
    _FIter
    adjacent_find(_FIter, _FIter, __gnu_parallel::sequential_tag);

  template<typename _FIter, typename _IterTag>
    _FIter
    __adjacent_find_switch(_FIter, _FIter, _IterTag);

  template<typename _RAIter>
    _RAIter
    __adjacent_find_switch(_RAIter, _RAIter, random_access_iterator_tag);


  template<typename _FIter, typename _BiPredicate>
    _FIter
    adjacent_find(_FIter, _FIter, _BiPredicate);

  template<typename _FIter, typename _BiPredicate>
    _FIter
    adjacent_find(_FIter, _FIter, _BiPredicate,
                  __gnu_parallel::sequential_tag);

  template<typename _FIter, typename _BiPredicate, typename _IterTag>
    _FIter
    __adjacent_find_switch(_FIter, _FIter, _BiPredicate, _IterTag);

  template<typename _RAIter, typename _BiPredicate>
    _RAIter
    __adjacent_find_switch(_RAIter, _RAIter, _BiPredicate, 
                         random_access_iterator_tag);


  template<typename _IIter, typename _Tp>
    typename iterator_traits<_IIter>::difference_type
    count(_IIter, _IIter, const _Tp&);

  template<typename _IIter, typename _Tp>
    typename iterator_traits<_IIter>::difference_type
    count(_IIter, _IIter, const _Tp&, __gnu_parallel::sequential_tag);

  template<typename _IIter, typename _Tp>
    typename iterator_traits<_IIter>::difference_type
    count(_IIter, _IIter, const _Tp&, __gnu_parallel::_Parallelism);

  template<typename _IIter, typename _Tp, typename _IterTag>
    typename iterator_traits<_IIter>::difference_type
    __count_switch(_IIter, _IIter, const _Tp&, _IterTag);

  template<typename _RAIter, typename _Tp>
    typename iterator_traits<_RAIter>::difference_type
    __count_switch(_RAIter, _RAIter, const _Tp&, random_access_iterator_tag,
                 __gnu_parallel::_Parallelism __parallelism
                 = __gnu_parallel::parallel_unbalanced);


  template<typename _IIter, typename _Predicate>
    typename iterator_traits<_IIter>::difference_type
    count_if(_IIter, _IIter, _Predicate);

  template<typename _IIter, typename _Predicate>
    typename iterator_traits<_IIter>::difference_type
    count_if(_IIter, _IIter, _Predicate, __gnu_parallel::sequential_tag);

  template<typename _IIter, typename _Predicate>
    typename iterator_traits<_IIter>::difference_type
    count_if(_IIter, _IIter, _Predicate, __gnu_parallel::_Parallelism);

  template<typename _IIter, typename _Predicate, typename _IterTag>
    typename iterator_traits<_IIter>::difference_type
    __count_if_switch(_IIter, _IIter, _Predicate, _IterTag);

  template<