// Safe iterator implementation  -*- C++ -*-

// Copyright (C) 2011-2019 Free Software Foundation, Inc.
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

/** @file debug/safe_local_iterator.h
 *  This file is a GNU debug extension to the Standard C++ Library.
 */

#ifndef _GLIBCXX_DEBUG_SAFE_LOCAL_ITERATOR_H
#define _GLIBCXX_DEBUG_SAFE_LOCAL_ITERATOR_H 1

#include <debug/safe_unordered_base.h>

#define _GLIBCXX_DEBUG_VERIFY_OPERANDS(_Lhs, _Rhs) \
  _GLIBCXX_DEBUG_VERIFY(!_Lhs._M_singular() && !_Rhs._M_singular(),	\
			_M_message(__msg_iter_compare_bad)		\
			._M_iterator(_Lhs, "lhs")			\
			._M_iterator(_Rhs, "rhs"));			\
  _GLIBCXX_DEBUG_VERIFY(_Lhs._M_can_compare(_Rhs),			\
			_M_message(__msg_compare_different)		\
			._M_iterator(_Lhs, "lhs")			\
			._M_iterator(_Rhs, "rhs"));			\
  _GLIBCXX_DEBUG_VERIFY(_Lhs._M_in_same_bucket(_Rhs),			\
			_M_message(__msg_local_iter_compare_bad)	\
			._M_iterator(_Lhs, "lhs")			\
			._M_iterator(_Rhs, "rhs"))

namespace __gnu_debug
{
  /** \brief Safe iterator wrapper.
   *
   *  The class template %_Safe_local_iterator is a wrapper around an
   *  iterator that tracks the iterator's movement among sequences and
   *  checks that operations performed on the "safe" iterator are
   *  legal. In additional to the basic iterator operations (which are
   *  validated, and then passed to the underlying iterator),
   *  %_Safe_local_iterator has member functions for iterator invalidation,
   *  attaching/detaching the iterator from sequences, and querying
   *  the iterator's state.
   */
  template<typename _Iterator, typename _Sequence>
    class _Safe_local_iterator
    : private _Iterator
    , public _Safe_local_iterator_base
    {
      typedef _Iterator _Iter_base;
      typedef _Safe_local_iterator_base _Safe_base;

      typedef typename _Sequence::size_type size_type;

      typedef std::iterator_traits<_Iterator> _Traits;

      typedef std::__are_same<
	typename _Sequence::_Base::const_local_iterator,
	_Iterator> _IsConstant;

      typedef typename __gnu_cxx::__conditional_type<_IsConstant::__value,
	typename _Sequence::_Base::local_iterator,
	typename _Sequence::_Base::const_local_iterator>::__type
      _OtherIterator;

      typedef _Safe_local_iterator _Self;
      typedef _Safe_local_iterator<_OtherIterator, _Sequence> _OtherSelf;

      struct _Attach_single
      { };

      _Safe_local_iterator(_Iterator __i, _Safe_sequence_base* __cont,
			   _Attach_single) noexcept
      : _Iter_base(__i)
      { _M_attach_single(__cont); }

    public:
      typedef _Iterator					iterator_type;
      typedef typename _Traits::iterator_category	iterator_category;
      typedef typename _Traits::value_type		value_type;
      typedef typename _Traits::difference_type		difference_type;
      typedef typename _Traits::reference		reference;
      typedef typename _Traits::pointer			pointer;

      /// @post the iterator is singular and unattached
      _Safe_local_iterator() noexcept : _Iter_base() { }

      /**
       * @brief Safe iterator construction from an unsafe iterator and
       * its sequence.
       *
       * @pre @p seq is not NULL
       * @post this is not 