// Safe iterator implementation  -*- C++ -*-

// Copyright (C) 2003-2019 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

/** @file debug/safe_iterator.h
 *  This file is a GNU debug extension to the Standard C++ Library.
 */

#ifndef _GLIBCXX_DEBUG_SAFE_ITERATOR_H
#define _GLIBCXX_DEBUG_SAFE_ITERATOR_H 1

#include <debug/assertions.h>
#include <debug/macros.h>
#include <debug/functions.h>
#include <debug/safe_base.h>
#include <bits/stl_pair.h>
#include <ext/type_traits.h>

#define _GLIBCXX_DEBUG_VERIFY_OPERANDS(_Lhs, _Rhs, _BadMsgId, _DiffMsgId) \
  _GLIBCXX_DEBUG_VERIFY(!_Lhs._M_singular() && !_Rhs._M_singular(),	\
			_M_message(_BadMsgId)				\
			._M_iterator(_Lhs, #_Lhs)			\
			._M_iterator(_Rhs, #_Rhs));			\
  _GLIBCXX_DEBUG_VERIFY(_Lhs._M_can_compare(_Rhs),			\
			_M_message(_DiffMsgId)				\
			._M_iterator(_Lhs, #_Lhs)			\
			._M_iterator(_Rhs, #_Rhs))

#define _GLIBCXX_DEBUG_VERIFY_EQ_OPERANDS(_Lhs, _Rhs)			\
  _GLIBCXX_DEBUG_VERIFY_OPERANDS(_Lhs, _Rhs, __msg_iter_compare_bad,	\
				 __msg_compare_different)

#define _GLIBCXX_DEBUG_VERIFY_REL_OPERANDS(_Lhs, _Rhs)		\
  _GLIBCXX_DEBUG_VERIFY_OPERANDS(_Lhs, _Rhs, __msg_iter_order_bad,	\
				 __msg_order_different)

#define _GLIBCXX_DEBUG_VERIFY_DIST_OPERANDS(_Lhs, _Rhs)			\
  _GLIBCXX_DEBUG_VERIFY_OPERANDS(_Lhs, _Rhs, __msg_distance_bad,	\
				 __msg_distance_different)

namespace __gnu_debug
{
  /** Helper struct to deal with sequence offering a before_begin
   *  iterator.
   **/
  template<typename _Sequence>
    struct _BeforeBeginHelper
    {
      template<typename _Iterator, typename _Category>
	static bool
	_S_Is(const _Safe_iterator<_Iterator, _Sequence, _Category>&)
	{ return false; }

      template<typename _Iterator, typename _Category>
	static bool
	_S_Is_Beginnest(const _Safe_iterator<_Iterator, _Sequence, _Category>& __it)
	{ return __it.base() == __it._M_get_sequence()->_M_base().begin(); }
    };

  /** Sequence traits giving the size of a container if possible. */
  template<typename _Sequence>
    struct _Sequence_traits
    {
      typedef _Distance_traits<typename _Sequence::iterator> _DistTraits;

      static typename _DistTraits::__type
      _S_size(const _Sequence& __seq)
      { return std::make_pair(__seq.size(), __dp_exact); }
    };

  /** \brief Safe iterator wrapper.
   *
   *  The class template %_Safe_iterator is a wrapper around an
   *  iterator that tracks the iterator's movement among sequences and
   *  checks that operations performed on the "safe" iterator are
   *  legal. In additional to the basic iterator operations (which are
   *  validated, and then passed to the underlying iterator),
   *  %_Safe_iterator has member functions for iterator invalidation,
   *  attaching/detaching the iterator from sequences, and querying
   *  the iterator's state.
   *
   *  Note that _Iterator must be the first base class so that it gets
   *  initialized before the iterator is being attached to the container's list
   *  of iterators and it is being detached before _Iterator get
   *  destroyed. Otherwise it would result in a data race.
   */
  template<typename _Iterator, typename _Seque