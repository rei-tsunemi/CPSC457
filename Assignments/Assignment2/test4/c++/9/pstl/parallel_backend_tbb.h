// -*- C++ -*-
//===-- parallel_backend_tbb.h --------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef __PSTL_parallel_backend_tbb_H
#define __PSTL_parallel_backend_tbb_H

#include <algorithm>
#include <type_traits>

#include "parallel_backend_utils.h"

// Bring in minimal required subset of Intel TBB
#include <tbb/blocked_range.h>
#include <tbb/parallel_for.h>
#include <tbb/parallel_reduce.h>
#include <tbb/parallel_scan.h>
#include <tbb/parallel_invoke.h>
#include <tbb/task_arena.h>
#include <tbb/tbb_allocator.h>

#if TBB_INTERFACE_VERSION < 10000
#error Intel(R) Threading Building Blocks 2018 is required; older versions are not supported.
#endif

namespace __pstl
{
namespace __par_backend
{

//! Raw memory buffer with automatic freeing and no exceptions.
/** Some of our algorithms need to start with raw memory buffer,
not an initialize array, because initialization/destruction
would make the span be at least O(N). */
// tbb::allocator can improve performance in some cases.
template <typename _Tp>
class __buffer
{
    tbb::tbb_allocator<_Tp> _M_allocator;
    _Tp* _M_ptr;
    const std::size_t _M_buf_size;
    __buffer(const __buffer&) = delete;
    void
    operator=(const __buffer&) = delete;

  public:
    //! Try to obtain buffer of given size to store objects of _Tp type
    __buffer(std::size_t n) : _M_allocator(), _M_ptr(_M_allocator.allocate(n)), _M_buf_size(n) {}
    //! True if buffer was successfully obtained, zero otherwise.
    operator bool() const { return _M_ptr != NULL; }
    //! Return pointer to buffer, or  NULL if buffer could not be obtained.
    _Tp*
    get() const
    {
        return _M_ptr;
    }
    //! Destroy buffer
    ~__buffer() { _M_allocator.deallocate(_M_ptr, _M_buf_size); }
};

// Wrapper for tbb::task
inline void
__cancel_execution()
{
    tbb::task::self().group()->cancel_group_execution();
}

//------------------------------------------------------------------------
// parallel_for
//------------------------------------------------------------------------

template <class _Index, class _RealBody>
class __parallel_for_body
{
  public:
    __parallel_for_body(const _RealBody& __body) : _M_body(__body) {}
    __parallel_for_body(const __parallel_for_body& __body) : _M_body(__body._M_body) {}
    void
    operator()(const tbb::blocked_range<_Index>& __range) const
    {
        _M_body(__range.begin(), __range.end());
    }

  private:
    _RealBody _M_body;
};

//! Evaluation of brick f[i,j) for each subrange [i,j) of [first,last)
// wrapper over tbb::parallel_for
template <class _ExecutionPolicy, class _Index, class _Fp>
void
__parallel_for(_ExecutionPolicy&&, _Index __first, _Index __last, _Fp __f)
{
    tbb::this_task_arena::isolate([=]() {
        tbb::parallel_for(tbb::blocked_range<_Index>(__first, __last), __parallel_for_body<_Index, _Fp>(__f));
    });
}

//! Evaluation of brick f[i,j) for each subrange [i,j) of [first,last)
// wrapper over tbb::parallel_reduce
template <class _ExecutionPolicy, class _Value, class _Index, typename _RealBody, typename _Reduction>
_Value
__parallel_reduce(_ExecutionPolicy&&, _Index __first, _Index __last, const _Value& __identity,
                  const _RealBody& __real_body, const _Reduction& __reduction)
{
    return tbb::this_task_arena::isolate([__first, __last, &__identity, &__real_body, &__reduction]() -> _Value {
        return tbb::parallel_reduce(
            tbb::blocked_range<_Index>(__first, __last), __identity,
            [__real_body](const tbb::blocked_range<_Index>& __r, const _Value& __value) -> _Value {
                return __real_body(__r.begin(), __r.end(), __value);
            },
            __reduction);
    });
}

//------------------------------------------------------------------------
// parallel_transform_reduce
//
// Notation:
