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

/** @file bits/ios_base.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{ios}
 */

//
// ISO C++ 14882: 27.4  Iostreams base classes
//

#ifndef _IOS_BASE_H
#define _IOS_BASE_H 1

#pragma GCC system_header

#include <ext/atomicity.h>
#include <bits/localefwd.h>
#include <bits/locale_classes.h>

#if __cplusplus < 201103L
# include <stdexcept>
#else
# include <system_error>
#endif

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  // The following definitions of bitmask types are enums, not ints,
  // as permitted (but not required) in the standard, in order to provide
  // better type safety in iostream calls.  A side effect is that in C++98
  // expressions involving them are not compile-time constants.
  enum _Ios_Fmtflags 
    { 
      _S_boolalpha 	= 1L << 0,
      _S_dec 		= 1L << 1,
      _S_fixed 		= 1L << 2,
      _S_hex 		= 1L << 3,
      _S_internal 	= 1L << 4,
      _S_left 		= 1L << 5,
      _S_oct 		= 1L << 6,
      _S_right 		= 1L << 7,
      _S_scientific 	= 1L << 8,
      _S_showbase 	= 1L << 9,
      _S_showpoint 	= 1L << 10,
      _S_showpos 	= 1L << 11,
      _S_skipws 	= 1L << 12,
      _S_unitbuf 	= 1L << 13,
      _S_uppercase 	= 1L << 14,
      _S_adjustfield 	= _S_left | _S_right | _S_internal,
      _S_basefield 	= _S_dec | _S_oct | _S_hex,
      _S_floatfield 	= _S_scientific | _S_fixed,
      _S_ios_fmtflags_end = 1L << 16,
      _S_ios_fmtflags_max = __INT_MAX__,
      _S_ios_fmtflags_min = ~__INT_MAX__
    };

  inline _GLIBCXX_CONSTEXPR _Ios_Fmtflags
  operator&(_Ios_Fmtflags __a, _Ios_Fmtflags __b)
  { return _Ios_Fmtflags(static_cast<int>(__a) & static_cast<int>(__b)); }

  inline _GLIBCXX_CONSTEXPR _Ios_Fmtflags
  operator|(_Ios_Fmtflags __a, _Ios_Fmtflags __b)
  { return _Ios_Fmtflags(static_cast<int>(__a) | static_cast<int>(__b)); }

  inline _GLIBCXX_CONSTEXPR _Ios_Fmtflags
  operator^(_Ios_Fmtflags __a, _Ios_Fmtflags __b)
  { return _Ios_Fmtflags(static_cast<int>(__a) ^ static_cast<int>(__b)); }

  inline _GLIBCXX_CONSTEXPR _Ios_Fmtflags
  operator~(_Ios_Fmtflags __a)
  { return _Ios_Fmtflags(~static_cast<int>(__a)); }

  inline const _Ios_Fmtflags&
  operator|=(_Ios_Fmtflags& __a, _Ios_Fmtflags __b)
  { return __a = __a | __b; }

  inline const _Ios_Fmtflags&
  operator&=(_Ios_Fmtflags& __a, _Ios_Fmtflags __b)
  { return __a = __a & __b; }

  inline const _Ios_Fmtflags&
  operator^=(_Ios_Fmtflags& __a, _Ios_Fmtflags __b)
  { return __a = __a ^ __b; }


  enum _Ios_Openmode 
    { 
      _S_app 		= 1L << 0,
      _S_ate 		= 1L << 1,
      _S_bin 		= 1L << 2,
      _S_in 		= 1L << 3,
      _S_out 		= 1L << 4,
      _S_trunc 		= 1L << 5,
      _S_ios_openmode_end = 1L << 16,
      _S_ios_openmode_max = __INT_MAX__,
      _S_ios_openmode_min = ~__INT_MAX__
    };

  inline _GLIBCXX_CONSTEXPR _Ios_Openmode
  operator&(_Ios_Openmode __a, _Ios_Openmode __b)
  { return _Ios_Openmode(static_cast<int>(__a) & static_cast<int>(__b));