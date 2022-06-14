// Versatile string -*- C++ -*-

// Copyright (C) 2005-2019 Free Software Foundation, Inc.
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

/** @file ext/vstring.tcc
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{ext/vstring.h}
 */

#ifndef _VSTRING_TCC
#define _VSTRING_TCC 1

#pragma GCC system_header

#include <bits/cxxabi_forced.h>

namespace __gnu_cxx _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  template<typename _CharT, typename _Traits, typename _Alloc,
	   template <typename, typename, typename> class _Base>
    const typename __versa_string<_CharT, _Traits, _Alloc, _Base>::size_type
    __versa_string<_CharT, _Traits, _Alloc, _Base>::npos;

  template<typename _CharT, typename _Traits, typename _Alloc,
	   template <typename, typename, typename> class _Base>
    void
    __versa_string<_CharT, _Traits, _Alloc, _Base>::
    resize(size_type __n, _CharT __c)
    {
      const size_type __size = this->size();
      if (__size < __n)
	this->append(__n - __size, __c);
      else if (__n < __size)
	this->_M_erase(__n, __size - __n);
    }

  template<typename _CharT, typename _Traits, typename _Alloc,
	   template <typename, typename, typename> class _Base>
    __versa_string<_CharT, _Traits, _Alloc, _Base>&
    __versa_string<_CharT, _Traits, _Alloc, _Base>::
    _M_append(const _CharT* __s, size_type __n)
    {
      const size_type __len = __n + this->size();

      if (__len <= this->capacity() && !this->_M_is_shared())
	{
	  if (__n)
	    this->_S_copy(this->_M_data() + this->size(), __s, __n);
	}
      else
	this->_M_mutate(this->size(), size_type(0), __s, __n);

      this->_M_set_length(__len);
      return *this;
    }

  template<typename _CharT, typename _Traits, typename _Alloc,
	   template <typename, typename, typename> class _Base>
    template<typename _InputIterator>
      __versa_string<_CharT, _Traits, _Alloc, _Base>&
      __versa_string<_CharT, _Traits, _Alloc, _Base>::
      _M_replace_dispatch(const_iterator __i1, const_iterator __i2,
			  _InputIterator __k1, _InputIterator __k2,
			  std::__false_type)
      {
	const __versa_string __s(__k1, __k2);
	const size_type __n1 = __i2 - __i1;
	return _M_replace(__i1 - _M_ibegin(), __n1, __s._M_data(),
			  __s.size());
      }

  template<typename _CharT, typename _Traits, typename _Alloc,
	   template <typename, typename, typename> class _Base>
    __versa_string<_CharT, _Traits, _Alloc, _Base>&
    __versa_string<_CharT, _Traits, _Alloc, _Base>::
    _M_replace_aux(size_type __pos1, size_type __n1, size_type __n2,
		   _CharT __c)
    {
      _M_check_length(__n1, __n2, "__versa_string::_M_replace_aux");

      const size_type __old_size = this->size();
      const size_type __new_size = __old_size + __n2 - __n1;

      if (__new_size <= this->capacity() && !this->_M_is_shared())
	{
	  _CharT* __p = this->_M_data() + __pos1;

	  const size_type __how_much = __old_size - __pos1 - __n1;
	  if (__how_much && __n1 != __n2)
	    this->_S_move(__p + __n2, __p + __n1, __how_much);
	}
      else
	this->_M_mutate(__pos1, __n1, 0, __n2);

      if (