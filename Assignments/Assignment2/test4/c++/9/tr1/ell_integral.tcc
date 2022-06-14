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

/** @file tr1/ell_integral.tcc
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{tr1/cmath}
 */

//
// ISO C++ 14882 TR1: 5.2  Special functions
//

// Written by Edward Smith-Rowland based on:
//   (1)  B. C. Carlson Numer. Math. 33, 1 (1979)
//   (2)  B. C. Carlson, Special Functions of Applied Mathematics (1977)
//   (3)  The Gnu Scientific Library, http://www.gnu.org/software/gsl
//   (4)  Numerical Recipes in C, 2nd ed, by W. H. Press, S. A. Teukolsky,
//        W. T. Vetterling, B. P. Flannery, Cambridge University Press
//        (1992), pp. 261-269

#ifndef _GLIBCXX_TR1_ELL_INTEGRAL_TCC
#define _GLIBCXX_TR1_ELL_INTEGRAL_TCC 1

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

#if _GLIBCXX_USE_STD_SPEC_FUNCS
#elif defined(_GLIBCXX_TR1_CMATH)
namespace tr1
{
#else
# error do not include this header directly, use <cmath> or <tr1/cmath>
#endif
  // [5.2] Special functions

  // Implementation-space details.
  namespace __detail
  {
    /**
     *   @brief Return the Carlson elliptic function @f$ R_F(x,y,z) @f$
     *          of the first kind.
     * 
     *   The Carlson elliptic function of the first kind is defined by:
     *   @f[
     *       R_F(x,y,z) = \frac{1}{2} \int_0^\infty
     *                 \frac{dt}{(t + x)^{1/2}(t + y)^{1/2}(t + z)^{1/2}}
     *   @f]
     *
     *   @param  __x  The first of three symmetric arguments.
     *   @param  __y  The second of three symmetric arguments.
     *   @param  __z  The third of three symmetric arguments.
     *   @return  The Carlson elliptic function of the first kind.
     */
    template<typename _Tp>
    _Tp
    __ellint_rf(_Tp __x, _Tp __y, _Tp __z)
    {
      const _Tp __min = std::numeric_limits<_Tp>::min();
      const _Tp __max = std::numeric_limits<_Tp>::max();
      const _Tp __lolim = _Tp(5) * __min;
      const _Tp __uplim = __max / _Tp(5);

      if (__x < _Tp(0) || __y < _Tp(0) || __z < _Tp(0))
        std::__throw_domain_error(__N("Argument less than zero "
                                      "in __ellint_rf."));
      else if (__x + __y < __lolim || __x + __z < __lolim
            || __y + __z < __lolim)
        std::__throw_domain_error(__N("Argument too small in __ellint_rf"));
      else
        {
          const _Tp __c0 = _Tp(1) / _Tp(4);
          const _Tp __c1 = _Tp(1) / _Tp(24);
          const _Tp __c2 = _Tp(1) / _Tp(10);
          const _Tp __c3 = _Tp(3) / _Tp(44);
          const _Tp __c4 = _Tp(1) / _Tp(14);

          _Tp __xn = __x;
          _Tp __yn = __y;
          _Tp __zn = __z;

          const _Tp __eps = std::numeric_limits<_Tp>::epsilon();
          const _Tp __errtol = std::pow(__eps, _Tp(1) / _Tp(6));
          _Tp __mu;
          _Tp __xndev, __yndev, __zndev;

          const unsigned int __max_iter = 100;
          for (unsigned int __iter = 0; __iter < __max_iter; ++__iter)
            {
              __mu = (__xn + __yn + __zn) / _Tp(3);
              _