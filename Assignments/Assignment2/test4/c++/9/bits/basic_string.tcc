// Components for manipulating sequences of characters -*- C++ -*-

// Copyright (C) 1997-2019 Free Software Foundation, Inc.
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

/** @file bits/basic_string.tcc
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{string}
 */

//
// ISO C++ 14882: 21  Strings library
//

// Written by Jason Merrill based upon the specification by Takanori Adachi
// in ANSI X3J16/94-0013R2.  Rewritten by Nathan Myers to ISO-14882.
// Non-reference-counted implementation written by Paolo Carlini and
// updated by Jonathan Wakely for ISO-14882-2011.

#ifndef _BASIC_STRING_TCC
#define _BASIC_STRING_TCC 1

#pragma GCC system_header

#include <bits/cxxabi_forced.h>

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

#if _GLIBCXX_USE_CXX11_ABI

  template<typename _CharT, typename _Traits, typename _Alloc>
    const typename basic_string<_CharT, _Traits, _Alloc>::size_type
    basic_string<_CharT, _Traits, _Alloc>::npos;

  template<typename _CharT, typename _Traits, typename _Alloc>
    void
    basic_string<_CharT, _Traits, _Alloc>::
    swap(basic_string& __s) _GLIBCXX_NOEXCEPT
    {
      if (this == &__s)
	return;

      _Alloc_traits::_S_on_swap(_M_get_allocator(), __s._M_get_allocator());

      if (_M_is_local())
	if (__s._M_is_local())
	  {
	    if (length() && __s.length())
	      {
		_CharT __tmp_data[_S_local_capacity + 1];
		traits_type::copy(__tmp_data, __s._M_local_buf,
				  _S_local_capacity + 1);
		traits_type::copy(__s._M_local_buf, _M_local_buf,
				  _S_local_capacity + 1);
		traits_type::copy(_M_local_buf, __tmp_data,
				  _S_local_capacity + 1);
	      }
	    else if (__s.length())
	      {
		traits_type::copy(_M_local_buf, __s._M_local_buf,
				  _S_local_capacity + 1);
		_M_length(__s.length());
		__s._M_set_length(0);
		return;
	      }
	    else if (length())
	      {
		traits_type::copy(__s._M_local_buf, _M_local_buf,
				  _S_local_capacity + 1);
		__s._M_length(length());
		_M_set_length(0);
		return;
	      }
	  }
	else
	  {
	    const size_type __tmp_capacity = __s._M_allocated_capacity;
	    traits_type::copy(__s._M_local_buf, _M_local_buf,
			      _S_local_capacity + 1);
	    _M_data(__s._M_data());
	    __s._M_data(__s._M_local_buf);
	    _M_capacity(__tmp_capacity);
	  }
      else
	{
	  const size_type __tmp_capacity = _M_allocated_capacity;
	  if (__s._M_is_local())
	    {
	      traits_type::copy(_M_local_buf, __s._M_local_buf,
				_S_local_capacity + 1);
	      __s._M_data(_M_data());
	      _M_data(_M_local_buf);
	    }
	  else
	    {
	      pointer __tmp_ptr = _M_data();
	      _M_data(__s._M_data());
	      __s._M_data(__tmp_ptr);
	      _M_capacity(__s._M_allocated_capacity);
	    }
	  __s._M_capacity(__tmp_capacity);
	}

      const size_type __tmp_length = length();
      _M_length(__s.length());
      __s._M_length(__tmp_length);
    }

  template<typename _CharT, typename _Traits, typename _Alloc>
    typename basic_string<_CharT, _Traits, _Alloc>::pointer
    basic_string<_CharT, _Traits, _Alloc>::
    _M_create(size_type& __capacity, siz