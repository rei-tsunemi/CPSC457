// <parallel/numeric> Forward declarations -*- C++ -*-

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

/** @file parallel/numericfwd.h
 *  This file is a GNU parallel extension to the Standard C++ Library.
 */

#ifndef _GLIBCXX_PARALLEL_NUMERICFWD_H
#define _GLIBCXX_PARALLEL_NUMERICFWD_H 1

#pragma GCC system_header

#include <parallel/tags.h>
#include <parallel/settings.h>

namespace std _GLIBCXX_VISIBILITY(default)
{
namespace __parallel
{
  template<typename _IIter, typename _Tp>
    _Tp
    accumulate(_IIter, _IIter, _Tp);

  template<typename _IIter, typename _Tp>
    _Tp
    accumulate(_IIter, _IIter, _Tp, __gnu_parallel::sequential_tag);

  template<typename _IIter, typename _Tp>
    _Tp
    accumulate(_IIter, _IIter, _Tp, __gnu_parallel::_Parallelism);

  template<typename _IIter, typename _Tp, typename _Tag>
    _Tp
    __accumulate_switch(_IIter, _IIter, _Tp, _Tag);

  template<typename _IIter, typename _Tp, typename _BinaryOper>
    _Tp
    accumulate(_IIter, _IIter, _Tp, _BinaryOper);

  template<typename _IIter, typename _Tp, typename _BinaryOper>
    _Tp
    accumulate(_IIter, _IIter, _Tp, _BinaryOper,
               __gnu_parallel::sequential_tag);

  template<typename _IIter, typename _Tp, typename _BinaryOper>
    _Tp
    accumulate(_IIter, _IIter, _Tp, _BinaryOper,
               __gnu_parallel::_Parallelism);

  template<typename _IIter, typename _Tp, typename _BinaryOper,
           typename _Tag>
    _Tp
    __accumulate_switch(_IIter, _IIter, _Tp, _BinaryOper, _Tag);

  template<typename _RAIter, typename _Tp, typename _BinaryOper>
    _Tp
    __accumulate_switch(_RAIter, _RAIter, _Tp, _BinaryOper,
                      random_access_iterator_tag,
                      __gnu_parallel::_Parallelism __parallelism
                      = __gnu_parallel::parallel_unbalanced);

  template<typename _IIter, typename _OIter>
    _OIter
    adjacent_difference(_IIter, _IIter, _OIter);

  template<typename _IIter, typename _OIter, typename _BinaryOper>
    _OIter
    adjacent_difference(_IIter, _IIter, _OIter, _BinaryOper);

  template<typename _IIter, typename _OIter>
    _OIter
    adjacent_difference(_IIter, _IIter, _OIter,
                        __gnu_parallel::sequential_tag);

  template<typename _IIter, typename _OIter, typename _BinaryOper>
    _OIter
    adjacent_difference(_IIter, _IIter, _OIter, _BinaryOper, 
                        __gnu_parallel::sequential_tag);

  template<typename _IIter, typename _OIter>
    _OIter
    adjacent_difference(_IIter, _IIter, _OIter,
                        __gnu_parallel::_Parallelism);

  template<typename _IIter, typename _OIter, typename _BinaryOper>
    _OIter
    adjacent_difference(_IIter, _IIter, _OIter, _BinaryOper, 
                        __gnu_parallel::_Parallelism);

  template<typename _IIter, typename _OIter, typename _BinaryOper,
           typename _Tag1, typename _Tag2>
    _OIter
    __adjacent_difference_switch(_IIter, _IIter, _OIter, _BinaryOper,
                               _Tag1, _Tag2);

  template<typename _IIter, typename _OIter, typename _BinaryOper>
    _OIte