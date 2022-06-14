// <forward_list.h> -*- C++ -*-

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

/** @file bits/forward_list.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{forward_list}
 */

#ifndef _FORWARD_LIST_H
#define _FORWARD_LIST_H 1

#pragma GCC system_header

#include <initializer_list>
#include <bits/stl_iterator_base_types.h>
#include <bits/stl_iterator.h>
#include <bits/stl_algobase.h>
#include <bits/stl_function.h>
#include <bits/allocator.h>
#include <ext/alloc_traits.h>
#include <ext/aligned_buffer.h>

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION
_GLIBCXX_BEGIN_NAMESPACE_CONTAINER

  /**
   *  @brief  A helper basic node class for %forward_list.
   *          This is just a linked list with nothing inside it.
   *          There are purely list shuffling utility methods here.
   */
  struct _Fwd_list_node_base
  {
    _Fwd_list_node_base() = default;
    _Fwd_list_node_base(_Fwd_list_node_base&& __x) noexcept
      : _M_next(__x._M_next)
    { __x._M_next = nullptr; }

    _Fwd_list_node_base(const _Fwd_list_node_base&) = delete;
    _Fwd_list_node_base& operator=(const _Fwd_list_node_base&) = delete;

    _Fwd_list_node_base&
    operator=(_Fwd_list_node_base&& __x) noexcept
    {
      _M_next = __x._M_next;
      __x._M_next = nullptr;
      return *this;
    }

    _Fwd_list_node_base* _M_next = nullptr;

    _Fwd_list_node_base*
    _M_transfer_after(_Fwd_list_node_base* __begin,
		      _Fwd_list_node_base* __end) noexcept
    {
      _Fwd_list_node_base* __keep = __begin->_M_next;
      if (__end)
	{
	  __begin->_M_next = __end->_M_next;
	  __end->_M_next = _M_next;
	}
      else
	__begin->_M_next = nullptr;
      _M_next = __keep;
      return __end;
    }

    void
    _M_reverse_after() noexcept
    {
      _Fwd_list_node_base* __tail = _M_next;
      if (!__tail)
	return;
      while (_Fwd_list_node_base* __temp = __tail->_M_next)
	{
	  _Fwd_list_node_base* __keep = _M_next;
	  _M_next = __temp;
	  __tail->_M_next = __temp->_M_next;
	  _M_next->_M_next = __keep;
	}
    }
  };

  /**
   *  @brief  A helper node class for %forward_list.
   *          This is just a linked list with uninitialized storage for a
   *          data value in each node.
   *          There is a sorting utility method.
   */
  template<typename _Tp>
    struct _Fwd_list_node
    : public _Fwd_list_node_base
    {
      _Fwd_list_node() = default;

      __gnu_cxx::__aligned_buffer<_Tp> _M_storage;

      _Tp*
      _M_valptr() noexcept
      { return _M_storage._M_ptr(); }

      const _Tp*
      _M_valptr() const noexcept
      { return _M_storage._M_ptr(); }
    };

  /**
   *   @brief A forward_list::iterator.
   *
   *   All the functions are op overloads.
   */
  template<typename _Tp>
    struct _Fwd_list_iterator
    {
      typedef _Fwd_list_iterator<_Tp>		_Self;
      typedef _Fwd_list_node<_Tp>		_Node;

      typedef _Tp				value_type;
      typedef _Tp*				pointer;
      typedef _Tp&				reference;
      typedef ptrdiff_t				difference_type;
   