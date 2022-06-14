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

/** @file tr1/legendre_function.tcc
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
//       Section 8, pp. 331-341
//   (2) The Gnu Scientific Library, http://www.gnu.org/software/gsl
//   (3) Numerical Recipes in C, by W. H. Press, S. A. Teukolsky,
//       W. T. Vetterling, B. P. Flannery, Cambridge University Press (1992),
//       2nd ed, pp. 252-254

#ifndef _GLIBCXX_TR1_LEGENDRE_FUNCTION_TCC
#define _GLIBCXX_TR1_LEGENDRE_FUNCTION_TCC 1

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
     *   @brief  Return the Legendre polynomial by recursion on degree
     *           @f$ l @f$.
     * 
     *   The Legendre function of @f$ l @f$ and @f$ x @f$,
     *   @f$ P_l(x) @f$, is defined by:
     *   @f[
     *     P_l(x) = \frac{1}{2^l l!}\frac{d^l}{dx^l}(x^2 - 1)^{l}
     *   @f]
     * 
     *   @param  l  The degree of the Legendre polynomial.  @f$l >= 0@f$.
     *   @param  x  The argument of the Legendre polynomial.  @f$|x| <= 1@f$.
     */
    template<typename _Tp>
    _Tp
    __poly_legendre_p(unsigned int __l, _Tp __x)
    {

      if (__isnan(__x))
        return std::numeric_limits<_Tp>::quiet_NaN();
      else if (__x == +_Tp(1))
        return +_Tp(1);
      else if (__x == -_Tp(1))
        return (__l % 2 == 1 ? -_Tp(1) : +_Tp(1));
      else
        {
          _Tp __p_lm2 = _Tp(1);
          if (__l == 0)
            return __p_lm2;

          _Tp __p_lm1 = __x;
          if (__l == 1)
            return __p_lm1;

          _Tp __p_l = 0;
          for (unsigned int __ll = 2; __ll <= __l; ++__ll)
            {
              //  This arrangement is supposed to be better for roundoff
              //  protection, Arfken, 2nd Ed, Eq 12.17a.
              __p_l = _Tp(2) * __x * __p_lm1 - __p_lm2
                    - (__x * __p_lm1 - __p_lm2) / _Tp(__ll);
              __p_lm2 = __p_lm1;
              __p_lm1 = __p_l;
            }

          return __p_l;
        }
    }


    /**
     *   @brief  Return the associated Legendre function by recursion
     *           on @f$ l @f$.
     * 
     *   The associated Legendre function is derived from the Legendre function
     *   @f$ P_l(x) @f$ by the Rodrigues formula:
     *   @f[
     *     P_l^m(x) = (1 - x^2)^{m/2}\frac{d^m}{dx^m}P_l(x)
     *   @f]
     *   @note @f$ P_l^m(x) = 0 @f$ if @f$ m 