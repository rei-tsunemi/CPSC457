// -*- C++ -*-
//===-- numeric_impl.h ----------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef __PSTL_numeric_impl_H
#define __PSTL_numeric_impl_H

#include <iterator>
#include <type_traits>
#include <numeric>

#include "execution_impl.h"
#include "unseq_backend_simd.h"
#include "algorithm_fwd.h"

#if __PSTL_USE_PAR_POLICIES
#include "parallel_backend.h"
#endif

namespace __pstl
{
namespace __internal
{

//------------------------------------------------------------------------
// transform_reduce (version with two binary functions, according to draft N4659)
//------------------------------------------------------------------------

template <class _ForwardIterator1, class _ForwardIterator2, class _Tp, class _BinaryOperation1, class _BinaryOperation2>
_Tp
__brick_transform_reduce(_ForwardIterator1 __first1, _ForwardIterator1 __last1, _ForwardIterator2 __first2, _Tp __init,
                         _BinaryOperation1 __binary_op1, _BinaryOperation2 __binary_op2,
                         /*is_vector=*/std::false_type) noexcept
{
    return std::inner_product(__first1, __last1, __first2, __init, __binary_op1, __binary_op2);
}

template <class _ForwardIterator1, class _ForwardIterator2, class _Tp, class _BinaryOperation1, class _BinaryOperation2>
_Tp
__brick_transform_reduce(_ForwardIterator1 __first1, _ForwardIterator1 __last1, _ForwardIterator2 __first2, _Tp __init,
                         _BinaryOperation1 __binary_op1, _BinaryOperation2 __binary_op2,
                         /*is_vector=*/std::true_type) noexcept
{
    typedef typename std::iterator_traits<_ForwardIterator1>::difference_type _DifferenceType;
    return __unseq_backend::__simd_transform_reduce(
        __last1 - __first1, __init, __binary_op1,
        [=, &__binary_op2](_DifferenceType __i) { return __binary_op2(__first1[__i], __first2[__i]); });
}

template <class _ExecutionPolicy, class _ForwardIterator1, class _ForwardIterator2, class _Tp, class _BinaryOperation1,
          class _BinaryOperation2, class _IsVector>
_Tp
__pattern_transform_reduce(_ExecutionPolicy&&, _ForwardIterator1 __first1, _ForwardIterator1 __last1,
                           _ForwardIterator2 __first2, _Tp __init, _BinaryOperation1 __binary_op1,
                           _BinaryOperation2 __binary_op2, _IsVector __is_vector,
                           /*is_parallel=*/std::false_type) noexcept
{
    return __brick_transform_reduce(__first1, __last1, __first2, __init, __binary_op1, __binary_op2, __is_vector);
}

#if __PSTL_USE_PAR_POLICIES
template <class _ExecutionPolicy, class _RandomAccessIterator1, class _RandomAccessIterator2, class _Tp,
          class _BinaryOperation1, class _BinaryOperation2, class _IsVector>
_Tp
__pattern_transform_reduce(_ExecutionPolicy&& __exec, _RandomAccessIterator1 __first1, _RandomAccessIterator1 __last1,
                           _RandomAccessIterator2 __first2, _Tp __init, _BinaryOperation1 __binary_op1,
                           _BinaryOperation2 __binary_op2, _IsVector __is_vector, /*is_parallel=*/std::true_type)
{
    return __internal::__except_handler([&]() {
        return __par_backend::__parallel_transform_reduce(
            std::forward<_ExecutionPolicy>(__exec), __first1, __last1,
            [__first1, __first2, __binary_op2](_RandomAccessIterator1 __i) mutable {
                return __binary_op2(*__i, *(__first2 + (__i - __first1)));
            },
            __init,
            __binary_op1, // Combine
            [__first1, __first2, __binary_op1, __binary_op2,
             __is_vector](_RandomAccessIterator1 __i, _RandomAccessIterator1 __j, _Tp __init) -> _Tp {
                return __internal::__brick_transform_reduce(__i, __j, __first2 + (__i - __first1), __init, __binary_op1,
                                                __binar