// unordered_map implementation -*- C++ -*-

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

/** @file bits/unordered_map.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{unordered_map}
 */

#ifndef _UNORDERED_MAP_H
#define _UNORDERED_MAP_H

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION
_GLIBCXX_BEGIN_NAMESPACE_CONTAINER

  /// Base types for unordered_map.
  template<bool _Cache>
    using __umap_traits = __detail::_Hashtable_traits<_Cache, false, true>;

  template<typename _Key,
	   typename _Tp,
	   typename _Hash = hash<_Key>,
	   typename _Pred = std::equal_to<_Key>,
	   typename _Alloc = std::allocator<std::pair<const _Key, _Tp> >,
	   typename _Tr = __umap_traits<__cache_default<_Key, _Hash>::value>>
    using __umap_hashtable = _Hashtable<_Key, std::pair<const _Key, _Tp>,
                                        _Alloc, __detail::_Select1st,
				        _Pred, _Hash,
				        __detail::_Mod_range_hashing,
				        __detail::_Default_ranged_hash,
				        __detail::_Prime_rehash_policy, _Tr>;

  /// Base types for unordered_multimap.
  template<bool _Cache>
    using __ummap_traits = __detail::_Hashtable_traits<_Cache, false, false>;

  template<typename _Key,
	   typename _Tp,
	   typename _Hash = hash<_Key>,
	   typename _Pred = std::equal_to<_Key>,
	   typename _Alloc = std::allocator<std::pair<const _Key, _Tp> >,
	   typename _Tr = __ummap_traits<__cache_default<_Key, _Hash>::value>>
    using __ummap_hashtable = _Hashtable<_Key, std::pair<const _Key, _Tp>,
					 _Alloc, __detail::_Select1st,
					 _Pred, _Hash,
					 __detail::_Mod_range_hashing,
					 __detail::_Default_ranged_hash,
					 __detail::_Prime_rehash_policy, _Tr>;

  template<class _Key, class _Tp, class _Hash, class _Pred, class _Alloc>
    class unordered_multimap;

  /**
   *  @brief A standard container composed of unique keys (containing
   *  at most one of each key value) that associates values of another type
   *  with the keys.
   *
   *  @ingroup unordered_associative_containers
   *
   *  @tparam  _Key    Type of key objects.
   *  @tparam  _Tp     Type of mapped objects.
   *  @tparam  _Hash   Hashing function object type, defaults to hash<_Value>.
   *  @tparam  _Pred   Predicate function object type, defaults
   *                   to equal_to<_Value>.
   *  @tparam  _Alloc  Allocator type, defaults to 
   *                   std::allocator<std::pair<const _Key, _Tp>>.
   *
   *  Meets the requirements of a <a href="tables.html#65">container</a>, and
   *  <a href="tables.html#xx">unordered associative container</a>
   *
   * The resulting value type of the container is std::pair<const _Key, _Tp>.
   *
   *  Base is _Hashtable, dispatched at compile time via template
   *  alias __umap_hashtable.
   */
  template<typename _Key, typename _Tp,
	   typename _Hash = hash<_Key>,
	   typename _Pred = equal_to<_Key>,
	   typename _Alloc = allocator<std::pair<const _Key, _Tp>>>
    class unordered_map
    {
      typedef __umap_hashtable<_Key, _Tp, _Ha