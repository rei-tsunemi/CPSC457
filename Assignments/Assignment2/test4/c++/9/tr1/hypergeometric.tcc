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

/** @file tr1/hypergeometric.tcc
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{tr1/cmath}
 */

//
// ISO C++ 14882 TR1: 5.2  Special functions
//

// Written by Edward Smith-Rowland based:
//   (1) Handbook of Mathematical Functions,
//       ed. Milton Abramowitz and Irene A. Stegun,
//       Dover Publications,
//       Section 6, pp. 555-566
//   (2) The Gnu Scientific Library, http://www.gnu.org/software/gsl

#ifndef _GLIBCXX_TR1_HYPERGEOMETRIC_TCC
#define _GLIBCXX_TR1_HYPERGEOMETRIC_TCC 1

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
  // [5.2] Special functions

  // Implementation-space details.
  namespace __detail
  {
    /**
     *   @brief This routine returns the confluent hypergeometric function
     *          by series expansion.
     * 
     *   @f[
     *     _1F_1(a;c;x) = \frac{\Gamma(c)}{\Gamma(a)}
     *                      \sum_{n=0}^{\infty}
     *                      \frac{\Gamma(a+n)}{\Gamma(c+n)}
     *                      \frac{x^n}{n!}
     *   @f]
     * 
     *   If a and b are integers and a < 0 and either b > 0 or b < a
     *   then the series is a polynomial with a finite number of
     *   terms.  If b is an integer and b <= 0 the confluent
     *   hypergeometric function is undefined.
     *
     *   @param  __a  The "numerator" parameter.
     *   @param  __c  The "denominator" parameter.
     *   @param  __x  The argument of the confluent hypergeometric function.
     *   @return  The confluent hypergeometric function.
     */
    template<typename _Tp>
    _Tp
    __conf_hyperg_series(_Tp __a, _Tp __c, _Tp __x)
    {
      const _Tp __eps = std::numeric_limits<_Tp>::epsilon();

      _Tp __term = _Tp(1);
      _Tp __Fac = _Tp(1);
      const unsigned int __max_iter = 100000;
      unsigned int __i;
      for (__i = 0; __i < __max_iter; ++__i)
        {
          __term *= (__a + _Tp(__i)) * __x
                  / ((__c + _Tp(__i)) * _Tp(1 + __i));
          if (std::abs(__term) < __eps)
            {
              break;
            }
          __Fac += __term;
        }
      if (__i == __max_iter)
        std::__throw_runtime_error(__N("Series failed to converge "
                                       "in __conf_hyperg_series."));

      return __Fac;
    }


    /**
     *  @brief  Return the hypogeometric function @f$ _2F_1(a,b;c;x) @f$
     *          by an iterative procedure described in
     *          Luke, Algorithms for the Computation of Mathematical Functions.
     *
     *  Like the case of the 2F1 rational approximations, these are 
     *  probably guaranteed to converge for x < 0, barring gross    
     *  numerical instability in the pre-asymptotic regime.      