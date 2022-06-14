// Implementation of std::reference_wrapper -*- C++ -*-

// Copyright (C) 2004-2019 Free Software Foundation, Inc.
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

/** @file include/bits/refwrap.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{functional}
 */

#ifndef _GLIBCXX_REFWRAP_H
#define _GLIBCXX_REFWRAP_H 1

#pragma GCC system_header

#if __cplusplus < 201103L
# include <bits/c++0x_warning.h>
#else

#include <bits/move.h>
#include <bits/invoke.h>
#include <bits/stl_function.h> // for unary_function and binary_function

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  /**
   * Derives from @c unary_function or @c binary_function, or perhaps
   * nothing, depending on the number of arguments provided. The
   * primary template is the basis case, which derives nothing.
   */
  template<typename _Res, typename... _ArgTypes>
    struct _Maybe_unary_or_binary_function { };

  /// Derives from @c unary_function, as appropriate.
  template<typename _Res, typename _T1>
    struct _Maybe_unary_or_binary_function<_Res, _T1>
    : std::unary_function<_T1, _Res> { };

  /// Derives from @c binary_function, as appropriate.
  template<typename _Res, typename _T1, typename _T2>
    struct _Maybe_unary_or_binary_function<_Res, _T1, _T2>
    : std::binary_function<_T1, _T2, _Res> { };

  template<typename _Signature>
    struct _Mem_fn_traits;

  template<typename _Res, typename _Class, typename... _ArgTypes>
    struct _Mem_fn_traits_base
    {
      using __result_type = _Res;
      using __maybe_type
	= _Maybe_unary_or_binary_function<_Res, _Class*, _ArgTypes...>;
      using __arity = integral_constant<size_t, sizeof...(_ArgTypes)>;
    };

#define _GLIBCXX_MEM_FN_TRAITS2(_CV, _REF, _LVAL, _RVAL)		\
  template<typename _Res, typename _Class, typename... _ArgTypes>	\
    struct _Mem_fn_traits<_Res (_Class::*)(_ArgTypes...) _CV _REF>	\
    : _Mem_fn_traits_base<_Res, _CV _Class, _ArgTypes...>		\
    {									\
      using __vararg = false_type;					\
    };									\
  template<typename _Res, typename _Class, typename... _ArgTypes>	\
    struct _Mem_fn_traits<_Res (_Class::*)(_ArgTypes... ...) _CV _REF>	\
    : _Mem_fn_traits_base<_Res, _CV _Class, _ArgTypes...>		\
    {									\
      using __vararg = true_type;					\
    };

#define _GLIBCXX_MEM_FN_TRAITS(_REF, _LVAL, _RVAL)		\
  _GLIBCXX_MEM_FN_TRAITS2(		, _REF, _LVAL, _RVAL)	\
  _GLIBCXX_MEM_FN_TRAITS2(const		, _REF, _LVAL, _RVAL)	\
  _GLIBCXX_MEM_FN_TRAITS2(volatile	, _REF, _LVAL, _RVAL)	\
  _GLIBCXX_MEM_FN_TRAITS2(const volatile, _REF, _LVAL, _RVAL)

_GLIBCXX_MEM_FN_TRAITS( , true_type, true_type)
_GLIBCXX_MEM_FN_TRAITS(&, true_type, false_type)
_GLIBCXX_MEM_FN_TRAITS(&&, false_type, true_type)

#if __cplusplus > 201402L
_GLIBCXX_MEM_FN_TRAITS(noexcept, true_type, true_type)
_GLIBCXX_MEM_FN_TRAITS(& noexcept, true_type, false_type)
_GLIBCXX_MEM_FN_TRAITS(&& noexcept, false_type, true_type)
#endif

#undef _GLIBCXX_MEM_FN_TRAITS
#undef _GLIBCXX_MEM_FN_TRAITS2

  /// If we have found a result_type, extract it.
  template<typename _Functor, typename = __