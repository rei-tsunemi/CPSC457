// Debugging iterator implementation (out of line) -*- C++ -*-

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

/** @file debug/safe_iterator.tcc
 *  This file is a GNU debug extension to the Standard C++ Library.
 */

#ifndef _GLIBCXX_DEBUG_SAFE_ITERATOR_TCC
#define _GLIBCXX_DEBUG_SAFE_ITERATOR_TCC 1

namespace __gnu_debug
{
  template<typename _Iterator, typename _Sequence, typename _Category>
    typename _Distance_traits<_Iterator>::__type
    _Safe_iterator<_Iterator, _Sequence, _Category>::
    _M_get_distance_from_begin() const
    {
      typedef _Sequence_traits<_Sequence> _SeqTraits;

      // No need to consider before_begin as this function is only used in
      // _M_can_advance which won't be used for forward_list iterators.
      if (_M_is_begin())
	return std::make_pair(0, __dp_exact);

      if (_M_is_end())
	return _SeqTraits::_S_size(*_M_get_sequence());

      typename _Distance_traits<_Iterator>::__type __res
	= __get_distance(_M_get_sequence()->_M_base().begin(), base());

      if (__res.second == __dp_equality)
	return std::make_pair(1, __dp_sign);

      return __res;
    }

  template<typename _Iterator, typename _Sequence, typename _Category>
    typename _Distance_traits<_Iterator>::__type
    _Safe_iterator<_Iterator, _Sequence, _Category>::
    _M_get_distance_to_end() const
    {
      typedef _Sequence_traits<_Sequence> _SeqTraits;

      // No need to consider before_begin as this function is only used in
      // _M_can_advance which won't be used for forward_list iterators.
      if (_M_is_begin())
	return _SeqTraits::_S_size(*_M_get_sequence());

      if (_M_is_end())
	return std::make_pair(0, __dp_exact);

      typename _Distance_traits<_Iterator>::__type __res
	= __get_distance(base(), _M_get_sequence()->_M_base().end());

      if (__res.second == __dp_equality)
	return std::make_pair(1, __dp_sign);

      return __res;
    }

  template<typename _Iterator, typename _Sequence, typename _Category>
    bool
    _Safe_iterator<_Iterator, _Sequence, _Category>::
    _M_can_advance(difference_type __n) const
    {
      if (this->_M_singular())
	return false;

      if (__n == 0)
	return true;

      if (__n < 0)
	{
	  std::pair<difference_type, _Distance_precision> __dist =
	    _M_get_distance_from_begin();
	  bool __ok =  ((__dist.second == __dp_exact && __dist.first >= -__n)
			|| (__dist.second != __dp_exact && __dist.first > 0));
	  return __ok;
	}
      else
	{
	  std::pair<difference_type, _Distance_precision> __dist =
	    _M_get_distance_to_end();
	  bool __ok = ((__dist.second == __dp_exact && __dist.first >= __n)
		       || (__dist.second != __dp_exact && __dist.first > 0));
	  return __ok;
	}
    }

  template<typename _Iterator, typename _Sequence, typename _Category>
    typename _Distance_traits<_Iterator>::__type
    _Safe_iterator<_Iterator, _Sequence, _Category>::
    _M_get_distance_to(const _Safe_iterator& __rhs) const
    {
      typedef typename _Distance_traits<_Iterator>::__type _Diff;
      typedef _Sequence_traits<_Sequence> _SeqTraits;

      if (this->base() == __rhs.base())
	return 