// Allocator traits -*- C++ -*-

// Copyright (C) 2011-2019 Free Software Foundation, Inc.
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

/** @file bits/alloc_traits.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{memory}
 */

#ifndef _ALLOC_TRAITS_H
#define _ALLOC_TRAITS_H 1

#if __cplusplus >= 201103L

#include <bits/memoryfwd.h>
#include <bits/ptr_traits.h>
#include <ext/numeric_traits.h>

#define __cpp_lib_allocator_traits_is_always_equal 201411

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  struct __allocator_traits_base
  {
    template<typename _Tp, typename _Up, typename = void>
      struct __rebind : __replace_first_arg<_Tp, _Up> { };

    template<typename _Tp, typename _Up>
      struct __rebind<_Tp, _Up,
		      __void_t<typename _Tp::template rebind<_Up>::other>>
      { using type = typename _Tp::template rebind<_Up>::other; };

  protected:
    template<typename _Tp>
      using __pointer = typename _Tp::pointer;
    template<typename _Tp>
      using __c_pointer = typename _Tp::const_pointer;
    template<typename _Tp>
      using __v_pointer = typename _Tp::void_pointer;
    template<typename _Tp>
      using __cv_pointer = typename _Tp::const_void_pointer;
    template<typename _Tp>
      using __pocca = typename _Tp::propagate_on_container_copy_assignment;
    template<typename _Tp>
      using __pocma = typename _Tp::propagate_on_container_move_assignment;
    template<typename _Tp>
      using __pocs = typename _Tp::propagate_on_container_swap;
    template<typename _Tp>
      using __equal = typename _Tp::is_always_equal;
  };

  template<typename _Alloc, typename _Up>
    using __alloc_rebind
      = typename __allocator_traits_base::template __rebind<_Alloc, _Up>::type;

  /**
   * @brief  Uniform interface to all allocator types.
   * @ingroup allocators
  */
  template<typename _Alloc>
    struct allocator_traits : __allocator_traits_base
    {
      /// The allocator type
      typedef _Alloc allocator_type;
      /// The allocated type
      typedef typename _Alloc::value_type value_type;

      /**
       * @brief   The allocator's pointer type.
       *
       * @c Alloc::pointer if that type exists, otherwise @c value_type*
      */
      using pointer = __detected_or_t<value_type*, __pointer, _Alloc>;

    private:
      // Select _Func<_Alloc> or pointer_traits<pointer>::rebind<_Tp>
      template<template<typename> class _Func, typename _Tp, typename = void>
	struct _Ptr
	{
	  using type = typename pointer_traits<pointer>::template rebind<_Tp>;
	};

      template<template<typename> class _Func, typename _Tp>
	struct _Ptr<_Func, _Tp, __void_t<_Func<_Alloc>>>
	{
	  using type = _Func<_Alloc>;
	};

      // Select _A2::difference_type or pointer_traits<_Ptr>::difference_type
      template<typename _A2, typename _PtrT, typename = void>
	struct _Diff
	{ using type = typename pointer_traits<_PtrT>::difference_type; };

      template<typename _A2, typename _PtrT>
	struct _Diff<_A2, _PtrT, __void_t<typename _A2::difference_type>>
	{ using type = typename _A2::diffe