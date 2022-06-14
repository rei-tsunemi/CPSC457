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

/** @file parallel/losertree.h
*  @brief Many generic loser tree variants.
*  This file is a GNU parallel extension to the Standard C++ Library.
*/

// Written by Johannes Singler.

#ifndef _GLIBCXX_PARALLEL_LOSERTREE_H
#define _GLIBCXX_PARALLEL_LOSERTREE_H 1

#include <bits/stl_algobase.h>
#include <bits/stl_function.h>
#include <parallel/features.h>
#include <parallel/base.h>

namespace __gnu_parallel
{
  /**
   * @brief Guarded loser/tournament tree.
   *
   * The smallest element is at the top.
   *
   * Guarding is done explicitly through one flag _M_sup per element,
   * inf is not needed due to a better initialization routine.  This
   * is a well-performing variant.
   *
   * @param _Tp the element type
   * @param _Compare the comparator to use, defaults to std::less<_Tp>
   */
  template<typename _Tp, typename _Compare>
    class _LoserTreeBase
    {
    protected:
      /** @brief Internal representation of a _LoserTree element. */
      struct _Loser
      {
	/** @brief flag, true iff this is a "maximum" __sentinel. */
	bool _M_sup;
	/** @brief __index of the __source __sequence. */
	int _M_source;
	/** @brief _M_key of the element in the _LoserTree. */
	_Tp _M_key;
      };

      unsigned int _M_ik, _M_k, _M_offset;

      /** log_2{_M_k} */
      unsigned int _M_log_k;

      /** @brief _LoserTree __elements. */
      _Loser* _M_losers;

      /** @brief _Compare to use. */
      _Compare _M_comp;

      /**
       * @brief State flag that determines whether the _LoserTree is empty.
       *
       * Only used for building the _LoserTree.
       */
      bool _M_first_insert;

    public:
      /**
       * @brief The constructor.
       *
       * @param __k The number of sequences to merge.
       * @param __comp The comparator to use.
       */
      _LoserTreeBase(unsigned int __k, _Compare __comp)
      : _M_comp(__comp)
      {
	_M_ik = __k;

	// Compute log_2{_M_k} for the _Loser Tree
	_M_log_k = __rd_log2(_M_ik - 1) + 1;

	// Next greater power of 2.
	_M_k = 1 << _M_log_k;
	_M_offset = _M_k;

	// Avoid default-constructing _M_losers[]._M_key
	_M_losers = static_cast<_Loser*>(::operator new(2 * _M_k
							* sizeof(_Loser)));
	for (unsigned int __i = _M_ik - 1; __i < _M_k; ++__i)
	  _M_losers[__i + _M_k]._M_sup = true;

	_M_first_insert = true;
      }

      /**
       * @brief The destructor.
       */
      ~_LoserTreeBase()
      {
	for (unsigned int __i = 0; __i < (2 * _M_k); ++__i)
	  _M_losers[__i].~_Loser();
	::operator delete(_M_losers);
      }

      /**
       * @brief Initializes the sequence "_M_source" with the element "__key".
       *
       * @param __key the element to insert
       * @param __source __index of the __source __sequence
       * @param __sup flag that determines whether the value to insert is an
       *   explicit __supremum.
       */
      void
      __insert_start(const _Tp& __key, int __source, bool __sup)
      {
	unsigned int __pos = _M_k + __source;

	if (_M_first_insert)
	  {
	    // Construct all keys, so we can easily d