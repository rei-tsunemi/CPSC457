// ostream classes -*- C++ -*-

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

/** @file bits/ostream.tcc
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{ostream}
 */

//
// ISO C++ 14882: 27.6.2  Output streams
//

#ifndef _OSTREAM_TCC
#define _OSTREAM_TCC 1

#pragma GCC system_header

#include <bits/cxxabi_forced.h>

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  template<typename _CharT, typename _Traits>
    basic_ostream<_CharT, _Traits>::sentry::
    sentry(basic_ostream<_CharT, _Traits>& __os)
    : _M_ok(false), _M_os(__os)
    {
      // XXX MT
      if (__os.tie() && __os.good())
	__os.tie()->flush();

      if (__os.good())
	_M_ok = true;
      else
	__os.setstate(ios_base::failbit);
    }

  template<typename _CharT, typename _Traits>
    template<typename _ValueT>
      basic_ostream<_CharT, _Traits>&
      basic_ostream<_CharT, _Traits>::
      _M_insert(_ValueT __v)
      {
	sentry __cerb(*this);
	if (__cerb)
	  {
	    ios_base::iostate __err = ios_base::goodbit;
	    __try
	      {
		const __num_put_type& __np = __check_facet(this->_M_num_put);
		if (__np.put(*this, *this, this->fill(), __v).failed())
		  __err |= ios_base::badbit;
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
    basic_ostream<_CharT, _Traits>&
    basic_ostream<_CharT, _Traits>::
    operator<<(short __n)
    {
      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // 117. basic_ostream uses nonexistent num_put member functions.
      const ios_base::fmtflags __fmt = this->flags() & ios_base::basefield;
      if (__fmt == ios_base::oct || __fmt == ios_base::hex)
	return _M_insert(static_cast<long>(static_cast<unsigned short>(__n)));
      else
	return _M_insert(static_cast<long>(__n));
    }

  template<typename _CharT, typename _Traits>
    basic_ostream<_CharT, _Traits>&
    basic_ostream<_CharT, _Traits>::
    operator<<(int __n)
    {
      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // 117. basic_ostream uses nonexistent num_put member functions.
      const ios_base::fmtflags __fmt = this->flags() & ios_base::basefield;
      if (__fmt == ios_base::oct || __fmt == ios_base::hex)
	return _M_insert(static_cast<long>(static_cast<unsigned int>(__n)));
      else
	return _M_insert(static_cast<long>(__n));
    }
  
  template<typename _CharT, typename _Traits>
    basic_ostream<_CharT, _Traits>&
    basic_ostream<_CharT, _Traits>::
    operator<<(__streambuf_type* __sbin)
    {
      ios_base::iostate __err = ios_base::goodbit;
      sentry __cerb(*this);
      if (__cerb && __sbin)
	{
	  __try
	    {
	      if (!__copy_streambufs(__sbin, this->rdbuf()))
		__err |= ios_base::failbit;
	    }
	  __catch(__cxxabiv1::__forced_unwind&)
	    {
	      this->_M_setstate(ios_base::badbit);		
	      __throw_e