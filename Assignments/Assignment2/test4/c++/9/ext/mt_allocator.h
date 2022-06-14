// MT-optimized allocator -*- C++ -*-

// Copyright (C) 2003-2019 Free Software Foundation, Inc.
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

/** @file ext/mt_allocator.h
 *  This file is a GNU extension to the Standard C++ Library.
 */

#ifndef _MT_ALLOCATOR_H
#define _MT_ALLOCATOR_H 1

#include <new>
#include <cstdlib>
#include <bits/functexcept.h>
#include <ext/atomicity.h>
#include <bits/move.h>
#if __cplusplus >= 201103L
#include <type_traits>
#endif

namespace __gnu_cxx _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  using std::size_t;
  using std::ptrdiff_t;

  typedef void (*__destroy_handler)(void*);

  /// Base class for pool object.
  struct __pool_base
  {
    // Using short int as type for the binmap implies we are never
    // caching blocks larger than 32768 with this allocator.
    typedef unsigned short int _Binmap_type;

    // Variables used to configure the behavior of the allocator,
    // assigned and explained in detail below.
    struct _Tune
     {
      // Compile time constants for the default _Tune values.
      enum { _S_align = 8 };
      enum { _S_max_bytes = 128 };
      enum { _S_min_bin = 8 };
      enum { _S_chunk_size = 4096 - 4 * sizeof(void*) };
      enum { _S_max_threads = 4096 };
      enum { _S_freelist_headroom = 10 };

      // Alignment needed.
      // NB: In any case must be >= sizeof(_Block_record), that
      // is 4 on 32 bit machines and 8 on 64 bit machines.
      size_t	_M_align;
      
      // Allocation requests (after round-up to power of 2) below
      // this value will be handled by the allocator. A raw new/
      // call will be used for requests larger than this value.
      // NB: Must be much smaller than _M_chunk_size and in any
      // case <= 32768.
      size_t	_M_max_bytes; 

      // Size in bytes of the smallest bin.
      // NB: Must be a power of 2 and >= _M_align (and of course
      // much smaller than _M_max_bytes).
      size_t	_M_min_bin;

      // In order to avoid fragmenting and minimize the number of
      // new() calls we always request new memory using this
      // value. Based on previous discussions on the libstdc++
      // mailing list we have chosen the value below.
      // See http://gcc.gnu.org/ml/libstdc++/2001-07/msg00077.html
      // NB: At least one order of magnitude > _M_max_bytes. 
      size_t	_M_chunk_size;

      // The maximum number of supported threads. For
      // single-threaded operation, use one. Maximum values will
      // vary depending on details of the underlying system. (For
      // instance, Linux 2.4.18 reports 4070 in
      // /proc/sys/kernel/threads-max, while Linux 2.6.6 reports
      // 65534)
      size_t 	_M_max_threads;

      // Each time a deallocation occurs in a threaded application
      // we make sure that there are no more than
      // _M_freelist_headroom % of used memory on the freelist. If
      // the number of additional records is more than
      // _M_freelist_headroom % of the freelist, we move these
      // records back to the global pool.
      size_t 	_M_freelist_headroom;
      
      // Set to true forces all allocations to use new