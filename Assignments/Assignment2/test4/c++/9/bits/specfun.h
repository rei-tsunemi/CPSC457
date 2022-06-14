// Mathematical Special Functions for -*- C++ -*-

// Copyright (C) 2006-2019 Free Software Foundation, Inc.
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

/** @file bits/specfun.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{cmath}
 */

#ifndef _GLIBCXX_BITS_SPECFUN_H
#define _GLIBCXX_BITS_SPECFUN_H 1

#pragma GCC visibility push(default)

#include <bits/c++config.h>

#define __STDCPP_MATH_SPEC_FUNCS__ 201003L

#define __cpp_lib_math_special_functions 201603L

#if __cplusplus <= 201403L && __STDCPP_WANT_MATH_SPEC_FUNCS__ == 0
# error include <cmath> and define __STDCPP_WANT_MATH_SPEC_FUNCS__
#endif

#include <bits/stl_algobase.h>
#include <limits>
#include <type_traits>

#include <tr1/gamma.tcc>
#include <tr1/bessel_function.tcc>
#include <tr1/beta_function.tcc>
#include <tr1/ell_integral.tcc>
#include <tr1/exp_integral.tcc>
#include <tr1/hypergeometric.tcc>
#include <tr1/legendre_function.tcc>
#include <tr1/modified_bessel_func.tcc>
#include <tr1/poly_hermite.tcc>
#include <tr1/poly_laguerre.tcc>
#include <tr1/riemann_zeta.tcc>

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  /**
   * @defgroup mathsf Mathematical Special Functions
   * @ingroup numerics
   *
   * A collection of advanced mathematical special functions,
   * defined by ISO/IEC IS 29124.
   * @{
   */

  /**
   * @mainpage Mathematical Special Functions
   *
   * @section intro Introduction and History
   * The first significant library upgrade on the road to C++2011,
   * <a href="http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2005/n1836.pdf">
   * TR1</a>, included a set of 23 mathematical functions that significantly
   * extended the standard transcendental functions inherited from C and declared
   * in @<cmath@>.
   *
   * Although most components from TR1 were eventually adopted for C++11 these
   * math functions were left behind out of concern for implementability.
   * The math functions were published as a separate international standard
   * <a href="http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2010/n3060.pdf">
   * IS 29124 - Extensions to the C++ Library to Support Mathematical Special
   * Functions</a>.
   *
   * For C++17 these functions were incorporated into the main standard.
   *
   * @section contents Contents
   * The following functions are implemented in namespace @c std:
   * - @ref assoc_laguerre "assoc_laguerre - Associated Laguerre functions"
   * - @ref assoc_legendre "assoc_legendre - Associated Legendre functions"
   * - @ref beta "beta - Beta functions"
   * - @ref comp_ellint_1 "comp_ellint_1 - Complete elliptic functions of the first kind"
   * - @ref comp_ellint_2 "comp_ellint_2 - Complete elliptic functions of the second kind"
   * - @ref comp_ellint_3 "comp_ellint_3 - Complete elliptic functions of the third kind"
   * - @ref cyl_bessel_i "cyl_bessel_i - Regular modified cylindrical Bessel functions"
   * - @ref cyl_bessel_j "cyl_bessel_j - Cylindrical Bessel functions of the first kind"
   * - @ref cyl_bessel_k "cyl_bessel_k - Irregular modified cylindrical Bessel functions"
   *