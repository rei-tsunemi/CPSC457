// -*- C++ -*-
//===-- algorithm_fwd.h --------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef __PSTL_algorithm_fwd_H
#define __PSTL_algorithm_fwd_H

#include <type_traits>
#include <utility>

namespace __pstl
{
namespace __internal
{

//------------------------------------------------------------------------
// any_of
//------------------------------------------------------------------------

template <class _ForwardIterator, class _Pred>
bool
__brick_any_of(const _ForwardIterator, const _ForwardIterator, _Pred,
               /*__is_vector=*/std::false_type) noexcept;

template <class _ForwardIterator, class _Pred>
bool
__brick_any_of(const _ForwardIterator, const _ForwardIterator, _Pred,
               /*__is_vector=*/std::true_type) noexcept;

template <class _ExecutionPolicy, class _ForwardIterator, class _Pred, class _IsVector>
bool
__pattern_any_of(_ExecutionPolicy&&, _ForwardIterator, _ForwardIterator, _Pred, _IsVector,
                 /*parallel=*/std::false_type) noexcept;

#if __PSTL_USE_PAR_POLICIES
template <class _ExecutionPolicy, class _ForwardIterator, class _Pred, class _IsVector>
bool
__pattern_any_of(_ExecutionPolicy&&, _ForwardIterator, _ForwardIterator, _Pred, _IsVector,
                 /*parallel=*/std::true_type);
#endif

//------------------------------------------------------------------------
// walk1 (pseudo)
//
// walk1 evaluates f(x) for each dereferenced value x drawn from [first,last)
//------------------------------------------------------------------------

template <class _ForwardIterator, class _Function>
void __brick_walk1(_ForwardIterator, _ForwardIterator, _Function,
                   /*vector=*/std::false_type) noexcept;

template <class _RandomAccessIterator, class _Function>
void __brick_walk1(_RandomAccessIterator, _RandomAccessIterator, _Function,
                   /*vector=*/std::true_type) noexcept;

template <class _ExecutionPolicy, class _ForwardIterator, class _Function, class _IsVector>
void
__pattern_walk1(_ExecutionPolicy&&, _ForwardIterator, _ForwardIterator, _Function, _IsVector,
                /*parallel=*/std::false_type) noexcept;

#if __PSTL_USE_PAR_POLICIES
template <class _ExecutionPolicy, class _ForwardIterator, class _Function, class _IsVector>
void
__pattern_walk1(_ExecutionPolicy&&, _ForwardIterator, _ForwardIterator, _Function, _IsVector,
                /*parallel=*/std::true_type);
#endif

template <class _ExecutionPolicy, class _ForwardIterator, class _Brick>
void
__pattern_walk_brick(_ExecutionPolicy&&, _ForwardIterator, _ForwardIterator, _Brick,
                     /*parallel=*/std::false_type) noexcept;

#if __PSTL_USE_PAR_POLICIES
template <class _ExecutionPolicy, class _ForwardIterator, class _Brick>
void
__pattern_walk_brick(_ExecutionPolicy&&, _ForwardIterator, _ForwardIterator, _Brick,
                     /*parallel=*/std::true_type);
#endif

//------------------------------------------------------------------------
// walk1_n
//------------------------------------------------------------------------

template <class _ForwardIterator, class _Size, class _Function>
_ForwardIterator __brick_walk1_n(_ForwardIterator, _Size, _Function,
                                 /*_IsVectorTag=*/std::false_type);

template <class _RandomAccessIterator, class _DifferenceType, class _Function>
_RandomAccessIterator __brick_walk1_n(_RandomAccessIterator, _DifferenceType, _Function,
                                      /*vectorTag=*/std::true_type) noexcept;

template <class _ExecutionPolicy, class _ForwardIterator, class _Size, class _Function, class _IsVector>
_ForwardIterator
__pattern_walk1_n(_ExecutionPolicy&&, _ForwardIterator, _Size, _Function, _IsVector,
                  /*is_parallel=*/std::false_type) noexcept;

#if __PSTL_USE_PAR_POLICIES
template <class 