// random number generation -*- C++ -*-

// Copyright (C) 2009-2019 Free Software Foundation, Inc.
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

/**
 * @file bits/random.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{random}
 */

#ifndef _RANDOM_H
#define _RANDOM_H 1

#include <vector>
#include <bits/uniform_int_dist.h>

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  // [26.4] Random number generation

  /**
   * @defgroup random Random Number Generation
   * @ingroup numerics
   *
   * A facility for generating random numbers on selected distributions.
   * @{
   */

  /**
   * @brief A function template for converting the output of a (integral)
   * uniform random number generator to a floatng point result in the range
   * [0-1).
   */
  template<typename _RealType, size_t __bits,
	   typename _UniformRandomNumberGenerator>
    _RealType
    generate_canonical(_UniformRandomNumberGenerator& __g);

  /*
   * Implementation-space details.
   */
  namespace __detail
  {
    template<typename _UIntType, size_t __w,
	     bool = __w < static_cast<size_t>
			  (std::numeric_limits<_UIntType>::digits)>
      struct _Shift
      { static const _UIntType __value = 0; };

    template<typename _UIntType, size_t __w>
      struct _Shift<_UIntType, __w, true>
      { static const _UIntType __value = _UIntType(1) << __w; };

    template<int __s,
	     int __which = ((__s <= __CHAR_BIT__ * sizeof (int))
			    + (__s <= __CHAR_BIT__ * sizeof (long))
			    + (__s <= __CHAR_BIT__ * sizeof (long long))
			    /* assume long long no bigger than __int128 */
			    + (__s <= 128))>
      struct _Select_uint_least_t
      {
	static_assert(__which < 0, /* needs to be dependent */
		      "sorry, would be too much trouble for a slow result");
      };

    template<int __s>
      struct _Select_uint_least_t<__s, 4>
      { typedef unsigned int type; };

    template<int __s>
      struct _Select_uint_least_t<__s, 3>
      { typedef unsigned long type; };

    template<int __s>
      struct _Select_uint_least_t<__s, 2>
      { typedef unsigned long long type; };

#ifdef _GLIBCXX_USE_INT128
    template<int __s>
      struct _Select_uint_least_t<__s, 1>
      { typedef unsigned __int128 type; };
#endif

    // Assume a != 0, a < m, c < m, x < m.
    template<typename _Tp, _Tp __m, _Tp __a, _Tp __c,
	     bool __big_enough = (!(__m & (__m - 1))
				  || (_Tp(-1) - __c) / __a >= __m - 1),
             bool __schrage_ok = __m % __a < __m / __a>
      struct _Mod
      {
	typedef typename _Select_uint_least_t<std::__lg(__a)
					      + std::__lg(__m) + 2>::type _Tp2;
	static _Tp
	__calc(_Tp __x)
	{ return static_cast<_Tp>((_Tp2(__a) * __x + __c) % __m); }
      };

    // Schrage.
    template<typename _Tp, _Tp __m, _Tp __a, _Tp __c>
      struct _Mod<_Tp, __m, __a, __c, false, true>
      {
	static _Tp
	__calc(_Tp __x);
      };

    // Special cases:
    // - for m == 2^n or m == 0, unsigned integer overflow is safe.
    // - a * (m - 1) + c fits in _Tp, there is no overflow.
    template<typename _Tp, _Tp __m, _T