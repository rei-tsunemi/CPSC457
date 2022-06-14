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

/** @file profile/impl/profiler_map_to_unordered_map.h
 *  @brief Diagnostics for map to unordered_map.
 */

// Written by Silvius Rus.

#ifndef _GLIBCXX_PROFILE_PROFILER_MAP_TO_UNORDERED_MAP_H
#define _GLIBCXX_PROFILE_PROFILER_MAP_TO_UNORDERED_MAP_H 1

#include "profile/impl/profiler.h"
#include "profile/impl/profiler_node.h"
#include "profile/impl/profiler_trace.h"

namespace __gnu_profile
{
  inline int
  __log2(std::size_t __size)
  {
    for (int __bit_count = sizeof(std::size_t) - 1; __bit_count >= 0;
	 -- __bit_count)
      if ((2 << __bit_count) & __size)
	return __bit_count;
    return 0;
  }

  inline float
  __map_insert_cost(std::size_t __size)
  { return (_GLIBCXX_PROFILE_DATA(__map_insert_cost_factor).__value
	    * static_cast<float>(__log2(__size))); }

  inline float
  __map_erase_cost(std::size_t __size)
  { return (_GLIBCXX_PROFILE_DATA(__map_erase_cost_factor).__value
	    * static_cast<float>(__log2(__size))); }

  inline float
  __map_find_cost(std::size_t __size)
  { return (_GLIBCXX_PROFILE_DATA(__map_find_cost_factor).__value
	    * static_cast<float>(__log2(__size))); }

  /** @brief A map-to-unordered_map instrumentation line in the
      object table.  */
  class __map2umap_info
  : public __object_info_base
  {
  public:
    __map2umap_info(__stack_t __stack)
    : __object_info_base(__stack), _M_insert(0), _M_erase(0), _M_find(0),
      _M_iterate(0), _M_umap_cost(0.0), _M_map_cost(0.0)
    { }

    void
    __merge(const __map2umap_info& __o)
    {
      __object_info_base::__merge(__o);
      _M_insert		+= __o._M_insert;
      _M_erase		+= __o._M_erase;
      _M_find		+= __o._M_find;
      _M_iterate	+= __o._M_iterate;
      _M_umap_cost	+= __o._M_umap_cost;
      _M_map_cost	+= __o._M_map_cost;
    }

    void
    __write(FILE* __f) const
    {
      std::fprintf(__f, "%Zu %Zu %Zu %Zu %.0f %.0f\n",
		   _M_insert, _M_erase, _M_find, _M_iterate, _M_map_cost,
		   _M_umap_cost);
    }

    float
    __magnitude() const
    { return _M_map_cost - _M_umap_cost; }

    std::string
    __advice() const
    { return "prefer an unordered container"; }

    void
    __record_insert(std::size_t __size, std::size_t __count)
    {
      ++_M_insert;
      if (__count)
	{
	  _M_map_cost += __count * __map_insert_cost(__size);
	  _M_umap_cost
	    += (__count
		* _GLIBCXX_PROFILE_DATA(__umap_insert_cost_factor).__value);
	}
    }

    void
    __record_erase(std::size_t __size, std::size_t __count)
    {
      ++_M_erase;
      if (__count)
	{
	  _M_map_cost += __count * __map_erase_cost(__size);
	  _M_umap_cost
	    += (__count
		* _GLIBCXX_PROFILE_DATA(__umap_erase_cost_factor).__value);
	}
    }

    void
    __record_find(std::size_t __size)
    {
      ++_M_find;
      _M_map_cost += __map_find_cost(__size);
      _M_umap_cost += _GLIBCXX_PROFILE_DATA(__umap_find_cost_factor).__value;
    }

    void
    __record_iterate(int __count)
    { __gnu_cxx::__atomic_add(&_M_iterate, __count); }

    void
    __set_iterate_costs()
    {
      _M_umap_cost
	+= (_M_iterate
	    * _GLIBCXX_PROFILE_DATA(__umap_iterate_cost_factor).__value);
      _M_map_