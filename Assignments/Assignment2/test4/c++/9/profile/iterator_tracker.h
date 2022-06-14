// Profiling iterator implementation -*- C++ -*-

// Copyright (C) 2009-2019 Free Software Foundation, Inc.
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

/** @file profile/iterator_tracker.h
 *  This file is a GNU profile extension to the Standard C++ Library.
 */

#ifndef _GLIBCXX_PROFILE_ITERATOR_TRACKER
#define _GLIBCXX_PROFILE_ITERATOR_TRACKER 1

#include <ext/type_traits.h>

namespace std _GLIBCXX_VISIBILITY(default)
{
namespace __profile
{
  template<typename _Iterator, typename _Sequence>
    class __iterator_tracker
    {
      typedef __iterator_tracker _Self;

      // The underlying iterator
      _Iterator _M_current;

      // The underlying data structure
      const _Sequence* _M_ds;
      typedef std::iterator_traits<_Iterator> _Traits;

    public:
      typedef _Iterator					_Base_iterator;
      typedef typename _Traits::iterator_category	iterator_category;
      typedef typename _Traits::value_type		value_type;
      typedef typename _Traits::difference_type		difference_type;
      typedef typename _Traits::reference		reference;
      typedef typename _Traits::pointer			pointer;

      __iterator_tracker() _GLIBCXX_NOEXCEPT
      : _M_current(), _M_ds(0) { }

      __iterator_tracker(const _Iterator& __i, const _Sequence* __seq)
      _GLIBCXX_NOEXCEPT
      : _M_current(__i), _M_ds(__seq) { }

      __iterator_tracker(const __iterator_tracker& __x) _GLIBCXX_NOEXCEPT
      : _M_current(__x._M_current), _M_ds(__x._M_ds) { }

      template<typename _MutableIterator>
	__iterator_tracker(const __iterator_tracker<_MutableIterator,
			   typename __gnu_cxx::__enable_if
			   <(std::__are_same<_MutableIterator, typename
			     _Sequence::iterator::_Base_iterator>::__value),
			   _Sequence>::__type>& __x) _GLIBCXX_NOEXCEPT
	:  _M_current(__x.base()), _M_ds(__x._M_get_sequence()) { }

      _Iterator
      base() const _GLIBCXX_NOEXCEPT { return _M_current; }

      /**
       * @brief Conversion to underlying non-debug iterator to allow
       * better interaction with non-profile containers.
       */
      operator _Iterator() const _GLIBCXX_NOEXCEPT { return _M_current; }

      pointer
      operator->() const _GLIBCXX_NOEXCEPT { return &*_M_current; }

      __iterator_tracker&
      operator++() _GLIBCXX_NOEXCEPT
      {
	_M_ds->_M_profile_iterate();
	++_M_current;
	return *this;
      }

      __iterator_tracker
      operator++(int) _GLIBCXX_NOEXCEPT
      {
	_M_ds->_M_profile_iterate();
	__iterator_tracker __tmp(*this);
	++_M_current;
	return __tmp;
      }

      __iterator_tracker&
      operator--() _GLIBCXX_NOEXCEPT
      {
	_M_ds->_M_profile_iterate(1);
	--_M_current;
	return *this;
      }

      __iterator_tracker
      operator--(int) _GLIBCXX_NOEXCEPT
      {
	_M_ds->_M_profile_iterate(1);
	__iterator_tracker __tmp(*this);
	--_M_current;
	return __tmp;
      }

      __iterator_tracker&
      operator=(const __iterator_tracker& __x) _GLIBCXX_NOEXCEPT
      {
	_M_current = __x._M_current;
	_M_ds = __x._M_ds;
	return *this;
      }

      reference
      operator*() const _GLIBCXX_NOEXCEPT
      { return *_M_current; }

      // ------ Random access iterator requirements -