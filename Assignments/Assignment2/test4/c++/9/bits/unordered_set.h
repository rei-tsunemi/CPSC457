// unordered_set implementation -*- C++ -*-

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

/** @file bits/unordered_set.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{unordered_set}
 */

#ifndef _UNORDERED_SET_H
#define _UNORDERED_SET_H

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION
_GLIBCXX_BEGIN_NAMESPACE_CONTAINER

  /// Base types for unordered_set.
  template<bool _Cache>
    using __uset_traits = __detail::_Hashtable_traits<_Cache, true, true>;

  template<typename _Value,
	   typename _Hash = hash<_Value>,
	   typename _Pred = std::equal_to<_Value>,
  	   typename _Alloc = std::allocator<_Value>,
	   typename _Tr = __uset_traits<__cache_default<_Value, _Hash>::value>>
    using __uset_hashtable = _Hashtable<_Value, _Value, _Alloc,
					__detail::_Identity, _Pred, _Hash,
					__detail::_Mod_range_hashing,
					__detail::_Default_ranged_hash,
					__detail::_Prime_rehash_policy, _Tr>;

  /// Base types for unordered_multiset.
  template<bool _Cache>
    using __umset_traits = __detail::_Hashtable_traits<_Cache, true, false>;

  template<typename _Value,
	   typename _Hash = hash<_Value>,
	   typename _Pred = std::equal_to<_Value>,
	   typename _Alloc = std::allocator<_Value>,
	   typename _Tr = __umset_traits<__cache_default<_Value, _Hash>::value>>
    using __umset_hashtable = _Hashtable<_Value, _Value, _Alloc,
					 __detail::_Identity,
					 _Pred, _Hash,
					 __detail::_Mod_range_hashing,
					 __detail::_Default_ranged_hash,
					 __detail::_Prime_rehash_policy, _Tr>;

  template<class _Value, class _Hash, class _Pred, class _Alloc>
    class unordered_multiset;

  /**
   *  @brief A standard container composed of unique keys (containing
   *  at most one of each key value) in which the elements' keys are
   *  the elements themselves.
   *
   *  @ingroup unordered_associative_containers
   *
   *  @tparam  _Value  Type of key objects.
   *  @tparam  _Hash  Hashing function object type, defaults to hash<_Value>.

   *  @tparam _Pred Predicate function object type, defaults to
   *                equal_to<_Value>.
   *
   *  @tparam  _Alloc  Allocator type, defaults to allocator<_Key>.
   *
   *  Meets the requirements of a <a href="tables.html#65">container</a>, and
   *  <a href="tables.html#xx">unordered associative container</a>
   *
   *  Base is _Hashtable, dispatched at compile time via template
   *  alias __uset_hashtable.
   */
  template<typename _Value,
	   typename _Hash = hash<_Value>,
	   typename _Pred = equal_to<_Value>,
	   typename _Alloc = allocator<_Value>>
    class unordered_set
    {
      typedef __uset_hashtable<_Value, _Hash, _Pred, _Alloc>  _Hashtable;
      _Hashtable _M_h;

    public:
      // typedefs:
      //@{
      /// Public typedefs.
      typedef typename _Hashtable::key_type	key_type;
      typedef typename _Hashtable::value_type	value_type;
      typedef typename _Hashtable::hasher	hasher;
      typedef typename _Hashtable::key_equal	key_equal;
      typedef typename _Hashtable::allocator