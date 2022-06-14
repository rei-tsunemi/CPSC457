// List implementation -*- C++ -*-

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

/** @file bits/stl_list.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{list}
 */

#ifndef _STL_LIST_H
#define _STL_LIST_H 1

#include <bits/concept_check.h>
#include <ext/alloc_traits.h>
#if __cplusplus >= 201103L
#include <initializer_list>
#include <bits/allocated_ptr.h>
#include <ext/aligned_buffer.h>
#endif

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  namespace __detail
  {
    // Supporting structures are split into common and templated
    // types; the latter publicly inherits from the former in an
    // effort to reduce code duplication.  This results in some
    // "needless" static_cast'ing later on, but it's all safe
    // downcasting.

    /// Common part of a node in the %list.
    struct _List_node_base
    {
      _List_node_base* _M_next;
      _List_node_base* _M_prev;

      static void
      swap(_List_node_base& __x, _List_node_base& __y) _GLIBCXX_USE_NOEXCEPT;

      void
      _M_transfer(_List_node_base* const __first,
		  _List_node_base* const __last) _GLIBCXX_USE_NOEXCEPT;

      void
      _M_reverse() _GLIBCXX_USE_NOEXCEPT;

      void
      _M_hook(_List_node_base* const __position) _GLIBCXX_USE_NOEXCEPT;

      void
      _M_unhook() _GLIBCXX_USE_NOEXCEPT;
    };

    /// The %list node header.
    struct _List_node_header : public _List_node_base
    {
#if _GLIBCXX_USE_CXX11_ABI
      std::size_t _M_size;
#endif

      _List_node_header() _GLIBCXX_NOEXCEPT
      { _M_init(); }

#if __cplusplus >= 201103L
      _List_node_header(_List_node_header&& __x) noexcept
      : _List_node_base{ __x._M_next, __x._M_prev }
# if _GLIBCXX_USE_CXX11_ABI
      , _M_size(__x._M_size)
# endif
      {
	if (__x._M_base()->_M_next == __x._M_base())
	  this->_M_next = this->_M_prev = this;
	else
	  {
	    this->_M_next->_M_prev = this->_M_prev->_M_next = thi