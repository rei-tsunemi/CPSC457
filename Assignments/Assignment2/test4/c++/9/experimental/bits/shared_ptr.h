// Experimental shared_ptr with array support -*- C++ -*-

// Copyright (C) 2015-2019 Free Software Foundation, Inc.
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

/** @file experimental/bits/shared_ptr.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{experimental/memory}
 */

#ifndef _GLIBCXX_EXPERIMENTAL_SHARED_PTR_H
#define _GLIBCXX_EXPERIMENTAL_SHARED_PTR_H 1

#pragma GCC system_header

#if __cplusplus >= 201402L

#include <memory>
#include <experimental/type_traits>

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

namespace experimental
{
inline namespace fundamentals_v2
{
  // 8.2.1

  template<typename _Tp> class shared_ptr;
  template<typename _Tp> class weak_ptr;
  template<typename _Tp> class enable_shared_from_this;

  template<typename _Yp, typename _Tp>
    constexpr bool __sp_compatible_v
      = std::__sp_compatible_with<_Yp*, _Tp*>::value;

  template<typename _Tp, typename _Yp>
    constexpr bool __sp_is_constructible_v
      = std::__sp_is_constructible<_Tp, _Yp>::value;

  template<typename _Tp>
    class shared_ptr : public __shared_ptr<_Tp>
    {
      using _Base_type = __shared_ptr<_Tp>;

    public:
      using element_type = typename _Base_type::element_type;

    private:
      // Constraint for construction from a pointer of type _Yp*:
      template<typename _Yp>
	using _SafeConv = enable_if_t<__sp_is_constructible_v<_Tp, _Yp>>;

      template<typename _Tp1, typename _Res = void>
	using _Compatible
	  = enable_if_t<__sp_compatible_v<_Tp1, _Tp>, _Res>;

      template<typename _Tp1, typename _Del,
	       typename _Ptr = typename unique_ptr<_Tp1, _Del>::pointer,
	       typename _Res = void>
	using _UniqCompatible = enable_if_t<
	  __sp_compatible_v<_Tp1, _Tp>
	  && experimental::is_convertible_v<_Ptr, element_type*>,
	  _Res>;

    public:

      // 8.2.1.1, shared_ptr constructors
      constexpr shared_ptr() noexcept = default;

      template<typename _Tp1, typename = _SafeConv<_Tp1>>
	explicit
	shared_ptr(_Tp1* __p) : _Base_type(__p)
	{ _M_enable_shared_from_this_with(__p); }

      template<typename _Tp1, typename _Deleter, typename = _SafeConv<_Tp1>>
	shared_ptr(_Tp1* __p, _Deleter __d)
	: _Base_type(__p, __d)
	{ _M_enable_shared_from_this_with(__p); }

      template<typename _Tp1, typename _Deleter, typename _Alloc,
	       typename = _SafeConv<_Tp1>>
	shared_ptr(_Tp1* __p, _Deleter __d, _Alloc __a)
	: _Base_type(__p, __d, __a)
	{ _M_enable_shared_from_this_with(__p); }

      template<typename _Deleter>
	shared_ptr(nullptr_t __p, _Deleter __d)
	: _Base_type(__p, __d) { }

      template<typename _Deleter, typename _Alloc>
	shared_ptr(nullptr_t __p, _Deleter __d, _Alloc __a)
	: _Base_type(__p, __d, __a) { }

      template<typename _Tp1>
	shared_ptr(const shared_ptr<_Tp1>& __r, element_type* __p) noexcept
	: _Base_type(__r, __p) { }

      shared_ptr(const shared_ptr& __r) noexcept
	: _Base_type(__r) { }

      template<typename _Tp1, typename = _Compatible<_Tp1>>
	shared_ptr(const shared_ptr<_Tp1>& __r) noexcept
	: _Base_type(__r) { }

      shared_ptr(shared_pt