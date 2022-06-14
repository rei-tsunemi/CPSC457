// Profiling map implementation -*- C++ -*-

// Copyright (C) 2009-2019 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

/** @file profile/map.h
 *  This file is a GNU profile extension to the Standard C++ Library.
 */

#ifndef _GLIBCXX_PROFILE_MAP_H
#define _GLIBCXX_PROFILE_MAP_H 1

#include <profile/base.h>
#include <profile/ordered_base.h>

namespace std _GLIBCXX_VISIBILITY(default)
{
namespace __profile
{
  /// Class std::map wrapper with performance instrumentation.
  template<typename _Key, typename _Tp, typename _Compare = std::less<_Key>,
	   typename _Allocator = std::allocator<std::pair<const _Key, _Tp> > >
    class map
    : public _GLIBCXX_STD_C::map<_Key, _Tp, _Compare, _Allocator>,
      public _Ordered_profile<map<_Key, _Tp, _Compare, _Allocator> >
    {
      typedef _GLIBCXX_STD_C::map<_Key, _Tp, _Compare, _Allocator> _Base;

      typedef typename _Base::iterator			_Base_iterator;
      typedef typename _Base::const_iterator		_Base_const_iterator;

    public:
      // types:
      typedef _Key					key_type;
      typedef _Tp					mapped_type;
      typedef typename _Base::value_type		value_type;
      typedef _Compare					key_compare;
      typedef typename _Base::reference			reference;
      typedef typename _Base::const_reference		const_reference;

      typedef __iterator_tracker<_Base_iterator, map>	iterator;
      typedef __iterator_tracker<_Base_const_iterator,
				 map>			const_iterator;
      typedef std::reverse_iterator<iterator>		reverse_iterator;
      typedef std::reverse_iterator<const_iterator>	const_reverse_iterator;

      typedef typename _Base::size_type			size_type;
      typedef typename _Base::difference_type		difference_type;

      // 23.3.1.1 construct/copy/destroy:

#if __cplusplus < 201103L
      map()
      : _Base() { }
      map(const map& __x)
	: _Base(__x) { }
      ~map()
      { }
#else
      map() = default;
      map(const map&) = default;
      map(map&&) = default;
      ~map() = default;
#endif

      explicit
      map(const _Compare& __comp,
	  const _Allocator& __a = _Allocator())
      : _Base(__comp, __a) { }

#if __cplusplus >= 201103L
      template<typename _InputIterator,
	       typename = std::_RequireInputIter<_InputIterator>>
#else
      template<typename _InputIterator>
#endif
	map(_InputIterator __first, _InputIterator __last,
	    const _Compare& __comp = _Compare(),
	    const _Allocator& __a = _Allocator())
	: _Base(__first, __last, __comp, __a) { }

      map(const _Base& __x)
      : _Base(__x) { }

#if __cplusplus >= 201103L
      map(initializer_list<value_type> __l,
	  const _Compare& __c = _Compare(),
	  const _Allocator& __a = _Allocator())
      : _Base(__l, __c, __a) { }

      explicit
      map(const _Allocator& __a)
      : _Base(__a) { }

      map(const map& __x, const _Allocator& __a)
      : _Base(__x, __a) { }

      map(map&& __x, const _Allocator& __a)
      noexcept( noexcept(_Base(std::move(__x), __a)) )
      : _Base(std::move(__x), __a) { }

      map(initializer_list<value_type> __l, const _Allocator& __a)
      : _Base(__l, __a) { }

      template<typename _InputIterator>
	map(_InputIterator __first, _InputIterator __last,
	    const _Allocator& __a)
	: _Bas