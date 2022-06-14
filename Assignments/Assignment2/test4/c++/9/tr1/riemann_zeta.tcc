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

/** @file tr1/riemann_zeta.tcc
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{tr1/cmath}
 */

//
// ISO C++ 14882 TR1: 5.2  Special functions
//

// Written by Edward Smith-Rowland based on:
//   (1) Handbook of Mathematical Functions,
//       Ed. by Milton Abramowitz and Irene A. Stegun,
//       Dover Publications, New-York, Section 5, pp. 807-808.
//   (2) The Gnu Scientific Library, http://www.gnu.org/software/gsl
//   (3) Gamma, Exploring Euler's Constant, Julian Havil,
//       Princeton, 2003.

#ifndef _GLIBCXX_TR1_RIEMANN_ZETA_TCC
#define _GLIBCXX_TR1_RIEMANN_ZETA_TCC 1

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
  // [5.2] Special functions

  // Implementation-space details.
  namespace __detail
  {
    /**
     *   @brief  Compute the Riemann zeta function @f$ \zeta(s) @f$
     *           by summation for s > 1.
     * 
     *   The Riemann zeta function is defined by:
     *    \f[
     *      \zeta(s) = \sum_{k=1}^{\infty} \frac{1}{k^{s}} for s > 1
     *    \f]
     *   For s < 1 use the reflection formula:
     *    \f[
     *      \zeta(s) = 2^s \pi^{s-1} \Gamma(1-s) \zeta(1-s)
     *    \f]
     */
    template<typename _Tp>
    _Tp
    __riemann_zeta_sum(_Tp __s)
    {
      //  A user shouldn't get to this.
      if (__s < _Tp(1))
        std::__throw_domain_error(__N("Bad argument in zeta sum."));

      const unsigned int max_iter = 10000;
      _Tp __zeta = _Tp(0);
      for (unsigned int __k = 1; __k < max_iter; ++__k)
        {
          _Tp __term = std::pow(static_cast<_Tp>(__k), -__s);
          if (__term < std::numeric_limits<_Tp>::epsilon())
            {
              break;
            }
          __zeta += __term;
        }

      return __zeta;
    }


    /**
     *   @brief  Evaluate the Riemann zeta function @f$ \zeta(s) @f$
     *           by an alternate series for s > 0.
     * 
     *   The Riemann zeta function is defined by:
     *    \f[
     *      \zeta(s) = \sum_{k=1}^{\infty} \frac{1}{k^{s}} for s > 1
     *    \f]
     *   For s < 1 use the reflection formula:
     *    \f[
     *      \zeta(s) = 2^s \pi^{s-1} \Gamma(1-s) \zeta(1-s)
     *    \f]
     */
    template<typename _Tp>
    _Tp
    __riemann_zeta_alt(_Tp __s)
    {
      _Tp __sgn = _Tp(1);
      _Tp __zeta = _Tp(0);
      for (unsigned int __i = 1; __i < 10000000; ++__i)
        {
          _Tp __term = __sgn / std::pow(__i, __s);
          if (std::abs(__term) < std::numeric_limits<_Tp>::epsilon())
            break;
          __zeta += __term;
          __sgn *= _Tp(-1);
        }
      __zeta /= _Tp(1) - std::pow(_Tp(2), _Tp(1) - __s);

      return __zeta;
    }


  