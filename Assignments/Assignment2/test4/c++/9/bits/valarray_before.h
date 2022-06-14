// The template and inlines for the -*- C++ -*- internal _Meta class.

// Copyright (C) 1997-2019 Free Software Foundation, Inc.
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

/** @file bits/valarray_before.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{valarray}
 */

// Written by Gabriel Dos Reis <Gabriel.Dos-Reis@cmla.ens-cachan.fr>

#ifndef _VALARRAY_BEFORE_H
#define _VALARRAY_BEFORE_H 1

#pragma GCC system_header

#include <bits/slice_array.h>

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  //
  // Implementing a loosened valarray return value is tricky.
  // First we need to meet 26.3.1/3: we should not add more than
  // two levels of template nesting. Therefore we resort to template
  // template to "flatten" loosened return value types.
  // At some point we use partial specialization to remove one level
  // template nesting due to _Expr<>
  //

  // This class is NOT defined. It doesn't need to.
  template<typename _Tp1, typename _Tp2> class _Constant;

  // Implementations of unary functions applied to valarray<>s.
  // I use hard-coded object functions here instead of a generic
  // approach like pointers to function:
  //    1) correctness: some functions take references, others values.
  //       we can't deduce the correct type afterwards.
  //    2) efficiency -- object functions can be easily inlined
  //    3) be Koenig-lookup-friendly

  struct _Abs
  {
    template<typename _Tp>
      _Tp operator()(const _Tp& __t) const
      { return abs(__t); }
  };

  struct _Cos
  {
    template<typename _Tp>
      _Tp operator()(const _Tp& __t) const
      { return cos(__t); }
  };

  struct _Acos
  {
    template<typename _Tp>
      _Tp operator()(const _Tp& __t) const
      { return acos(__t); }
  };

  struct _Cosh
  {
    template<typename _Tp>
      _Tp operator()(const _Tp& __t) const
      { return cosh(__t); }
  };

  struct _Sin
  {
    template<typename _Tp>
      _Tp operator()(const _Tp& __t) const
      { return sin(__t); }
  };

  struct _Asin
  {
    template<typename _Tp>
      _Tp operator()(const _Tp& __t) const
      { return asin(__t); }
  };

  struct _Sinh
  {
    template<typename _Tp>
      _Tp operator()(const _Tp& __t) const
      { return sinh(__t); }
  };

  struct _Tan
  {
    template<typename _Tp>
      _Tp operator()(const _Tp& __t) const
      { return tan(__t); }
  };

  struct _Atan
  {
    template<typename _Tp>
      _Tp operator()(const _Tp& __t) const
      { return atan(__t); }
  };

  struct _Tanh
  {
    template<typename _Tp>
      _Tp operator()(const _Tp& __t) const
      { return tanh(__t); }
  };

  struct _Exp
  {
    template<typename _Tp>
      _Tp operator()(const _Tp& __t) const
      { return exp(__t); }
  };

  struct _Log
  {
    template<typename _Tp>
      _Tp operator()(const _Tp& __t) const
      { return log(__t); }
  };

  struct _Log10
  {
    template<typename _Tp>
      _Tp operator()(const _Tp& __t) const
      { return log10(__t); }
  };

  struct _Sqrt
  {
    template<typename _Tp>
      _Tp operator()(const _Tp& __t)