// istream classes -*- C++ -*-

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

/** @file bits/istream.tcc
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{istream}
 */

//
// ISO C++ 14882: 27.6.1  Input streams
//

#ifndef _ISTREAM_TCC
#define _ISTREAM_TCC 1

#pragma GCC system_header

#include <bits/cxxabi_forced.h>

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  template<typename _CharT, typename _Traits>
    basic_istream<_CharT, _Traits>::sentry::
    sentry(basic_istream<_CharT, _Traits>& __in, bool __noskip) : _M_ok(false)
    {
      ios_base::iostate __err = ios_base::goodbit;
      if (__in.good())
	__try
	  {
	    if (__in.tie())
	      __in.tie()->flush();
	    if (!__noskip && bool(__in.flags() & ios_base::skipws))
	      {
		const __int_type __eof = traits_type::eof();
		__streambuf_type* __sb = __in.rdbuf();
		__int_type __c = __sb->sgetc();

		const __ctype_type& __ct = __check_facet(__in._M_ctype);
		while (!traits_type::eq_int_type(__c, __eof)
		       && __ct.is(ctype_base::space,
				  traits_type::to_char_type(__c)))
		  __c = __sb->snextc();

		// _GLIBCXX_RESOLVE_LIB_DEFECTS
		// 195. Should basic_istream::sentry's constructor ever
		// set eofbit?
		if (traits_type::eq_int_type(__c, __eof))
		  __err |= ios_base::eofbit;
	      }
	  }
	__catch(__cxxabiv1::__forced_unwind&)
	  {
	    __in._M_setstate(ios_base::badbit);
	    __throw_exception_again;
	  }
	__catch(...)
	  { __in._M_setstate(ios_base::badbit); }

      if (__in.good() && __err == ios_base::goodbit)
	_M_ok = true;
      else
	{
	  __err |= ios_base::failbit;
	  __in.setstate(__err);
	}
    }

  template<typename _CharT, typename _Traits>
    template<typename _ValueT>
      basic_istream<_CharT, _Traits>&
      basic_istream<_CharT, _Traits>::
      _M_extract(_ValueT& __v)
      {
	sentry __cerb(*this, false);
	if (__cerb)
	  {
	    ios_base::iostate __err = ios_base::goodbit;
	    __try
	      {
		const __num_get_type& __ng = __check_facet(this->_M_num_get);
		__ng.get(*this, 0, *this, __err, __v);
	      }
	    __catch(__cxxabiv1::__forced_unwind&)
	      {
		this->_M_setstate(ios_base::badbit);
		__throw_exception_again;
	      }
	    __catch(...)
	      { this->_M_setstate(ios_base::badbit); }
	    if (__err)
	      this->setstate(__err);
	  }
	return *this;
      }

  template<typename _CharT, typename _Traits>
    basic_istream<_CharT, _Traits>&
    basic_istream<_CharT, _Traits>::
    operator>>(short& __n)
    {
      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // 118. basic_istream uses nonexistent num_get member functions.
      sentry __cerb(*this, false);
      if (__cerb)
	{
	  ios_base::iostate __err = ios_base::goodbit;
	  __try
	    {
	      long __l;
	      const __num_get_type& __ng = __check_facet(this->_M_num_get);
	      __ng.get(*this, 0, *this, __err, __l);

	      // _GLIBCXX_RESOLVE_LIB_DEFECTS
	      // 696. istream::operator>>(int&) broken.
	      if (__l < __gnu_cxx::__numeric_traits<short>::__min)
		{
		  __err |= ios_base::failbit;