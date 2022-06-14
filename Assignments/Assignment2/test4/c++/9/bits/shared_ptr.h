// shared_ptr and weak_ptr implementation -*- C++ -*-

// Copyright (C) 2007-2019 Free Software Foundation, Inc.
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

// GCC Note: Based on files from version 1.32.0 of the Boost library.

//  shared_count.hpp
//  Copyright (c) 2001, 2002, 2003 Peter Dimov and Multi Media Ltd.

//  shared_ptr.hpp
//  Copyright (C) 1998, 1999 Greg Colvin and Beman Dawes.
//  Copyright (C) 2001, 2002, 2003 Peter Dimov

//  weak_ptr.hpp
//  Copyright (C) 2001, 2002, 2003 Peter Dimov

//  enable_shared_from_this.hpp
//  Copyright (C) 2002 Peter Dimov

// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/** @file
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{memory}
 */

#ifndef _SHARED_PTR_H
#define _SHARED_PTR_H 1

#include <bits/shared_ptr_base.h>

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  /**
   * @addtogroup pointer_abstractions
   * @{
   */

  /// 20.7.2.2.11 shared_ptr I/O
  template<typename _Ch, typename _Tr, typename _Tp, _Lock_policy _Lp>
    inline std::basic_ostream<_Ch, _Tr>&
    operator<<(std::basic_ostream<_Ch, _Tr>& __os,
	       const __shared_ptr<_Tp, _Lp>& __p)
    {
      __os << __p.get();
      return __os;
    }

  template<typename _Del, typename _Tp, _Lock_policy _Lp>
    inline _Del*
    get_deleter(const __shared_ptr<_Tp, _Lp>& __p) noexcept
    {
#if __cpp_rtti
      return static_cast<_Del*>(__p._M_get_deleter(typeid(_Del)));
#else
      return 0;
#endif
    }

  /// 20.7.2.2.10 shared_ptr get_deleter
  template<typename _Del, typename _Tp>
    inline _Del*
    get_deleter(const shared_ptr<_Tp>& __p) noexcept
    {
#if __cpp_rtti
      return static_cast<_Del*>(__p._M_get_deleter(typeid(_Del)));
#else
      return 0;
#endif
    }

  /**
   *  @brief  A smart pointer with reference-counted copy semantics.
   *
   *  The object pointed to is deleted when the last shared_ptr pointing to
   *  it is destroyed or reset.
  */
  template<typename _Tp>
    class shared_ptr : public __shared_ptr<_Tp>
    {
      template<typename... _Args>
	using _Constructible = typename enable_if<
	  is_constructible<__shared_ptr<_Tp>, _Args...>::value
	>::type;

      template<typename _Arg>
	using _Assignable = typename enable_if<
	  is_assignable<__shared_ptr<_Tp>&, _Arg>::value, shared_ptr&
	>::type;

    public:

      using element_type = typename __shared_ptr<_Tp>::element_type;

#if __cplusplus > 201402L
# define __cpp_lib_shared_ptr_weak_type 201606
      using weak_type = weak_ptr<_Tp>;
#endif
      /**
       *  @brief  Construct an empty %shared_ptr.
       *  @post   use_count()==0 && get()==0
       */
      constexpr shared_ptr() noexcept : __shared_ptr<_Tp>() { }

      shared_ptr(const shared_ptr&) noexcept = default;

      /**
       *  @brief  Construct a %shared_ptr that owns the pointer @a __p.
       *  @param  __p  A pointer that is convertible to element_type*.
       *  @post   use_count() == 1 && get() == __p
       *  @throw  std::b