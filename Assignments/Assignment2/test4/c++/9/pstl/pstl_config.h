// -*- C++ -*-
//===-- pstl_config.h -----------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef __PSTL_config_H
#define __PSTL_config_H

#define PSTL_VERSION 203
#define PSTL_VERSION_MAJOR (PSTL_VERSION / 100)
#define PSTL_VERSION_MINOR (PSTL_VERSION - PSTL_VERSION_MAJOR * 100)

// Check the user-defined macro for parallel policies
#if defined(PSTL_USE_PARALLEL_POLICIES)
#undef __PSTL_USE_PAR_POLICIES
#define __PSTL_USE_PAR_POLICIES PSTL_USE_PARALLEL_POLICIES
// Check the internal macro for parallel policies
#elif !defined(__PSTL_USE_PAR_POLICIES)
#define __PSTL_USE_PAR_POLICIES 1
#endif

#if __PSTL_USE_PAR_POLICIES
#if !defined(__PSTL_PAR_BACKEND_TBB)
#define __PSTL_PAR_BACKEND_TBB 1
#endif
#else
#undef __PSTL_PAR_BACKEND_TBB
#endif

// Check the user-defined macro for warnings
#if defined(PSTL_USAGE_WARNINGS)
#undef __PSTL_USAGE_WARNINGS
#define __PSTL_USAGE_WARNINGS PSTL_USAGE_WARNINGS
// Check the internal macro for warnings
#elif !defined(__PSTL_USAGE_WARNINGS)
#define __PSTL_USAGE_WARNINGS 0
#endif

// Portability "#pragma" definition
#ifdef _MSC_VER
#define __PSTL_PRAGMA(x) __pragma(x)
#else
#define __PSTL_PRAGMA(x) _Pragma(#x)
#endif

#define __PSTL_STRING_AUX(x) #x
#define __PSTL_STRING(x) __PSTL_STRING_AUX(x)
#define __PSTL_STRING_CONCAT(x, y) x #y

// note that when ICC or Clang is in use, __PSTL_GCC_VERSION might not fully match
// the actual GCC version on the system.
#define __PSTL_GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)

#if __clang__
// according to clang documentation, version can be vendor specific
#define __PSTL_CLANG_VERSION (__clang_major__ * 10000 + __clang_minor__ * 100 + __clang_patchlevel__)
#endif

// Enable SIMD for compilers that support OpenMP 4.0
#if (_OPENMP >= 201307) || (__INTEL_COMPILER >= 1600) || (!defined(__INTEL_COMPILER) && __PSTL_GCC_VERSION >= 40900)
#define __PSTL_PRAGMA_SIMD __PSTL_PRAGMA(omp simd)
#define __PSTL_PRAGMA_DECLARE_SIMD __PSTL_PRAGMA(omp declare simd)
#define __PSTL_PRAGMA_SIMD_REDUCTION(PRM) __PSTL_PRAGMA(omp simd reduction(PRM))
#elif !defined(_MSC_VER) //#pragma simd
#define __PSTL_PRAGMA_SIMD __PSTL_PRAGMA(simd)
#define __PSTL_PRAGMA_DECLARE_SIMD
#define __PSTL_PRAGMA_SIMD_REDUCTION(PRM) __PSTL_PRAGMA(simd reduction(PRM))
#else //no simd
#define __PSTL_PRAGMA_SIMD
#define __PSTL_PRAGMA_DECLARE_SIMD
#define __PSTL_PRAGMA_SIMD_REDUCTION(PRM)
#endif //Enable SIMD

#if (__INTEL_COMPILER)
#define __PSTL_PRAGMA_FORCEINLINE __PSTL_PRAGMA(forceinline)
#else
#define __PSTL_PRAGMA_FORCEINLINE
#endif

#if (__INTEL_COMPILER >= 1900)
#define __PSTL_PRAGMA_SIMD_SCAN(PRM) __PSTL_PRAGMA(omp simd reduction(inscan, PRM))
#define __PSTL_PRAGMA_SIMD_INCLUSIVE_SCAN(PRM) __PSTL_PRAGMA(omp scan inclusive(PRM))
#define __PSTL_PRAGMA_SIMD_EXCLUSIVE_SCAN(PRM) __PSTL_PRAGMA(omp scan exclusive(PRM))
#else
#define __PSTL_PRAGMA_SIMD_SCAN(PRM)
#define __PSTL_PRAGMA_SIMD_INCLUSIVE_SCAN(PRM)
#define __PSTL_PRAGMA_SIMD_EXCLUSIVE_SCAN(PRM)
#endif

// Should be defined to 1 for environments with a vendor implementation of C++17 execution policies
#define __PSTL_CPP17_EXECUTION_POLICIES_PRESENT (_MSC_VER >= 1912)

#define __PSTL_CPP14_2RANGE_MISMATCH_EQUAL_PRESENT                                                                     \
    (_MSC_VER >= 1900 || __cplusplus >= 201300L || __cpp_lib_robust_nonmodifying_seq_ops == 201304)
#define __PSTL_CPP14_MAKE_REVERSE_ITERATOR_PRESENT                                                                     \
    (_MSC_VER >= 1900 || __cplusplus >= 201402L || __cpp_lib_make_reverse_iterator == 201402)
#define __PSTL_CPP14_INTEGER_SEQUENCE_PRESENT (_MSC_VER >= 1900 || __cplusplus >= 201402L)
#define __PSTL_CPP14_VARIABLE_TEMPLATES_PRESENT                                                                        \
    (!__INTEL_COMPILE