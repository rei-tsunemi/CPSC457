// SGI's rope class implementation -*- C++ -*-

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

/** @file ropeimpl.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{ext/rope}
 */

#include <cstdio>
#include <ostream>
#include <bits/functexcept.h>

#include <ext/algorithm> // For copy_n and lexicographical_compare_3way
#include <ext/memory> // For uninitialized_copy_n
#include <ext/numeric> // For power

namespace __gnu_cxx _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  using std::size_t;
  using std::printf;
  using std::basic_ostream;
  using std::__throw_length_error;
  using std::_Destroy;
  using std::__uninitialized_fill_n_a;

  // Set buf_start, buf_end, and buf_ptr appropriately, filling tmp_buf
  // if necessary.  Assumes _M_path_end[leaf_index] and leaf_pos are correct.
  // Results in a valid buf_ptr if the iterator can be legitimately
  // dereferenced.
  template <class _CharT, class _Alloc>
    void
    _Rope_iterator_base<_CharT, _Alloc>::
    _S_setbuf(_Rope_iterator_base<_CharT, _Alloc>& __x)
    {
      const _RopeRep* __leaf = __x._M_path_end[__x._M_leaf_index];
      size_t __leaf_pos = __x._M_leaf_pos;
      size_t __pos = __x._M_current_pos;

      switch(__leaf->_M_tag)
	{
	case __detail::_S_leaf:
	  __x._M_buf_start = ((_Rope_RopeLeaf<_CharT, _Alloc>*)__leaf)->_M_data;
	  __x._M_buf_ptr = __x._M_buf_start + (__pos - __leaf_pos);
	  __x._M_buf_end = __x._M_buf_start + __leaf->_M_size;
	  break;
	case __detail::_S_function:
	case __detail::_S_substringfn:
	  {
	    size_t __len = _S_iterator_buf_len;
	    size_t __buf_start_pos = __leaf_pos;
	    size_t __leaf_end = __leaf_pos + __leaf->_M_size;
	    char_producer<_CharT>* __fn = ((_Rope_RopeFunction<_CharT,
					    _Alloc>*)__leaf)->_M_fn;
	    if (__buf_start_pos + __len <= __pos)
	      {
		__buf_start_pos = __pos - __len / 4;
		if (__buf_start_pos + __len > __leaf_end)
		  __buf_start_pos = __leaf_end - __len;
	      }
	    if (__buf_start_pos + __len > __leaf_end)
	      __len = __leaf_end - __buf_start_pos;
	    (*__fn)(__buf_start_pos - __leaf_pos, __len, __x._M_tmp_buf);
	    __x._M_buf_ptr = __x._M_tmp_buf + (__pos - __buf_start_pos);
	    __x._M_buf_start = __x._M_tmp_buf;
	    __x._M_buf_end = __x._M_tmp_buf + __len;
	  }
	  break;
	default:
	  break;
	}
    }

  // Set path and buffer inside a rope iterator.  We assume that
  // pos and root are already set.
  template <class _CharT, class _Alloc>
    void
    _Rope_iterator_bas