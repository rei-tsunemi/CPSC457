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

/** @file tr1/poly_laguerre.tcc
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{tr1/cmath}
 */

//
// ISO C++ 14882 TR1: 5.2  Special functions
//

// Written by Edward Smith-Rowland based on:
//   (1) Handbook of Mathematical Functions,
//       Ed. Milton Abramowitz and Irene A. Stegun,
//       Dover Publications,
//       Section 13, pp. 509-510, Section 22 pp. 773-802
//   (2) The Gnu Scientific Library, http://www.gnu.org/software/gsl

#ifndef _GLIBCXX_TR1_POLY_LAGUERRE_TCC
#define _GLIBCXX_TR1_POLY_LAGUERRE_TCC 1

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
     *   @brief This routine returns the associated Laguerre polynomial 
     *          of order @f$ n @f$, degree @f$ \alpha @f$ for large n.
     *   Abramowitz & Stegun, 13.5.21
     *
     *   @param __n The order of the Laguerre function.
     *   @param __alpha The degree of the Laguerre function.
     *   @param __x The argument of the Laguerre function.
     *   @return The value of the Laguerre function of order n,
     *           degree @f$ \alpha @f$, and argument x.
     *
     *  This is from the GNU Scientific Library.
     */
    template<typename _Tpa, typename _Tp>
    _Tp
    __poly_laguerre_large_n(unsigned __n, _Tpa __alpha1, _Tp __x)
    {
      const _Tp __a = -_Tp(__n);
      const _Tp __b = _Tp(__alpha1) + _Tp(1);
      const _Tp __eta = _Tp(2) * __b - _Tp(4) * __a;
      const _Tp __cos2th = __x / __eta;
      const _Tp __sin2th = _Tp(1) - __cos2th;
      const _Tp __th = std::acos(std::sqrt(__cos2th));
      const _Tp __pre_h = __numeric_constants<_Tp>::__pi_2()
                        * __numeric_constants<_Tp>::__pi_2()
                        * __eta * __eta * __cos2th * __sin2th;

#if _GLIBCXX_USE_C99_MATH_TR1
      const _Tp __lg_b = _GLIBCXX_MATH_NS::lgamma(_Tp(__n) + __b);
      const _Tp __lnfact = _GLIBCXX_MATH_NS::lgamma(_Tp(__n + 1));
#else
      const _Tp __lg_b = __log_gamma(_Tp(__n) + __b);
      const _Tp __lnfact = __log_gamma(_Tp(__n + 1));
#endif

      _Tp __pre_term1 = _Tp(0.5L) * (_Tp(1) - __b)
                      * std::log(_Tp(0.25L) * __x * __eta);
      _Tp __pre_term2 = _Tp(0.25L) * std::log(__pre_h);
      _Tp __lnpre = __lg_b - __lnfact + _Tp(0.5L) * __x
                      + __pre_term1 - __pre_term2;
      _Tp __ser_term1 = std::sin(__a * __numeric_constants<_Tp>::__pi());
      _Tp __ser_term2 = std::sin(_Tp(0.25L) * __eta
                              * (_Tp(2) * __th
                               - std::sin(_Tp(2) * __th))
                               + __numeric_constants<_Tp>::__pi_4());
      _Tp __