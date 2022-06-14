// -*- C++ -*-
//===-- algorithm_impl.h --------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef __PSTL_algorithm_impl_H
#define __PSTL_algorithm_impl_H

#include <iterator>
#include <type_traits>
#include <utility>
#include <functional>
#include <algorithm>

#include "execution_impl.h"
#include "memory_impl.h"
#include "parallel_backend_utils.h"
#include "unseq_backend_simd.h"

#if __PSTL_USE_PAR_POLICIES
#include "parallel_backend.h"
#include "parallel_impl.h"
#endif

namespace __pstl
{
namespace __internal
{

//------------------------------------------------------------------------
// any_of
//------------------------------------------------------------------------

template <class _ForwardIterator, class _Pred>
bool
__brick_any_of(const _ForwardIterator __first, const _ForwardIterator __last, _Pred __pred,
               /*__is_vector=*/std::false_type) noexcept
{
    return std::any_of(__first, __last, __pred);
};

template <class _ForwardIterator, class _Pred>
bool
__brick_any_of(const _ForwardIterator __first, const _ForwardIterator __last, _Pred __pred,
               /*__is_vector=*/std::true_type) noexcept
{
    return __unseq_backend::__simd_or(__first, __last - __first, __pred);
};

template <class _ExecutionPolicy, class _ForwardIterator, class _Pred, class _IsVector>
bool
__pattern_any_of(_ExecutionPolicy&&, _ForwardIterator __first, _ForwardIterator __last, _Pred __pred,
                 _IsVector __is_vector, /*parallel=*/std::false_type) noexcept
{
    return __internal::__brick_any_of(__first, __last, __pred, __is_vector);
}

#if __PSTL_USE_PAR_POLICIES
template <class _ExecutionPolicy, class _ForwardIterator, class _Pred, class _IsVector>
bool
__pattern_any_of(_ExecutionPolicy&& __exec, _ForwardIterator __first, _ForwardIterator __last, _Pred __pred,
                 _IsVector __is_vector, /*parallel=*/std::true_type)
{
    return __internal::__except_handler([&]() {
       return __internal::__parallel_or(std::forward<_ExecutionPolicy>(__exec), __first, __last,
                             [__pred, __is_vector](_ForwardIterator __i, _ForwardIterator __j) {
                                 return __internal::__brick_any_of(__i, __j, __pred, __is_vector);
                             });
    });
}
#endif

// [alg.foreach]
// for_each_n with no policy

template <class _ForwardIterator, class _Size, class _Function>
_ForwardIterator
__for_each_n_it_serial(_ForwardIterator __first, _Size __n, _Function __f)
{
    for (; __n > 0; ++__first, --__n)
        __f(__first);
    return __first;
}

//------------------------------------------------------------------------
// walk1 (pseudo)
//
// walk1 evaluates f(x) for each dereferenced value x drawn from [first,last)
//------------------------------------------------------------------------
template <class _ForwardIterator, class _Function>
void
__brick_walk1(_ForwardIterator __first, _ForwardIterator __last, _Function __f, /*vector=*/std::false_type) noexcept
{
    std::for_each(__first, __last, __f);
}

template <class _RandomAccessIterator, class _Function>
void
__brick_walk1(_RandomAccessIterator __first, _RandomAccessIterator __last, _Function __f,
              /*vector=*/std::true_type) noexcept
{
    __unseq_backend::__simd_walk_1(__first, __last - __first, __f);
}

template <class _ExecutionPolicy, class _ForwardIterator, class _Function, class _IsVector>
void
__pattern_walk1(_ExecutionPolicy&&, _ForwardIterator __first, _ForwardIterator __last, _Function __f,
                _IsVector __is_vector,
                /*parallel=*/std::false_type) noexcept
{
    __internal::__brick_walk1(__first, __last, __f, __is_vector);
}

#if __PSTL_USE_PAR_POLICIES
template <class _ExecutionPolicy, class _ForwardIterator, class _Function, class _IsVector>
void
__pattern_walk1(_