// String based streams -*- C++ -*-

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

/** @file bits/sstream.tcc
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{sstream}
 */

//
// ISO C++ 14882: 27.7  String-based streams
//

#ifndef _SSTREAM_TCC
#define _SSTREAM_TCC 1

#pragma GCC system_header

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  template <class _CharT, class _Traits, class _Alloc>
    typename basic_stringbuf<_CharT, _Traits, _Alloc>::int_type
    basic_stringbuf<_CharT, _Traits, _Alloc>::
    pbackfail(int_type __c)
    {
      int_type __ret = traits_type::eof();
      if (this->eback() < this->gptr())
	{
	  // Try to put back __c into input sequence in one of three ways.
	  // Order these tests done in is unspecified by the standard.
	  const bool __testeof = traits_type::eq_int_type(__c, __ret);
	  if (!__testeof)
	    {
	      const bool __testeq = traits_type::eq(traits_type::
						    to_char_type(__c),
						    this->gptr()[-1]);	  
	      const bool __testout = this->_M_mode & ios_base::out;
	      if (__testeq || __testout)
		{
		  this->gbump(-1);
		  if (!__testeq)
		    *this->gptr() = traits_type::to_char_type(__c);
		  __ret = __c;
		}
	    }
	  else
	    {
	      this->gbump(-1);
	      __ret = traits_type::not_eof(__c);
	    }
	}
      return __ret;
    }

  template <class _CharT, class _Traits, class _Alloc>
    typename basic_stringbuf<_CharT, _Traits, _Alloc>::int_type
    basic_stringbuf<_CharT, _Traits, _Alloc>::
    overflow(int_type __c)
    {
      const bool __testout = this->_M_mode & ios_base::out;
      if (__builtin_expect(!__testout, false))
	return traits_type::eof();

      const bool __testeof = traits_type::eq_int_type(__c, traits_type::eof());
      if (__builtin_expect(__testeof, false))
	return traits_type::not_eof(__c);

      const __size_type __capacity = _M_string.capacity();

#if _GLIBCXX_USE_CXX11_ABI
      if ((this->epptr() - this->pbase()) < __capacity)
	{
	  // There is additional capacity in _M_string that can be used.
	  char_type* __base = const_cast<char_type*>(_M_string.data());
	  _M_pbump(__base, __base + __capacity, this->pptr() - this->pbase());
	  if (_M_mode & ios_base::in)
	    {
	      const __size_type __nget = this->gptr() - this->eback();
	      const __size_type __eget = this->egptr() - this->eback();
	      this->setg(__base, __base + __nget, __base + __eget + 1);
	    }
	  *this->pptr() = traits_type::to_char_type(__c);
	  this->pbump(1);
	  return __c;
	}
#endif

      const __size_type __max_size = _M_string.max_size();
      const bool __testput = this->pptr() < this->epptr();
      if (__builtin_expect(!__testput && __capacity == __max_size, false))
	return traits_type::eof();

      // Try to append __c into output sequence in one of two ways.
      // Order these tests done in is unspecified by the standard.
      const char_type __conv = traits_type::to_char_type(__c);
      if (!__testput)
	{
	  // NB: Start ostringstream buffers at 512 chars.  This i