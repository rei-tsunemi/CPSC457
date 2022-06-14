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

/** @file tr1/bessel_function.tcc
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{tr1/cmath}
 */

/* __cyl_bessel_jn_asymp adapted from GNU GSL version 2.4 specfunc/bessel_j.c
 * Copyright (C) 1996-2003 Gerard Jungman
 */

//
// ISO C++ 14882 TR1: 5.2  Special functions
//

// Written by Edward Smith-Rowland.
//
// References:
//   (1) Handbook of Mathematical Functions,
//       ed. Milton Abramowitz and Irene A. Stegun,
//       Dover Publications,
//       Section 9, pp. 355-434, Section 10 pp. 435-478
//   (2) The Gnu Scientific Library, http://www.gnu.org/software/gsl
//   (3) Numerical Recipes in C, by W. H. Press, S. A. Teukolsky,
//       W. T. Vetterling, B. P. Flannery, Cambridge University Press (1992),
//       2nd ed, pp. 240-245

#ifndef _GLIBCXX_TR1_BESSEL_FUNCTION_TCC
#define _GLIBCXX_TR1_BESSEL_FUNCTION_TCC 1

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
     *   @brief Compute the gamma functions required by the Temme series
     *          expansions of @f$ N_\nu(x) @f$ and @f$ K_\nu(x) @f$.
     *   @f[
     *     \Gamma_1 = \frac{1}{2\mu}
     *                [\frac{1}{\Gamma(1 - \mu)} - \frac{1}{\Gamma(1 + \mu)}]
     *   @f]
     *   and
     *   @f[
     *     \Gamma_2 = \frac{1}{2}
     *                [\frac{1}{\Gamma(1 - \mu)} + \frac{1}{\Gamma(1 + \mu)}]
     *   @f]
     *   where @f$ -1/2 <= \mu <= 1/2 @f$ is @f$ \mu = \nu - N @f$ and @f$ N @f$.
     *   is the nearest integer to @f$ \nu @f$.
     *   The values of \f$ \Gamma(1 + \mu) \f$ and \f$ \Gamma(1 - \mu) \f$
     *   are returned as well.
     * 
     *   The accuracy requirements on this are exquisite.
     *
     *   @param __mu     The input parameter of the gamma functions.
     *   @param __gam1   The output function \f$ \Gamma_1(\mu) \f$
     *   @param __gam2   The output function \f$ \Gamma_2(\mu) \f$
     *   @param __gampl  The output function \f$ \Gamma(1 + \mu) \f$
     *   @param __gammi  The output function \f$ \Gamma(1 - \mu) \f$
     */
    template <typename _Tp>
    void
    __gamma_temme(_Tp __mu,
                  _Tp & __gam1, _Tp & __gam2, _Tp & __gampl, _Tp & __gammi)
    {
#if _GLIBCXX_USE_C99_MATH_TR1
      __gampl = _Tp(1) / _GLIBCXX_MATH_NS::tgamma(_Tp(1) + __mu);
      __gammi = _Tp(1) / _GLIBCXX_MATH_NS::tgamma(_Tp(1) - __mu);
#else
      __gampl = _Tp(1) / __gamma(_Tp(1) + __mu);
      __gammi = _Tp(1) / __gamma(_Tp(1) - __mu);
#endif

      if (std::abs(__mu) < std::numeric_limits<_Tp>::epsilon())
        __gam1 = -_Tp(__numeric_constants<_Tp>::__gamma_e());
      else
 