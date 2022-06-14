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

/** @file profile/impl/profiler_trace.h
 *  @brief Data structures to represent profiling traces.
 */

// Written by Lixia Liu and Silvius Rus.

#ifndef _GLIBCXX_PROFILE_PROFILER_TRACE_H
#define _GLIBCXX_PROFILE_PROFILER_TRACE_H 1

#include <cstdio>  // fopen, fclose, fprintf, FILE
#include <cerrno>
#include <cstdlib> // atof, atoi, strtol, getenv, atexit, abort

#if __cplusplus >= 201103L
#include <unordered_map>
#define _GLIBCXX_IMPL_UNORDERED_MAP std::_GLIBCXX_STD_C::unordered_map
#else
#include <tr1/unordered_map>
#define _GLIBCXX_IMPL_UNORDERED_MAP std::tr1::unordered_map
#endif

#include <ext/concurrence.h>
#include <fstream>
#include <string>
#include <utility>
#include <vector>

#include "profile/impl/profiler_algos.h"
#include "profile/impl/profiler_state.h"
#include "profile/impl/profiler_node.h"

namespace __gnu_profile
{
  /** @brief Internal environment.  Values can be set one of two ways:
      1. In config file "var = value".  The default config file path is 
	 libstdcxx-profile.conf.
      2. By setting process environment variables.  For instance, in a Bash
	 shell you can set the unit cost of iterating through a map like this:
	 export __map_iterate_cost_factor=5.0.
	 If a value is set both in the input file and through an environment
	 variable, the environment value takes precedence.  */
  typedef _GLIBCXX_IMPL_UNORDERED_MAP<std::string, std::string> __env_t;

  _GLIBCXX_PROFILE_DEFINE_UNINIT_DATA(__env_t, __env);

  /** @brief Master lock.  */
  _GLIBCXX_PROFILE_DEFINE_UNINIT_DATA(__gnu_cxx::__mutex, __global_mutex);

  /** @brief Representation of a warning.  */
  struct __warning_data
  {
    float __magnitude;
    __stack_t __context;
    const char* __warning_id;
    std::string __warning_message;

    __warning_data()
    : __magnitude(0.0), __context(0), __warning_id(0) { }

    __warning_data(float __m, __stack_t __c, const char* __id, 
		   const std::string& __msg)
    : __magnitude(__m), __context(__c), __warning_id(__id), 
      __warning_message(__msg) { }

    bool
    operator<(const __warning_data& __other) const
    { return __magnitude < __other.__magnitude; }
  };

  typedef std::_GLIBCXX_STD_C::vector<__warning_data> __warning_vector_t;

  // Defined in profiler_<diagnostic name>.h.
  class __trace_hash_func;
  class __trace_hashtable_size;
  class __trace_map2umap;
  class __trace_vector_size;
  class __trace_vector_to_list;
  class __trace_list_to_slist; 
  class __trace_list_to_vector; 
  void __trace_vector_size_init();
  void __trace_hashtable_size_init();
  void __trace_hash_func_init();
  void __trace_vector_to_list_init();
  void __trace_list_to_slist_init();  
  void __trace_list_to_vector_init();  
  void __trace_map_to_unordered_map_init();
  void __trace_vector_size_report(FILE*, __warning_vector_t&);
  void __trace_hashtable_size_report(FILE*, __warning_vector_t&);
  void __trace_hash_func_report(FILE*, __warning_vector_t&);
  void __trace_vector_to_list_report(FILE*, __warning_vector_t&);
  void __trace_list_to_slist_report(FILE*, __warning_vector_t&); 
  void __trace_list_to_vector_report(FILE*, __warning_vector