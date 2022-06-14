// Deque implementation (out of line) -*- C++ -*-

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
 * Copyright (c) 1997
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

/** @file bits/deque.tcc
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{deque}
 */

#ifndef _DEQUE_TCC
#define _DEQUE_TCC 1

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION
_GLIBCXX_BEGIN_NAMESPACE_CONTAINER

#if __cplusplus >= 201103L
  template <typename _Tp, typename _Alloc>
    void
    deque<_Tp, _Alloc>::
    _M_default_initialize()
    {
      _Map_pointer __cur;
      __try
        {
          for (__cur = this->_M_impl._M_start._M_node;
	       __cur < this->_M_impl._M_finish._M_node;
	       ++__cur)
            std::__uninitialized_default_a(*__cur, *__cur + _S_buffer_size(),
					   _M_get_Tp_allocator());
          std::__uninitialized_default_a(this->_M_impl._M_finish._M_first,
					 this->_M_impl._M_finish._M_cur,
					 _M_get_Tp_allocator());
        }
      __catch(...)
        {
          std::_Destroy(this->_M_impl._M_start, iterator(*__cur, __cur),
			_M_get_Tp_allocator());
          __throw_exception_again;
        }
    }
#endif

  template <typename _Tp, typename _Alloc>
    deque<_Tp, _Alloc>&
    deque<_Tp, _Alloc>::
    operator=(const deque& __x)
    {
      if (&__x != this)
	{
#if __cplusplus >= 201103L
	  if (_Alloc_traits::_S_propagate_on_copy_assign())
	    {
	      if (!_Alloc_traits::_S_always_equal()
	          && _M_get_Tp_allocator() != __x._M_get_Tp_allocator())
	        {
		  // Replacement allocator cannot free existing storage,
		  // so deallocate everything and take copy of __x's data.
		  _M_replace_map(__x, __x.get_allocator());
		  std::__alloc_on_copy(_M_get_Tp_allocator(),
				       __x._M_get_Tp_allocator());
		  return *this;
		}
	      std::__alloc_on_copy(_M_get_Tp_allocator(),
				   __x._M_get_Tp_allocator());
	    }
#endif
	  const size_type __len = size();
	  if (__len >= __x.size())
	    _M_erase_at_end(std::co