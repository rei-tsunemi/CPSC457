// Versatile string utility -*- C++ -*-

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

/** @file ext/vstring_util.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{ext/vstring.h}
 */

#ifndef _VSTRING_UTIL_H
#define _VSTRING_UTIL_H 1

#pragma GCC system_header

#include <ext/vstring_fwd.h>
#include <debug/debug.h>
#include <bits/stl_function.h>  // For less
#include <bits/functexcept.h>
#include <bits/localefwd.h>
#include <bits/ostream_insert.h>
#include <bits/stl_iterator.h>
#include <ext/numeric_traits.h>
#include <bits/move.h>
#include <bits/range_access.h>

namespace __gnu_cxx _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  template<typename _CharT, typename _Traits, typename _Alloc>
    struct __vstring_utility
    {
      typedef typename _Alloc::template rebind<_CharT>::other _CharT_alloc_type;

      typedef _Traits					    traits_type;
      typedef typename _Traits::char_type		    value_type;
      typedef typename _CharT_alloc_type::size_type	    size_type;
      typedef typename _CharT_alloc_type::difference_type   difference_type;
      typedef typename _CharT_alloc_type::pointer	    pointer;
      typedef typename _CharT_alloc_type::const_pointer	    const_pointer;

      // For __sso_string.
      typedef __gnu_cxx::
      __normal_iterator<pointer, __gnu_cxx::
			__versa_string<_CharT, _Traits, _Alloc,
				       __sso_string_base> >
	__sso_iterator;
      typedef __gnu_cxx::
      __normal_iterator<const_pointer, __gnu_cxx::
			__versa_string<_CharT, _Traits, _Alloc,
				       __sso_string_base> >
	__const_sso_iterator;

      // For __rc_string.
      typedef __gnu_cxx::
      __normal_iterator<pointer, __gnu_cxx::
			__versa_string<_CharT, _Traits, _Alloc,
				       __rc_string_base> >
	__rc_iterator;
      typedef __gnu_cxx::
      __normal_iterator<const_pointer, __gnu_cxx::
			__versa_string<_CharT, _Traits, _Alloc,
				       __rc_string_base> >
	__const_rc_iterator;

      // NB:  When the allocator is empty, deriving from it saves space
      // (http://www.cantrip.org/emptyopt.html).
      template<typename _Alloc1>
	struct _Alloc_hider
	: public _Alloc1
	{
	  _Alloc_hider(_CharT* __ptr)
	  : _Alloc1(), _M_p(__ptr) { }

	  _Alloc_hider(const _Alloc1& __a, _CharT* __ptr)
	  : _Alloc1(__a), _M_p(__ptr) { }

	  _CharT*  _M_p; // The actual data.
	};

      // When __n = 1 way faster than the general multichar
      // traits_type::copy/move/assign.
      static void
      _S_copy(_CharT* __d, const _CharT* __s, size_type __n)
      {
	if (__n == 1)
	  traits_type::assign(*__d, *__s);
	else
	  traits_type::copy(__d, __s, __n);
      }

      static void
      _S_move(_CharT* __d, const _CharT* __s, size_type __n)
      {
	if (__n == 1)
	  traits_type::assign(*__d, *__s);
	else
	  traits_type::move(__d, __s, __n);
      }

      static void
      _S_assign(_CharT* __d, size_type __n, _CharT __c)
      {
	if (__n == 1)
	  traits_type::assign(*__d, __c);
	else
	  traits_type::assign(__d, __n, __c);
      }

      // _S_copy_chars is