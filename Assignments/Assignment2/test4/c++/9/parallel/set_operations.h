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

/**
 * @file parallel/set_operations.h
 * @brief Parallel implementations of set operations for random-access
 * iterators.
 *  This file is a GNU parallel extension to the Standard C++ Library.
 */

// Written by Marius Elvert and Felix Bondarenko.

#ifndef _GLIBCXX_PARALLEL_SET_OPERATIONS_H
#define _GLIBCXX_PARALLEL_SET_OPERATIONS_H 1

#include <omp.h>

#include <parallel/settings.h>
#include <parallel/multiseq_selection.h>

namespace __gnu_parallel
{
  template<typename _IIter, typename _OutputIterator>
    _OutputIterator
    __copy_tail(std::pair<_IIter, _IIter> __b,
		std::pair<_IIter, _IIter> __e, _OutputIterator __r)
    {
      if (__b.first != __e.first)
	{
          do
            {
              *__r++ = *__b.first++;
            }
          while (__b.first != __e.first);
	}
      else
	{
          while (__b.second != __e.second)
            *__r++ = *__b.second++;
	}
      return __r;
    }

  template<typename _IIter,
           typename _OutputIterator,
           typename _Compare>
    struct __symmetric_difference_func
    {
      typedef std::iterator_traits<_IIter> _TraitsType;
      typedef typename _TraitsType::difference_type _DifferenceType;
      typedef typename std::pair<_IIter, _IIter> _IteratorPair;

      __symmetric_difference_func(_Compare __comp) : _M_comp(__comp) {}

      _Compare _M_comp;

      _OutputIterator
      _M_invoke(_IIter __a, _IIter __b, _IIter __c, _IIter __d,
		_OutputIterator __r) const
      {
	while (__a != __b && __c != __d)
          {
            if (_M_comp(*__a, *__c))
              {
        	*__r = *__a;
        	++__a;
        	++__r;
              }
            else if (_M_comp(*__c, *__a))
              {
        	*__r = *__c;
        	++__c;
        	++__r;
              }
            else
              {
        	++__a;
        	++__c;
              }
          }
	return std::copy(__c, __d, std::copy(__a, __b, __r));
      }

      _DifferenceType
      __count(_IIter __a, _IIter __b, _IIter __c, _IIter __d) const
      {
	_DifferenceType __counter = 0;

	while (__a != __b && __c != __d)
          {
            if (_M_comp(*__a, *__c))
              {
        	++__a;
        	++__counter;
              }
            else if (_M_comp(*__c, *__a))
              {
        	++__c;
        	++__counter;
              }
            else
              {
        	++__a;
        	++__c;
              }
          }

	return __counter + (__b - __a) + (__d - __c);
      }

      _OutputIterator
      __first_empty(_IIter __c, _IIter __d, _OutputIterator __out) const
      { return std::copy(__c, __d, __out); }

      _OutputIterator
      __second_empty(_IIter __a, _IIter __b, _OutputIterator __out) const
      { return std::copy(__a, __b, __out); }
    };


  template<typename _IIter,
           typename _OutputIterator,
           typename _Compare>
    struct __difference_func
    {
      typedef std::iterator_traits<_IIter> _TraitsType;
      typedef typename _TraitsType::difference_type _DifferenceType;
      typedef typename 