// ABI Support -*- C++ -*-

// Copyright (C) 2000-2019 Free Software Foundation, Inc.
//
// This file is part of GCC.
//
// GCC is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3, or (at your option)
// any later version.
//
// GCC is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

// Written by Nathan Sidwell, Codesourcery LLC, <nathan@codesourcery.com>

/* This file declares the new abi entry points into the runtime. It is not
   normally necessary for user programs to include this header, or use the
   entry points directly. However, this header is available should that be
   needed.

   Some of the entry points are intended for both C and C++, thus this header
   is includable from both C and C++. Though the C++ specific parts are not
   available in C, naturally enough.  */

/** @file cxxabi.h
 *  The header provides an interface to the C++ ABI.
 */

#ifndef _CXXABI_H
#define _CXXABI_H 1

#pragma GCC system_header

#pragma GCC visibility push(default)

#include <stddef.h>
#include <bits/c++config.h>
#include <bits/cxxabi_tweaks.h>
#include <bits/cxxabi_forced.h>
#include <bits/cxxabi_init_exception.h>

#ifdef __cplusplus
namespace __cxxabiv1
{
  extern "C"
  {
#endif

  typedef __cxa_cdtor_return_type (*__cxa_cdtor_type)(void *);

  // Allocate array.
  void*
  __cxa_vec_new(size_t __element_count, size_t __element_size,
		size_t __padding_size, __cxa_cdtor_type __constructor,
		__cxa_cdtor_type __destructor);

  void*
  __cxa_vec_new2(size_t __element_count, size_t __element_size,
		 size_t __padding_size, __cxa_cdtor_type __constructor,
		 __cxa_cdtor_type __destructor, void *(*__alloc) (size_t),
		 void (*__dealloc) (void*));

  void*
  __cxa_vec_new3(size_t __element_count, size_t __element_size,
		 size_t __padding_size, __cxa_cdtor_type __constructor,
		 __cxa_cdtor_type __destructor, void *(*__alloc) (size_t),
		 void (*__dealloc) (void*, size_t));

  // Construct array.
  __cxa_vec_ctor_return_type
  __cxa_vec_ctor(void* __array_address, size_t __element_count,
		 size_t __element_size, __cxa_cdtor_type __constructor,
		 __cxa_cdtor_type __destructor);

  __cxa_vec_ctor_return_type
  __cxa_vec_cctor(void* __dest_array, void* __src_array,
		  size_t __element_count, size_t __element_size,
		  __cxa_cdtor_return_type (*__constructor) (void*, void*),
		  __cxa_cdtor_type __destructor);

  // Destruct array.
  void
  __cxa_vec_dtor(void* __array_address, size_t __element_count,
		 size_t __element_size, __cxa_cdtor_type __destructor);

  void
  __cxa_vec_cleanup(void* __array_address, size_t __element_count, size_t __s,
		    __cxa_cdtor_type __destructor) _GLIBCXX_NOTHROW;

  // Destruct and release array.
  void
  __cxa_vec_delete(void* __array_address, size_t __element_size,
		   size_t __padding_size, __cxa_cdtor_type __destructor);

  void
  __cxa_vec_delete2(void* __array_address, size_t __element_size,
		    size_t __padding_size, __cxa_cdtor_type __destructor,
		    void (*__dealloc) (void*));

  void
  __cxa_vec_delete3(void* __array_address, size_t __element_size,
		    size_t __padding_size, __cxa_cdtor_type __destructor,
		    void (*__dealloc) (void*, size_t));

  int
  __cxa_guard_acquire(__guard*);

  void
  __cxa_guard_release(__guard*) _GLIBCXX_NOTHROW;

  void
  __cxa_guard_abort(__guard*) _GLIBCXX_NOTHROW;

  // DSO destruction.
  int
  __cxa_atexit(void 