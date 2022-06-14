// List implementation (out of line) -*- C++ -*-

// Copyright (C) 2001-2019 Free Software Foundation, Inc.
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

/*
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Hewlett-Packard Company makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 *
 * Copyright (c) 1996,1997
 * Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 */

/** @file bits/list.tcc
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{list}
 */

#ifndef _LIST_TCC
#define _LIST_TCC 1

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION
_GLIBCXX_BEGIN_NAMESPACE_CONTAINER

  template<typename _Tp, typename _Alloc>
    void
    _List_base<_Tp, _Alloc>::
    _M_clear() _GLIBCXX_NOEXCEPT
    {
      typedef _List_node<_Tp>  _Node;
      __detail::_List_node_base* __cur = _M_impl._M_node._M_next;
      while (__cur != &_M_impl._M_node)
	{
	  _Node* __tmp = static_cast<_Node*>(__cur);
	  __cur = __tmp->_M_next;
	  _Tp* __val = __tmp->_M_valptr();
#if __cplusplus >= 201103L
	  _Node_alloc_traits::destroy(_M_get_Node_allocator(), __val);
#else
	  _Tp_alloc_type(_M_get_Node_allocator()).destroy(__val);
#endif
	  _M_put_node(__tmp);
	}
    }

#if __cplusplus >= 201103L
  template<typename _Tp, typename _Alloc>
    template<typename... _Args>
      typename list<_Tp, _Alloc>::iterator
      list<_Tp, _Alloc>::
      emplace(const_iterator __position, _Args&&... __args)
      {
	_Node* __tmp = _M_create_node(std::forward<_Args>(__args)...);
	__tmp->_M_hook(__position._M_const_cast()._M_node);
	this->_M_inc_size(1);
	return iterator(__tmp);
      }
#endif

  template<typename _Tp, typename _Alloc>
    typename list<_Tp, _Alloc>::iterator
    list<_Tp, _Alloc>::
#if __cplusplus >= 201103L
    insert(const_iterator __position, const value_type& __x)
#else
    insert(iterator __position, const value_type& __x)
#endif
    {
      _Node* __tmp = _M_create_node(__x);
      __tmp->_M_hook(__position._M_const_cast()._M_node);
      this->_M_inc_size(1);
      return iterator(__tmp);
    }

#if __cplusplus >= 201103L
  template<typename _Tp, typename _Alloc>
    typename list<_Tp, _Alloc>::iterator
    list<_Tp, _Alloc>::
    insert(const_iterator __pos