// Iostreams base classes -*- C++ -*-

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

/** @file bits/basic_ios.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{ios}
 */

#ifndef _BASIC_IOS_H
#define _BASIC_IOS_H 1

#pragma GCC system_header

#include <bits/localefwd.h>
#include <bits/locale_classes.h>
#include <bits/locale_facets.h>
#include <bits/streambuf_iterator.h>
#include <bits/move.h>

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  template<typename _Facet>
    inline const _Facet&
    __check_facet(const _Facet* __f)
    {
      if (!__f)
	__throw_bad_cast();
      return *__f;
    }

  /**
   *  @brief Template class basic_ios, virtual base class for all
   *  stream classes. 
   *  @ingroup io
   *
   *  @tparam _CharT  Type of character stream.
   *  @tparam _Traits  Traits for character type, defaults to
   *                   char_traits<_CharT>.
   *
   *  Most of the member functions called dispatched on stream objects
   *  (e.g., @c std::cout.foo(bar);) are consolidated in this class.
  */
  template<typename _CharT, typename _Traits>
    class basic_ios : public ios_base
    {
    public:
      //@{
      /**
       *  These are standard types.  They permit a standardized way of
       *  referring to names of (or names dependent on) the template
       *  parameters, which are specific to the implementation.
      */
      typedef _CharT                                 char_type;
      typedef typename _Traits::int_type             int_type;
      typedef typename _Traits::pos_type             pos_type;
      typedef typename _Traits::off_type             off_type;
      typedef _Traits                                traits_type;
      //@}

      //@{
      /**
       *  These are non-standard types.
      */
      typedef ctype<_CharT>                          __ctype_type;
      typedef num_put<_CharT, ostreambuf_iterator<_CharT, _Traits> >
						     __num_put_type;
      typedef num_get<_CharT, istreambuf_iterator<_CharT, _Traits> >
						     __num_get_type;
      //@}

      // Data members:
    protected:
      basic_ostream<_CharT, _Traits>*                _M_tie;
      mutable char_type                              _M_fill;
      mutable bool                                   _M_fill_init;
      basic_streambuf<_CharT, _Traits>*              _M_streambuf;

      // Cached use_facet<ctype>, which is based on the current locale info.
      const __ctype_type*                            _M_ctype;
      // For ostream.
      const __num_put_type*                          _M_num_put;
      // For istream.
      const __num_get_type*                          _M_num_get;

    public:
      //@{
      /**
       *  @brief  The quick-and-easy status check.
       *
       *  This allows you to write constructs such as
       *  <code>if (!a_stream) ...</code> and <code>while (a_stream) ...</code>
      */
#if __cplusplus >= 201103L
      explicit operator bool() const
      { return !this->fail(); }
#else
      operator