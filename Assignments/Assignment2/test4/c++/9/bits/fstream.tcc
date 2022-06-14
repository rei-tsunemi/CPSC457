// File based streams -*- C++ -*-

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

/** @file bits/fstream.tcc
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{fstream}
 */

//
// ISO C++ 14882: 27.8  File-based streams
//

#ifndef _FSTREAM_TCC
#define _FSTREAM_TCC 1

#pragma GCC system_header

#include <bits/cxxabi_forced.h>
#include <bits/move.h>   // for swap
#include <cerrno>

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  template<typename _CharT, typename _Traits>
    void
    basic_filebuf<_CharT, _Traits>::
    _M_allocate_internal_buffer()
    {
      // Allocate internal buffer only if one doesn't already exist
      // (either allocated or provided by the user via setbuf).
      if (!_M_buf_allocated && !_M_buf)
	{
	  _M_buf = new char_type[_M_buf_size];
	  _M_buf_allocated = true;
	}
    }

  template<typename _CharT, typename _Traits>
    void
    basic_filebuf<_CharT, _Traits>::
    _M_destroy_internal_buffer() throw()
    {
      if (_M_buf_allocated)
	{
	  delete [] _M_buf;
	  _M_buf = 0;
	  _M_buf_allocated = false;
	}
      delete [] _M_ext_buf;
      _M_ext_buf = 0;
      _M_ext_buf_size = 0;
      _M_ext_next = 0;
      _M_ext_end = 0;
    }

  template<typename _CharT, typename _Traits>
    basic_filebuf<_CharT, _Traits>::
    basic_filebuf() : __streambuf_type(), _M_lock(), _M_file(&_M_lock),
    _M_mode(ios_base::openmode(0)), _M_state_beg(), _M_state_cur(),
    _M_state_last(), _M_buf(0), _M_buf_size(BUFSIZ),
    _M_buf_allocated(false), _M_reading(false), _M_writing(false), _M_pback(), 
    _M_pback_cur_save(0), _M_pback_end_save(0), _M_pback_init(false),
    _M_codecvt(0), _M_ext_buf(0), _M_ext_buf_size(0), _M_ext_next(0),
    _M_ext_end(0)
    {
      if (has_facet<__codecvt_type>(this->_M_buf_locale))
	_M_codecvt = &use_facet<__codecvt_type>(this->_M_buf_locale);
    }

#if __cplusplus >= 201103L
  template<typename _CharT, typename _Traits>
    basic_filebuf<_CharT, _Traits>::
    basic_filebuf(basic_filebuf&& __rhs)
    : __streambuf_type(__rhs),
    _M_lock(), _M_file(std::move(__rhs._M_file), &_M_lock),
    _M_mode(std::__exchange(__rhs._M_mode, ios_base::openmode(0))),
    _M_state_beg(std::move(__rhs._M_state_beg)),
    _M_state_cur(std::move(__rhs._M_state_cur)),
    _M_state_last(std::move(__rhs._M_state_last)),
    _M_buf(std::__exchange(__rhs._M_buf, nullptr)),
    _M_buf_size(std::__exchange(__rhs._M_buf_size, 1)),
    _M_buf_allocated(std::__exchange(__rhs._M_buf_allocated, false)),
    _M_reading(std::__exchange(__rhs._M_reading, false)),
    _M_writing(std::__exchange(__rhs._M_writing, false)),
    _M_pback(__rhs._M_pback),
    _M_pback_cur_save(std::__exchange(__rhs._M_pback_cur_save, nullptr)),
    _M_pback_end_save(std::__exchange(__rhs._M_pback_end_save, nullptr)),
    _M_pback_init(std::__exchange(__rhs._M_pback_init, false)),
    _M_codecvt(__rhs._M_codecvt),
    _M_ext_buf(std::__exchange(__rhs._M_ext_buf, nullptr)),
    _M_ext_buf_size(std::__exchange(__rhs._M_ext_buf_size, 0)),
    _M_ext_next