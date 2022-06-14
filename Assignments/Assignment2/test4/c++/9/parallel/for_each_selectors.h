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

/** @file parallel/for_each_selectors.h
 *  @brief Functors representing different tasks to be plugged into the
 *  generic parallelization methods for embarrassingly parallel functions.
 *  This file is a GNU parallel extension to the Standard C++ Library.
 */

// Written by Felix Putze.

#ifndef _GLIBCXX_PARALLEL_FOR_EACH_SELECTORS_H
#define _GLIBCXX_PARALLEL_FOR_EACH_SELECTORS_H 1

#include <parallel/basic_iterator.h>

namespace __gnu_parallel
{
  /** @brief Generic __selector for embarrassingly parallel functions. */
  template<typename _It>
    struct __generic_for_each_selector
    {
      /** @brief _Iterator on last element processed; needed for some
       *  algorithms (e. g. std::transform()).
       */
      _It _M_finish_iterator;
    };

  /** @brief std::for_each() selector. */
  template<typename _It>
    struct __for_each_selector : public __generic_for_each_selector<_It>
    {
      /** @brief Functor execution.
       *  @param __o Operator.
       *  @param __i iterator referencing object. */
      template<typename _Op>
        bool
        operator()(_Op& __o, _It __i)
        {
          __o(*__i);
          return true;
        }
    };

  /** @brief std::generate() selector. */
  template<typename _It>
    struct __generate_selector : public __generic_for_each_selector<_It>
    {
      /** @brief Functor execution.
       *  @param __o Operator.
       *  @param __i iterator referencing object. */
      template<typename _Op>
        bool
        operator()(_Op& __o, _It __i)
        {
          *__i = __o();
          return true;
        }
    };

  /** @brief std::fill() selector. */
  template<typename _It>
    struct __fill_selector : public __generic_for_each_selector<_It>
    {
      /** @brief Functor execution.
       *  @param __v Current value.
       *  @param __i iterator referencing object. */
      template<typename _ValueType>
        bool
        operator()(_ValueType& __v, _It __i)
        {
          *__i = __v;
          return true;
        }
    };

  /** @brief std::transform() __selector, one input sequence variant. */
  template<typename _It>
    struct __transform1_selector : public __generic_for_each_selector<_It>
    {
      /** @brief Functor execution.
       *  @param __o Operator.
       *  @param __i iterator referencing object. */
      template<typename _Op>
        bool
        operator()(_Op& __o, _It __i)
        {
          *__i.second = __o(*__i.first);
          return true;
        }
    };

  /** @brief std::transform() __selector, two input sequences variant. */
  template<typename _It>
    struct __transform2_selector : public __generic_for_each_selector<_It>
    {
      /** @brief Functor execution.
       *  @param __o Operator.
       *  @param __i iterator referencing object. */
      template<typename _Op>
        bool
        operator()(_Op& __o, _It __i)
        {
          *__i._M_third = __o(*__i._M_first, *__i._M_second);
          return true;
        }
    };

  /** @brief std::replace() selector. */
  te