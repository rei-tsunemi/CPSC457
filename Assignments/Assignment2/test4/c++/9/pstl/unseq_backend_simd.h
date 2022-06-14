// -*- C++ -*-
//===-- unseq_backend_simd.h ----------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef __PSTL_unseq_backend_simd_H
#define __PSTL_unseq_backend_simd_H

#include <type_traits>

#include "utils.h"

// This header defines the minimum set of vector routines required
// to support parallel STL.
namespace __pstl
{
namespace __unseq_backend
{

// Expect vector width up to 64 (or 512 bit)
const std::size_t __lane_size = 64;

template <class _Iterator, class _DifferenceType, class _Function>
_Iterator
__simd_walk_1(_Iterator __first, _DifferenceType __n, _Function __f) noexcept
{
    __PSTL_PRAGMA_SIMD
    for (_DifferenceType __i = 0; __i < __n; ++__i)
        __f(__first[__i]);

    return __first + __n;
}

template <class _Iterator1, class _DifferenceType, class _Iterator2, class _Function>
_Iterator2
__simd_walk_2(_Iterator1 __first1, _DifferenceType __n, _Iterator2 __first2, _Function __f) noexcept
{
    __PSTL_PRAGMA_SIMD
    for (_DifferenceType __i = 0; __i < __n; ++__i)
        __f(__first1[__i], __first2[__i]);
    return __first2 + __n;
}

template <class _Iterator1, class _DifferenceType, class _Iterator2, class _Iterator3, class _Function>
_Iterator3
__simd_walk_3(_Iterator1 __first1, _DifferenceType __n, _Iterator2 __first2, _Iterator3 __first3,
              _Function __f) noexcept
{
    __PSTL_PRAGMA_SIMD
    for (_DifferenceType __i = 0; __i < __n; ++__i)
        __f(__first1[__i], __first2[__i], __first3[__i]);
    return __first3 + __n;
}

// TODO: check whether __simd_first() can be used here
template <class _Index, class _DifferenceType, class _Pred>
bool
__simd_or(_Index __first, _DifferenceType __n, _Pred __pred) noexcept
{
#if __PSTL_EARLYEXIT_PRESENT
    _DifferenceType __i;
    __PSTL_PRAGMA_VECTOR_UNALIGNED
    __PSTL_PRAGMA_SIMD_EARLYEXIT
    for (__i = 0; __i < __n; ++__i)
        if (__pred(__first[__i]))
            break;
    return __i < __n;
#else
    _DifferenceType __block_size = 4 < __n ? 4 : __n;
    const _Index __last = __first + __n;
    while (__last != __first)
    {
        int32_t __flag = 1;
        __PSTL_PRAGMA_SIMD_REDUCTION(& : __flag)
        for (_DifferenceType __i = 0; __i < __block_size; ++__i)
            if (__pred(*(__first + __i)))
                __flag = 0;
        if (!__flag)
            return true;

        __first += __block_size;
        if (__last - __first >= __block_size << 1)
        {
            // Double the block _Size.  Any unnecessary iterations can be amortized against work done so far.
            __block_size <<= 1;
        }
        else
        {
            __block_size = __last - __first;
        }
    }
    return false;
#endif
}

template <class _Index, class _DifferenceType, class _Compare>
_Index
__simd_first(_Index __first, _DifferenceType __begin, _DifferenceType __end, _Compare __comp) noexcept
{
#if __PSTL_EARLYEXIT_PRESENT
    _DifferenceType __i = __begin;
    __PSTL_PRAGMA_VECTOR_UNALIGNED // Do not generate peel loop part
        __PSTL_PRAGMA_SIMD_EARLYEXIT for (; __i < __end; ++__i)
    {
        if (__comp(__first, __i))
        {
            break;
        }
    }
    return __first + __i;
#else
    // Experiments show good block sizes like this
    const _DifferenceType __block_size = 8;
    alignas(__lane_size) _DifferenceType __lane[__block_size] = {0};
    while (__end - __begin >= __block_size)
    {
        _DifferenceType __found = 0;
        __PSTL_PRAGMA_VECTOR_UNALIGNED // Do not generate peel loop part
            __PSTL_PRAGMA_SIMD_REDUCTION(|
                                         : __found) for (_DifferenceType __i = __begin; __i < __begin + __block_size;
                                                         ++__i)
        {
            const _DifferenceType __t = __comp(__first, __i);
            __lane[__i - __