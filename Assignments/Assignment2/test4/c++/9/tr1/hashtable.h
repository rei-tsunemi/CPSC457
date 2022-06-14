// TR1 hashtable.h header -*- C++ -*-

// Copyright (C) 2007-2019 Free Software Foundation, Inc.
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

/** @file tr1/hashtable.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly.
 *  @headername{tr1/unordered_set, tr1/unordered_map}
 */

#ifndef _GLIBCXX_TR1_HASHTABLE_H
#define _GLIBCXX_TR1_HASHTABLE_H 1

#pragma GCC system_header

#include <tr1/hashtable_policy.h>

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

namespace tr1
{
  // Class template _Hashtable, class definition.

  // Meaning of class template _Hashtable's template parameters

  // _Key and _Value: arbitrary CopyConstructible types.

  // _Allocator: an allocator type ([lib.allocator.requirements]) whose
  // value type is Value.  As a conforming extension, we allow for
  // value type != Value.

  // _ExtractKey: function object that takes a object of type Value
  // and returns a value of type _Key.

  // _Equal: function object that takes two objects of type k and returns
  // a bool-like value that is true if the two objects are considered equal.

  // _H1: the hash function.  A unary function object with argument type
  // Key and result type size_t.  Return values should be distributed
  // over the entire range [0, numeric_limits<size_t>:::max()].

  // _H2: the range-hashing function (in the terminology of Tavori and
  // Dreizin).  A binary function object whose argument types and result
  // type are all size_t.  Given arguments r and N, the return value is
  // in the range [0, N).

  // _Hash: the ranged hash function (Tavori and Dreizin). A binary function
  // whose argument types are _Key and size_t and whose result type is
  // size_t.  Given arguments k and N, the return value is in the range
  // [0, N).  Default: hash(k, N) = h2(h1(k), N).  If _Hash is anything other
  // than the default, _H1 and _H2 are ignored.

  // _RehashPolicy: Policy class with three members, all of which govern
  // the bucket count. _M_next_bkt(n) returns a bucket count no smaller
  // than n.  _M_bkt_for_elements(n) returns a bucket count appropriate
  // for an element count of n.  _M_need_rehash(n_bkt, n_elt, n_ins)
  // determines whether, if the current bucket count is n_bkt and the
  // current element count is n_elt, we need to increase the bucket
  // count.  If so, returns make_pair(true, n), where n is the new
  // bucket count.  If not, returns make_pair(false, <anything>).

  // ??? Right now it is hard-wired that the number of buckets never
  // shrinks.  Should we allow _RehashPolicy to change that?

  // __cache_hash_code: bool.  true if we store the value of the hash
  // function along with the value.  This is a time-space tradeoff.
  // Storing it may improve lookup speed by reducing the number of times
  // we need to call the Equal function.

  // __constant_iterators: bool.  true if iterator and const_iterator are
  // both constant iterator types.  This is true for unordered_set and
  // unordered_multiset, false for unordered_map and unordered_multimap.

  // __unique_keys: bool.  true if t