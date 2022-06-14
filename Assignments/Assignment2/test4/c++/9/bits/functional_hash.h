// functional_hash.h header -*- C++ -*-

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

/** @file bits/functional_hash.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{functional}
 */

#ifndef _FUNCTIONAL_HASH_H
#define _FUNCTIONAL_HASH_H 1

#pragma GCC system_header

#include <bits/hash_bytes.h>

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  /** @defgroup hashes Hashes
   *  @ingroup functors
   *
   *   Hashing functors taking a variable type and returning a @c std::size_t.
   *
   *  @{
   */

  template<typename _Result, typename _Arg>
    struct __hash_base
    {
      typedef _Result     result_type _GLIBCXX17_DEPRECATED;
      typedef _Arg      argument_type _GLIBCXX17_DEPRECATED;
    };

  /// Primary class template hash.
  template<typename _Tp>
    struct hash;

  template<typename _Tp, typename = void>
    struct __poison_hash
    {
      static constexpr bool __enable_hash_call = false;
    private:
      // Private rather than deleted to be non-trivially-copyable.
      __poison_hash(__poison_hash&&);
      ~__poison_hash();
    };

  template<typename _Tp>
    struct __poison_hash<_Tp, __void_t<decltype(hash<_Tp>()(declval<_Tp>()))>>
    {
      static constexpr bool __enable_hash_call = true;
    };

  // Helper struct for SFINAE-poisoning non-enum types.
  template<typename _Tp, bool = is_enum<_Tp>::value>
    struct __hash_enum
    {
    private:
      // Private rather than deleted to be non-trivially-copyable.
      __hash_enum(__hash_enum&&);
      ~__hash_enum();
    };

  // Helper struct for hash with enum types.
  template<typename _Tp>
    struct __hash_enum<_Tp, true> : public __hash_base<size_t, _Tp>
    {
      size_t
      operator()(_Tp __val) const noexcept
      {
       using __type = typename underlying_type<_Tp>::type;
       return hash<__type>{}(static_cast<__type>(__val));
      }
    };

  /// Primary class template hash, usable for enum types only.
  // Use with non-enum types still SFINAES.
  template<typename _Tp>
    struct hash : __hash_enum<_Tp>
    { };

  /// Partial specializations for pointer types.
  template<typename _Tp>
    struct hash<_Tp*> : public __hash_base<size_t, _Tp*>
    {
      size_t
      operator()(_Tp* __p) const noexcept
      { return reinterpret_cast<size_t>(__p); }
    };

  // Explicit specializations for integer types.
#define _Cxx_hashtable_define_trivial_hash(_Tp) 	\
  template<>						\
    struct hash<_Tp> : public __hash_base<size_t, _Tp>  \
    {                                                   \
      size_t                                            \
      operator()(_Tp __val) const noexcept              \
      { return static_cast<size_t>(__val); }            \
    };

  /// Explicit specialization for bool.
  _Cxx_hashtable_define_trivial_hash(bool)

  /// Explicit specialization for char.
  _Cxx_hashtable_define_trivial_hash(char)

  /// Explicit specialization for signed char.
  _Cxx_hashtable_define_trivial_hash(signed char)

  /// Explicit specializa