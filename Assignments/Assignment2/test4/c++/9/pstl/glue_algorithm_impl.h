// -*- C++ -*-
//===-- glue_algorithm_impl.h ---------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef __PSTL_glue_algorithm_impl_H
#define __PSTL_glue_algorithm_impl_H

#include <functional>

#include "execution_defs.h"
#include "utils.h"
#include "algorithm_fwd.h"
#include "numeric_fwd.h" /* count and count_if use __pattern_transform_reduce */

namespace std
{

// [alg.any_of]

template <class _ExecutionPolicy, class _ForwardIterator, class _Predicate>
__pstl::__internal::__enable_if_execution_policy<_ExecutionPolicy, bool>
any_of(_ExecutionPolicy&& __exec, _ForwardIterator __first, _ForwardIterator __last, _Predicate __pred)
{
    using namespace __pstl;
    return __internal::__pattern_any_of(
        std::forward<_ExecutionPolicy>(__exec), __first, __last, __pred,
        __internal::__is_vectorization_preferred<_ExecutionPolicy, _ForwardIterator>(__exec),
        __internal::__is_parallelization_preferred<_ExecutionPolicy, _ForwardIterator>(__exec));
}

// [alg.all_of]

template <class _ExecutionPolicy, class _ForwardIterator, class _Pred>
__pstl::__internal::__enable_if_execution_policy<_ExecutionPolicy, bool>
all_of(_ExecutionPolicy&& __exec, _ForwardIterator __first, _ForwardIterator __last, _Pred __pred)
{
    return !std::any_of(std::forward<_ExecutionPolicy>(__exec), __first, __last,
                        __pstl::__internal::__not_pred<_Pred>(__pred));
}

// [alg.none_of]

template <class _ExecutionPolicy, class _ForwardIterator, class _Predicate>
__pstl::__internal::__enable_if_execution_policy<_ExecutionPolicy, bool>
none_of(_ExecutionPolicy&& __exec, _ForwardIterator __first, _ForwardIterator __last, _Predicate __pred)
{
    return !std::any_of(std::forward<_ExecutionPolicy>(__exec), __first, __last, __pred);
}

// [alg.foreach]

template <class _ExecutionPolicy, class _ForwardIterator, class _Function>
__pstl::__internal::__enable_if_execution_policy<_ExecutionPolicy, void>
for_each(_ExecutionPolicy&& __exec, _ForwardIterator __first, _ForwardIterator __last, _Function __f)
{
    using namespace __pstl;
    __internal::__pattern_walk1(std::forward<_ExecutionPolicy>(__exec), __first, __last, __f,
                                __internal::__is_vectorization_preferred<_ExecutionPolicy, _ForwardIterator>(__exec),
                                __internal::__is_parallelization_preferred<_ExecutionPolicy, _ForwardIterator>(__exec));
}

template <class _ExecutionPolicy, class _ForwardIterator, class _Size, class _Function>
__pstl::__internal::__enable_if_execution_policy<_ExecutionPolicy, _ForwardIterator>
for_each_n(_ExecutionPolicy&& __exec, _ForwardIterator __first, _Size __n, _Function __f)
{
    using namespace __pstl;
    return __internal::__pattern_walk1_n(
        std::forward<_ExecutionPolicy>(__exec), __first, __n, __f,
        __internal::__is_vectorization_preferred<_ExecutionPolicy, _ForwardIterator>(__exec),
        __internal::__is_parallelization_preferred<_ExecutionPolicy, _ForwardIterator>(__exec));
}

// [alg.find]

template <class _ExecutionPolicy, class _ForwardIterator, class _Predicate>
__pstl::__internal::__enable_if_execution_policy<_ExecutionPolicy, _ForwardIterator>
find_if(_ExecutionPolicy&& __exec, _ForwardIterator __first, _ForwardIterator __last, _Predicate __pred)
{
    using namespace __pstl;
    return __internal::__pattern_find_if(
        std::forward<_ExecutionPolicy>(__exec), __first, __last, __pred,
        __internal::__is_vectorization_preferred<_ExecutionPolicy, _ForwardIterator>(__exec),
        __internal::__is_parallelization_preferred<_ExecutionPolicy, _ForwardIterator>(__exec));
}

template <class _ExecutionPolicy, class _ForwardIterator, class _Predicate>
__pstl::__internal::__enable_if_execution_policy<_ExecutionPolicy, _ForwardIterator>
find_if_not(_ExecutionPolicy&& __exe