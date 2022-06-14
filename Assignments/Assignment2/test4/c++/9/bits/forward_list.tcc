// <forward_list.tcc> -*- C++ -*-

// Copyright (C) 2008-2019 Free Software Foundation, Inc.
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

/** @file bits/forward_list.tcc
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{forward_list}
 */

#ifndef _FORWARD_LIST_TCC
#define _FORWARD_LIST_TCC 1

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION
_GLIBCXX_BEGIN_NAMESPACE_CONTAINER

  template<typename _Tp, typename _Alloc>
    _Fwd_list_base<_Tp, _Alloc>::
    _Fwd_list_base(_Fwd_list_base&& __lst, _Node_alloc_type&& __a)
    : _M_impl(std::move(__a))
    {
      if (__lst._M_get_Node_allocator() == _M_get_Node_allocator())
	this->_M_impl._M_head = std::move(__lst._M_impl._M_head);
    }

  template<typename _Tp, typename _Alloc>
    template<typename... _Args>
      _Fwd_list_node_base*
      _Fwd_list_base<_Tp, _Alloc>::
      _M_insert_after(const_iterator __pos, _Args&&... __args)
      {
	_Fwd_list_node_base* __to
	  = const_cast<_Fwd_list_node_base*>(__pos._M_node);
	_Node* __thing = _M_create_node(std::forward<_Args>(__args)...);
	__thing->_M_next = __to->_M_next;
	__to->_M_next = __thing;
	return __to->_M_next;
      }

  template<typename _Tp, typename _Alloc>
    _Fwd_list_node_base*
    _Fwd_list_base<_Tp, _Alloc>::
    _M_erase_after(_Fwd_list_node_base* __pos)
    {
      _Node* __curr = static_cast<_Node*>(__pos->_M_next);
      __pos->_M_next = __curr->_M_next;
      _Node_alloc_traits::destroy(_M_get_Node_allocator(),
				  __curr->_M_valptr());
      __curr->~_Node();
      _M_put_node(__curr);
      return __pos->_M_next;
    }

  template<typename _Tp, typename _Alloc>
    _Fwd_list_node_base*
    _Fwd_list_base<_Tp, _Alloc>::
    _M_erase_after(_Fwd_list_node_base* __pos,
		   _Fwd_list_node_base* __last)
    {
      _Node* __curr = static_cast<_Node*>(__pos->_M_next);
      while (__curr != __last)
	{
	  _Node* __temp = __curr;
	  __curr = static_cast<_Node*>(__curr->_M_next);
	  _Node_alloc_traits::destroy(_M_get_Node_allocator(),
				      __temp->_M_valptr());
	  __temp->~_Node();
	  _M_put_node(__temp);
	}
      __pos->_M_next = __last;
      return __last;
    }

  // Called by the range constructor to implement [23.3.4.2]/9
  template<typename _Tp, typename _Alloc>
    template<typename _InputIterator>
      void
      forward_list<_Tp, _Alloc>::
      _M_range_initialize(_InputIterator __first, _InputIterator __last)
      {
	_Node_base* __to = &this->_M_impl._M_head;
	for (; __first != __last; ++__first)
	  {
	    __to->_M_next = this->_M_create_node(*__first);
	    __to = __to->_M_next;
	  }
      }

  // Called by forward_list(n,v,a).
  template<typename _Tp, typename _Alloc>
    void
    forward_list<_Tp, _Alloc>::
    _M_fill_initialize(size_type __n, const value_type& __value)
    {
      _Node_base* __to = &this->_M_impl._M_head;
      for (; __n; --__n)
	{
	  __to->_M_next = this->_M_create_node(__value);
	  __to = __to->_M_next;
	}
    }

  template<typename _Tp, typename _Alloc>
    void
    forward_list<_Tp, _Alloc>::
    _M_default_initialize(size_type __n)
    {
     