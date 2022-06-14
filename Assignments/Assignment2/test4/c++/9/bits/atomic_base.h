// -*- C++ -*- header.

// Copyright (C) 2008-2019 Free Software Foundation, Inc.
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

/** @file bits/atomic_base.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{atomic}
 */

#ifndef _GLIBCXX_ATOMIC_BASE_H
#define _GLIBCXX_ATOMIC_BASE_H 1

#pragma GCC system_header

#include <bits/c++config.h>
#include <stdint.h>
#include <bits/atomic_lockfree_defines.h>

#ifndef _GLIBCXX_ALWAYS_INLINE
#define _GLIBCXX_ALWAYS_INLINE inline __attribute__((__always_inline__))
#endif

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  /**
   * @defgroup atomics Atomics
   *
   * Components for performing atomic operations.
   * @{
   */

  /// Enumeration for memory_order
#if __cplusplus > 201703L
  enum class memory_order : int
    {
      relaxed,
      consume,
      acquire,
      release,
      acq_rel,
      seq_cst
    };

  inline constexpr memory_order memory_order_relaxed = memory_order::relaxed;
  inline constexpr memory_order memory_order_consume = memory_order::consume;
  inline constexpr memory_order memory_order_acquire = memory_order::acquire;
  inline constexpr memory_order memory_order_release = memory_order::release;
  inline constexpr memory_order memory_order_acq_rel = memory_order::acq_rel;
  inline constexpr memory_order memory_order_seq_cst = memory_order::seq_cst;
#else
  typedef enum memory_order
    {
      memory_order_relaxed,
      memory_order_consume,
      memory_order_acquire,
      memory_order_release,
      memory_order_acq_rel,
      memory_order_seq_cst
    } memory_order;
#endif

  enum __memory_order_modifier
    {
      __memory_order_mask          = 0x0ffff,
      __memory_order_modifier_mask = 0xffff0000,
      __memory_order_hle_acquire   = 0x10000,
      __memory_order_hle_release   = 0x20000
    };

  constexpr memory_order
  operator|(memory_order __m, __memory_order_modifier __mod)
  {
    return memory_order(int(__m) | int(__mod));
  }

  constexpr memory_order
  operator&(memory_order __m, __memory_order_modifier __mod)
  {
    return memory_order(int(__m) & int(__mod));
  }

  // Drop release ordering as per [atomics.types.operations.req]/21
  constexpr memory_order
  __cmpexch_failure_order2(memory_order __m) noexcept
  {
    return __m == memory_order_acq_rel ? memory_order_acquire
      : __m == memory_order_release ? memory_order_relaxed : __m;
  }

  constexpr memory_order
  __cmpexch_failure_order(memory_order __m) noexcept
  {
    return memory_order(__cmpexch_failure_order2(__m & __memory_order_mask)
      | __memory_order_modifier(__m & __memory_order_modifier_mask));
  }

  _GLIBCXX_ALWAYS_INLINE void
  atomic_thread_fence(memory_order __m) noexcept
  { __atomic_thread_fence(int(__m)); }

  _GLIBCXX_ALWAYS_INLINE void
  atomic_signal_fence(memory_order __m) noexcept
  { __atomic_signal_fence(int(__m)); }

  /// kill_dependency
  template<typename _Tp>
    inline _Tp
    kill_dependency(_Tp __y) noexcept
    {
      _Tp __ret(__y);
      return __ret;
    }


  // Base types for atomics.
  template<typename _IntTp>
  