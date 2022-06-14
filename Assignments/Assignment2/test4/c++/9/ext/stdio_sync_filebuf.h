// Iostreams wrapper for stdio FILE* -*- C++ -*-

// Copyright (C) 2003-2019 Free Software Foundation, Inc.
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

/** @file ext/stdio_sync_filebuf.h
 *  This file is a GNU extension to the Standard C++ Library.
 */

#ifndef _STDIO_SYNC_FILEBUF_H
#define _STDIO_SYNC_FILEBUF_H 1

#pragma GCC system_header

#include <streambuf>
#include <unistd.h>
#include <cstdio>
#include <bits/c++io.h>  // For __c_file
#include <bits/move.h>   // For __exchange

#ifdef _GLIBCXX_USE_WCHAR_T
#include <cwchar>
#endif

namespace __gnu_cxx _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  /**
   *  @brief Provides a layer of compatibility for C.
   *  @ingroup io
   *
   *  This GNU extension provides extensions for working with standard
   *  C FILE*'s.  It must be instantiated by the user with the type of
   *  character used in the file stream, e.g., stdio_filebuf<char>.
  */
  template<typename _CharT, typename _Traits = std::char_traits<_CharT> >
    class stdio_sync_filebuf : public std::basic_streambuf<_CharT, _Traits>
    {
    public:
      // Types:
      typedef _CharT					char_type;
      typedef _Traits					traits_type;
      typedef typename traits_type::int_type		int_type;
      typedef typename traits_type::pos_type		pos_type;
      typedef typename traits_type::off_type		off_type;

    private:
      typedef std::basic_streambuf<_CharT, _Traits> __streambuf_type;

      // Underlying stdio FILE
      std::__c_file* _M_file;

      // Last character gotten. This is used when pbackfail is
      // called from basic_streambuf::sungetc()
      int_type _M_unget_buf;

    public:
      explicit
      stdio_sync_filebuf(std::__c_file* __f)
      : _M_file(__f), _M_unget_buf(traits_type::eof())
      { }

#if __cplusplus >= 201103L
      stdio_sync_filebuf(stdio_sync_filebuf&& __fb) noexcept
      : __streambuf_type(std::move(__fb)),
      _M_file(__fb._M_file), _M_unget_buf(__fb._M_unget_buf)
      {
	__fb._M_file = nullptr;
	__fb._M_unget_buf = traits_type::eof();
      }

      stdio_sync_filebuf&
      operator=(stdio_sync_filebuf&& __fb) noexcept
      {
	__streambuf_type::operator=(__fb);
	_M_file = std::__exchange(__fb._M_file, nullptr);
	_M_unget_buf = std::__exchange(__fb._M_unget_buf, traits_type::eof());
	return *this;
      }

      void
      swap(stdio_sync_filebuf& __fb)
      {
	__streambuf_type::swap(__fb);
	std::swap(_M_file, __fb._M_file);
	std::swap(_M_unget_buf, __fb._M_unget_buf);
      }
#endif

      /**
       *  @return  The underlying FILE*.
       *
       *  This function can be used to access the underlying C file pointer.
       *  Note that there is no way for the library to track what you do
       *  with the file, so be careful.
       */
      std::__c_file*
      file() { return this->_M_file; }

    protected:
      int_type
      syncgetc();

      int_type
      syncungetc(int_type __c);

      int_type
      syncputc(int_type __c);

      virtual int_type
      underflow()
      {
	int_type __c = this->syncgetc();
	return this->syncungetc(__c);
      }

      virtual int_type
      uflow()
      {
	// Store the gotten 