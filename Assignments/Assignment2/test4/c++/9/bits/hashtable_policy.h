// Internal policy header for unordered_set and unordered_map -*- C++ -*-

// Copyright (C) 2010-2019 Free Software Foundation, Inc.
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

/** @file bits/hashtable_policy.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly.
 *  @headername{unordered_map,unordered_set}
 */

#ifndef _HASHTABLE_POLICY_H
#define _HASHTABLE_POLICY_H 1

#include <tuple>		// for std::tuple, std::forward_as_tuple
#include <limits>		// for std::numeric_limits
#include <bits/stl_algobase.h>	// for std::min.

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  template<typename _Key, typename _Value, typename _Alloc,
	   typename _ExtractKey, typename _Equal,
	   typename _H1, typename _H2, typename _Hash,
	   typename _RehashPolicy, typename _Traits>
    class _Hashtable;

namespace __detail
{
  /**
   *  @defgroup hashtable-detail Base and Implementation Classes
   *  @ingroup unordered_associative_containers
   *  @{
   */
  template<typename _Key, typename _Value,
	   typename _ExtractKey, typename _Equal,
	   typename _H1, typename _H2, typename _Hash, typename _Traits>
    struct _Hashtable_base;

  // Helper function: return distance(first, last) for forward
  // iterators, or 0/1 for input iterators.
  template<class _Iterator>
    inline typename std::iterator_traits<_Iterator>::difference_type
    __distance_fw(_Iterator __first, _Iterator __last,
		  std::input_iterator_tag)
    { return __first != __last ? 1 : 0; }

  template<class _Iterator>
    inline typename std::iterator_traits<_Iterator>::difference_type
    __distance_fw(_Iterator __first, _Iterator __last,
		  std::forward_iterator_tag)
    { return std::distance(__first, __last); }

  template<class _Iterator>
    inline typename std::iterator_traits<_Iterator>::difference_type
    __distance_fw(_Iterator __first, _Iterator __last)
    { return __distance_fw(__first, __last,
			   std::__iterator_category(__first)); }

  struct _Identity
  {
    template<typename _Tp>
      _Tp&&
      operator()(_Tp&& __x) const
      { return std::forward<_Tp>(__x); }
  };

  struct _Select1st
  {
    template<typename _Tp>
      auto
      operator()(_Tp&& __x) const
      -> decltype(std::get<0>(std::forward<_Tp>(__x)))
      { return std::get<0>(std::forward<_Tp>(__x)); }
  };

  template<typename _NodeAlloc>
    struct _Hashtable_alloc;

  // Functor recycling a pool of nodes and using allocation once the pool is
  // empty.
  template<typename _NodeAlloc>
    struct _ReuseOrAllocNode
    {
    private:
      using __node_alloc_type = _NodeAlloc;
      using __hashtable_alloc = _Hashtable_alloc<__node_alloc_type>;
      using __node_alloc_traits =
	typename __hashtable_alloc::__node_alloc_traits;
      using __node_type = typename __hashtable_alloc::__node_type;

    public:
      _ReuseOrAllocNode(__node_type* __nodes, __hashtable_alloc& __h)
	: _M_nodes(__nodes), _M_h(__h) { }
      _ReuseOrAllocNode(const _ReuseOrAllocNode&) = delete;

      ~_ReuseOrAllocNode()
      { _M_h._M_deallocate_nodes(_M_nodes); }

      template<typename _Arg>
	_