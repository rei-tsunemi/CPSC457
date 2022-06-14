// -*- C++ -*-
//
// Copyright (C) 2009-2019 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

/** @file profile/impl/profiler.h
 *  @brief Interface of the profiling runtime library.
 */

// Written by Lixia Liu and Silvius Rus.

#ifndef _GLIBCXX_PROFILE_PROFILER_H
#define _GLIBCXX_PROFILE_PROFILER_H 1

#include <bits/c++config.h>

// Mechanism to define data with inline linkage.
#define _GLIBCXX_PROFILE_DEFINE_UNINIT_DATA(__type, __name)             \
  inline __type&                                                        \
  __get_##__name()                                                      \
  {                                                                     \
    static __type __name;                                               \
    return __name;                                                      \
  }
#define _GLIBCXX_PROFILE_DEFINE_DATA(__type, __name, __initial_value...) \
  inline __type& __get_##__name() {                                      \
    static __type __name(__initial_value);                               \
    return __name;                                                       \
  }
#define _GLIBCXX_PROFILE_DATA(__name) \
  __get_##__name()

namespace __gnu_profile
{
  /** @brief Reentrance guard.
   *
   * Mechanism to protect all __gnu_profile operations against recursion,
   * multithreaded and exception reentrance.
   */
  struct __reentrance_guard
  {
    static bool
    __get_in()
    {
      if (__inside() == true)
	return false;
      else
	{
	  __inside() = true;
	  return true;
	}
    }

    static bool&
    __inside()
    {
      static __thread bool _S_inside(false);
      return _S_inside;
    }

    __reentrance_guard() { }
    ~__reentrance_guard() { __inside() = false; }
  };

  // Forward declarations of implementation functions.
  // Don't use any __gnu_profile:: in user code.
  // Instead, use the __profcxx... macros, which offer guarded access.
  class __container_size_info;
  class __hashfunc_info;
  class __map2umap_info;
  class __vector2list_info;
  class __list2slist_info;
  class __list2vector_info;

  bool __turn_on();
  bool __turn_off();
  bool __is_invalid();
  bool __is_on();
  bool __is_off();
  void __report();

  __container_size_info*
  __trace_hashtable_size_construct(std::size_t);
  void __trace_hashtable_size_resize(__container_size_info*,
				     std::size_t, std::size_t);
  void __trace_hashtable_size_destruct(__container_size_info*,
				       std::size_t, std::size_t);

  __hashfunc_info*
  __trace_hash_func_construct();
  void __trace_hash_func_destruct(__hashfunc_info*,
				  std::size_t, std::size_t, std::size_t);

  __container_size_info*
  __trace_vector_size_construct(std::size_t);
  void __trace_vector_size_resize(__container_size_info*,
				  std::size_t, std::size_t);
  void __trace_vector_size_destruct(__container_size_info*,
				    std::size_t, std::size_t);

  __vector2list_info*
  __trace_vector_to_list_construct();
  void __trace_vector_to_list_insert(__vector2list_info*,
				     std::size_t, std::size_t);
  void __trace_vector_to_list_iterate(__vector2list_info*, int);
  void __trace_vector_to_list_invalid_operator(__vector2list_info*);
  void __trace_vector_to_lis