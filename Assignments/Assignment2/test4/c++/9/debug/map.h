// Debugging map implementation -*- C++ -*-

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

/** @file debug/map.h
 *  This file is a GNU debug extension to the Standard C++ Library.
 */

#ifndef _GLIBCXX_DEBUG_MAP_H
#define _GLIBCXX_DEBUG_MAP_H 1

#include <debug/safe_sequence.h>
#include <debug/safe_container.h>
#include <debug/safe_iterator.h>
#include <utility>

namespace std _GLIBCXX_VISIBILITY(default)
{
namespace __debug
{
  /// Class std::map with safety/checking/debug instrumentation.
  template<typename _Key, typename _Tp, typename _Compare = std::less<_Key>,
	   typename _Allocator = std::allocator<std::pair<const _Key, _Tp> > >
    class map
    : public __gnu_debug::_Safe_container<
	map<_Key, _Tp, _Compare, _Allocator>, _Allocator,
	__gnu_debug::_Safe_node_sequence>,
      public _GLIBCXX_STD_C::map<_Key, _Tp, _Compare, _Allocator>
    {
      typedef _GLIBCXX_STD_C::map<
	_Key, _Tp, _Compare, _Allocator>			_Base;
      typedef __gnu_debug::_Safe_container<
	map, _Allocator, __gnu_debug::_Safe_node_sequence>	_Safe;

      typedef typename _Base::const_iterator	_Base_const_iterator;
      typedef typename _Base::iterator		_Base_iterator;
      typedef __gnu_debug::_Equal_to<_Base_const_iterator> _Equal;

      template<typename _ItT, typename _SeqT, typename _CatT>
	friend class ::__gnu_debug::_Safe_iterator;

    public:
      // types:
      typedef _Key					key_type;
      typedef _Tp					mapped_type;
      typedef std::pair<const _Key, _Tp>		value_type;
      typedef _Compare					key_compare;
      typedef _Allocator				allocator_type;
      typedef typename _Base::reference			reference;
      typedef typename _Base::const_reference		const_reference;

      typedef __gnu_debug::_Safe_iterator<_Base_iterator, map>
							iterator;
      typedef __gnu_debug::_Safe_iterator<_Base_const_iterator, map>
							const_iterator;

      typedef typename _Base::size_type			size_type;
      typedef typename _Base::difference_type		difference_type;
      typedef typename _Base::pointer			pointer;
      typedef typename _Base::const_pointer		const_pointer;
      typedef std::reverse_iterator<iterator>		reverse_iterator;
      typedef std::reverse_iterator<const_iterator>	const_reverse_iterator;

      // 23.3.1.1 construct/copy/destroy:

#if __cplusplus < 201103L
      map() : _Base() { }

      map(const map& __x)
      : _Base(__x) { }

      ~map() { }
#else
      map() = default;
      map(const map&) = default;
      map(map&&) = default;

      map(initializer_list<value_type> __l,
	  const _Compare& __c = _Compare(),
	  const allocator_type& __a = allocator_type())
      : _Base(__l, __c, __a) { }

      explicit
      map(const allocator_type& __a)
      : _Base(__a) { }

      map(const map& __m, const allocator_type& __a)
      : _Base(__m, __a) { }

      map(map&& __m, const allocator_type& __a)
      noexcept( noexcept(_Base(std::move(__m._M_base()), __a)) )
      : _Safe(std::move(__m._M_safe()), __a),
	_Base(std::move(__m._M_base()), __a) { }

      map(initializer_list<value_type> __l, const allocator_type& __a)
      : _Base(__l