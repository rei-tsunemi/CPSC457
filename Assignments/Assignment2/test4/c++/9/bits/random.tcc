// random number generation (out of line) -*- C++ -*-

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

/** @file bits/random.tcc
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{random}
 */

#ifndef _RANDOM_TCC
#define _RANDOM_TCC 1

#include <numeric> // std::accumulate and std::partial_sum

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  /*
   * (Further) implementation-space details.
   */
  namespace __detail
  {
    // General case for x = (ax + c) mod m -- use Schrage's algorithm
    // to avoid integer overflow.
    //
    // Preconditions:  a > 0, m > 0.
    //
    // Note: only works correctly for __m % __a < __m / __a.
    template<typename _Tp, _Tp __m, _Tp __a, _Tp __c>
      _Tp
      _Mod<_Tp, __m, __a, __c, false, true>::
      __calc(_Tp __x)
      {
	if (__a == 1)
	  __x %= __m;
	else
	  {
	    static const _Tp __q = __m / __a;
	    static const _Tp __r = __m % __a;

	    _Tp __t1 = __a * (__x % __q);
	    _Tp __t2 = __r * (__x / __q);
	    if (__t1 >= __t2)
	      __x = __t1 - __t2;
	    else
	      __x = __m - __t2 + __t1;
	  }

	if (__c != 0)
	  {
	    const _Tp __d = __m - __x;
	    if (__d > __c)
	      __x += __c;
	    else
	      __x = __c - __d;
	  }
	return __x;
      }

    template<typename _InputIterator, typename _OutputIterator,
	     typename _Tp>
      _OutputIterator
      __normalize(_InputIterator __first, _InputIterator __last,
		  _OutputIterator __result, const _Tp& __factor)
      {
	for (; __first != __last; ++__first, ++__result)
	  *__result = *__first / __factor;
	return __result;
      }

  } // namespace __detail

  template<typename _UIntType, _UIntType __a, _UIntType __c, _UIntType __m>
    constexpr _UIntType
    linear_congruential_engine<_UIntType, __a, __c, __m>::multiplier;

  template<typename _UIntType, _UIntType __a, _UIntType __c, _UIntType __m>
    constexpr _UIntType
    linear_congruential_engine<_UIntType, __a, __c, __m>::increment;

  template<typename _UIntType, _UIntType __a, _UIntType __c, _UIntType __m>
    constexpr _UIntType
    linear_congruential_engine<_UIntType, __a, __c, __m>::modulus;

  template<typename _UIntType, _UIntType __a, _UIntType __c, _UIntType __m>
    constexpr _UIntType
    linear_congruential_engine<_UIntType, __a, __c, __m>::default_seed;

  /**
   * Seeds the LCR with integral value @p __s, adjusted so that the
   * ring identity is never a member of the convergence set.
   */
  template<typename _UIntType, _UIntType __a, _UIntType __c, _UIntType __m>
    void
    linear_congruential_engine<_UIntType, __a, __c, __m>::
    seed(result_type __s)
    {
      if ((__detail::__mod<_UIntType, __m>(__c) == 0)
	  && (__detail::__mod<_UIntType, __m>(__s) == 0))
	_M_x = 1;
      else
	_M_x = __detail::__mod<_UIntType, __m>(__s);
    }

  /**
   * Seeds the LCR engine with a value generated by @p __q.
   */
  template<typename _UIntType, _UIntType __a, _UIntType __c, _UIntType __m>
    template<typename _Sseq>
      auto
      linear_congruential_engine<_UIntType, 