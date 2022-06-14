// POD character, std::char_traits specialization -*- C++ -*-

// Copyright (C) 2002-2019 Free Software Foundation, Inc.
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

/** @file ext/pod_char_traits.h
 *  This file is a GNU extension to the Standard C++ Library.
 */

// Gabriel Dos Reis <gdr@integrable-solutions.net>
// Benjamin Kosnik <bkoz@redhat.com>

#ifndef _POD_CHAR_TRAITS_H
#define _POD_CHAR_TRAITS_H 1

#pragma GCC system_header

#include <string>

namespace __gnu_cxx _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  // POD character abstraction.
  // NB: The char_type parameter is a subset of int_type, as to allow
  // int_type to properly hold the full range of char_type values as
  // well as EOF.
  /// @brief A POD class that serves as a character abstraction class.
  template<typename _Value, typename _Int, typename _St = std::mbstate_t>
    struct character
    {
      typedef _Value				value_type;
      typedef _Int				int_type;
      typedef _St				state_type;
      typedef character<_Value, _Int, _St>	char_type;

      value_type	value;

      template<typename V2>
        static char_type
        from(const V2& v)
        {
	  char_type ret = { static_cast<value_type>(v) };
	  return ret;
	}

      template<typename V2>
        static V2
        to(const char_type& c)
        {
	  V2 ret = { static_cast<V2>(c.value) };
	  return ret;
	}

    };

  template<typename _Value, typename _Int, typename _St>
    inline bool
    operator==(const character<_Value, _Int, _St>& lhs,
	       const character<_Value, _Int, _St>& rhs)
    { return lhs.value == rhs.value; }

  template<typename _Value, typename _Int, typename _St>
    inline bool
    operator<(const character<_Value, _Int, _St>& lhs,
	      const character<_Value, _Int, _St>& rhs)
    { return lhs.value < rhs.value; }

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  /// char_traits<__gnu_cxx::character> specialization.
  template<typename _Value, typename _Int, typename _St>
    struct char_traits<__gnu_cxx::character<_Value, _Int, _St> >
    {
      typedef __gnu_cxx::character<_Value, _Int, _St>	char_type;
      typedef typename char_type::int_type		int_type;
      typedef typename char_type::state_type		state_type;
      typedef fpos<state_type>				pos_type;
      typedef streamoff					off_type;

      static void
      assign(char_type& __c1, const char_type& __c2)
      { __c1 = __c2; }

      static bool
      eq(const char_type& __c1, const char_type& __c2)
      { return __c1 == __c2; }

      static bool
      lt(const char_type& __c1, const char_type& __c2)
      { return __c1 < __c2; }

      static int
      compare(const char_type* __s1, const char_type* __s2, size_t __n)
      {
	for (size_t __i = 0; __i < __n; ++__i)
	  if (!eq(__s1[__i], __s2[__i]))
	    return lt(__s1[__i], __s2[__i]) ? -1 : 1;
	return 0;
      }

      static size_t
      length(const char_type* __s)
      {
	const char_type* __p = __s;
	while (__p->value)
	  ++__p;
	return (__p - __s);
      }

      static const char_type*
      find(const char_type* __s, 