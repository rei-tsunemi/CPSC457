// Components for compile-time parsing of numbers -*- C++ -*-

// Copyright (C) 2013-2019 Free Software Foundation, Inc.
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

/** @file bits/parse_numbers.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{chrono}
 */

#ifndef _GLIBCXX_PARSE_NUMBERS_H
#define _GLIBCXX_PARSE_NUMBERS_H 1

#pragma GCC system_header

// From n3642.pdf except I added binary literals and digit separator '\''.

#if __cplusplus > 201103L

#include <limits>

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

namespace __parse_int
{
  template<unsigned _Base, char _Dig>
    struct _Digit;

  template<unsigned _Base>
    struct _Digit<_Base, '0'> : integral_constant<unsigned, 0>
    {
      using __valid = true_type;
    };

  template<unsigned _Base>
    struct _Digit<_Base, '1'> : integral_constant<unsigned, 1>
    {
      using __valid = true_type;
    };

  template<unsigned _Base, unsigned _Val>
    struct _Digit_impl : integral_constant<unsigned, _Val>
    {
      static_assert(_Base > _Val, "invalid digit");
      using __valid = true_type;
    };

  template<unsigned _Base>
    struct _Digit<_Base, '2'> : _Digit_impl<_Base, 2>
    { };

  template<unsigned _Base>
    struct _Digit<_Base, '3'> : _Digit_impl<_Base, 3>
    { };

  template<unsigned _Base>
    struct _Digit<_Base, '4'> : _Digit_impl<_Base, 4>
    { };

  template<unsigned _Base>
    struct _Digit<_Base, '5'> : _Digit_impl<_Base, 5>
    { };

  template<unsigned _Base>
    struct _Digit<_Base, '6'> : _Digit_impl<_Base, 6>
    { };

  template<unsigned _Base>
    struct _Digit<_Base, '7'> : _Digit_impl<_Base, 7>
    { };

  template<unsigned _Base>
    struct _Digit<_Base, '8'> : _Digit_impl<_Base, 8>
    { };

  template<unsigned _Base>
    struct _Digit<_Base, '9'> : _Digit_impl<_Base, 9>
    { };

  template<unsigned _Base>
    struct _Digit<_Base, 'a'> : _Digit_impl<_Base, 0xa>
    { };

  template<unsigned _Base>
    struct _Digit<_Base, 'A'> : _Digit_impl<_Base, 0xa>
    { };

  template<unsigned _Base>
    struct _Digit<_Base, 'b'> : _Digit_impl<_Base, 0xb>
    { };

  template<unsigned _Base>
    struct _Digit<_Base, 'B'> : _Digit_impl<_Base, 0xb>
    { };

  template<unsigned _Base>
    struct _Digit<_Base, 'c'> : _Digit_impl<_Base, 0xc>
    { };

  template<unsigned _Base>
    struct _Digit<_Base, 'C'> : _Digit_impl<_Base, 0xc>
    { };

  template<unsigned _Base>
    struct _Digit<_Base, 'd'> : _Digit_impl<_Base, 0xd>
    { };

  template<unsigned _Base>
    struct _Digit<_Base, 'D'> : _Digit_impl<_Base, 0xd>
    { };

  template<unsigned _Base>
    struct _Digit<_Base, 'e'> : _Digit_impl<_Base, 0xe>
    { };

  template<unsigned _Base>
    struct _Digit<_Base, 'E'> : _Digit_impl<_Base, 0xe>
    { };

  template<unsigned _Base>
    struct _Digit<_Base, 'f'> : _Digit_impl<_Base, 0xf>
    { };

  template<unsigned _Base>
    struct _Digit<_Base, 'F'> : _Digit_impl<_Base, 0xf>
    { };

  //  Digit separator
  template<unsigned _Base>
    struct _Digit<_Base, '\''> : integral_constant<unsigned, 0>
    {
      usin