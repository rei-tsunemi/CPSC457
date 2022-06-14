// Special functions -*- C++ -*-

// Copyright (C) 2006-2019 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

/** @file tr1/gamma.tcc
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{tr1/cmath}
 */

//
// ISO C++ 14882 TR1: 5.2  Special functions
//

// Written by Edward Smith-Rowland based on:
//   (1) Handbook of Mathematical Functions,
//       ed. Milton Abramowitz and Irene A. Stegun,
//       Dover Publications,
//       Section 6, pp. 253-266
//   (2) The Gnu Scientific Library, http://www.gnu.org/software/gsl
//   (3) Numerical Recipes in C, by W. H. Press, S. A. Teukolsky,
//       W. T. Vetterling, B. P. Flannery, Cambridge University Press (1992),
//       2nd ed, pp. 213-216
//   (4) Gamma, Exploring Euler's Constant, Julian Havil,
//       Princeton, 2003.

#ifndef _GLIBCXX_TR1_GAMMA_TCC
#define _GLIBCXX_TR1_GAMMA_TCC 1

#include <tr1/special_function_util.h>

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

#if _GLIBCXX_USE_STD_SPEC_FUNCS
# define _GLIBCXX_MATH_NS ::std
#elif defined(_GLIBCXX_TR1_CMATH)
namespace tr1
{
# define _GLIBCXX_MATH_NS ::std::tr1
#else
# error do not include this header directly, use <cmath> or <tr1/cmath>
#endif
  // Implementation-space details.
  namespace __detail
  {
    /**
     *   @brief This returns Bernoulli numbers from a table or by summation
     *          for larger values.
     *
     *   Recursion is unstable.
     *
     *   @param __n the order n of the Bernoulli number.
     *   @return  The Bernoulli number of order n.
     */
    template <typename _Tp>
    _Tp
    __bernoulli_series(unsigned int __n)
    {

      static const _Tp __num[28] = {
        _Tp(1UL),                        -_Tp(1UL) / _Tp(2UL),
        _Tp(1UL) / _Tp(6UL),             _Tp(0UL),
        -_Tp(1UL) / _Tp(30UL),           _Tp(0UL),
        _Tp(1UL) / _Tp(42UL),            _Tp(0UL),
        -_Tp(1UL) / _Tp(30UL),           _Tp(0UL),
        _Tp(5UL) / _Tp(66UL),            _Tp(0UL),
        -_Tp(691UL) / _Tp(2730UL),       _Tp(0UL),
        _Tp(7UL) / _Tp(6UL),             _Tp(0UL),
        -_Tp(3617UL) / _Tp(510UL),       _Tp(0UL),
        _Tp(43867UL) / _Tp(798UL),       _Tp(0UL),
        -_Tp(174611) / _Tp(330UL),       _Tp(0UL),
        _Tp(854513UL) / _Tp(138UL),      _Tp(0UL),
        -_Tp(236364091UL) / _Tp(2730UL), _Tp(0UL),
        _Tp(8553103UL) / _Tp(6UL),       _Tp(0UL)
      };

      if (__n == 0)
        return _Tp(1);

      if (__n == 1)
        return -_Tp(1) / _Tp(2);

      //  Take care of the rest of the odd ones.
      if (__n % 2 == 1)
        return _Tp(0);

      //  Take care of some small evens that are painful for the series.
      if (__n < 28)
        return __num[__n];


      _Tp __fact = _Tp(1);
      if ((__n / 2) % 2 == 0)
        __fact *= _Tp(-1);
      for (unsigned int __k = 1; __k <= __n; ++__k)
        __fact *= __k / (_Tp(2) * __numeric_constants<_Tp>::__pi());
      __fact *= _Tp(2);

      _Tp __sum = _Tp(0);
      for (unsigned int __i = 1; __i < 1000; ++__i)
        {
          _Tp __term 