// Hashtable implementation used by containers -*- C++ -*-

// Copyright (C) 2001-2019 Free Software Foundation, Inc.
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

/*
 * Copyright (c) 1996,1997
 * Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Hewlett-Packard Company makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 */

/** @file backward/hashtable.h
 *  This file is a GNU extension to the Standard C++ Library (possibly
 *  containing extensions from the HP/SGI STL subset).
 */

#ifndef _BACKWARD_HASHTABLE_H
#define _BACKWARD_HASHTABLE_H 1

// Hashtable class, used to implement the hashed associative containers
// hash_set, hash_map, hash_multiset, and hash_multimap.

#include <vector>
#include <iterator>
#include <algorithm>
#include <bits/stl_function.h>
#include <backward/hash_fun.h>

namespace __gnu_cxx _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  using std::size_t;
  using std::ptrdiff_t;
  using std::forward_iterator_tag;
  using std::input_iterator_tag;
  using std::_Construct;
  using std::_Destroy;
  using std::distance;
  using std::vector;
  using std::pair;
  using std::__iterator_category;

  template<class _Val>
    struct _Hashtable_node
    {
      _Hashtable_node* _M_next;
      _Val _M_val;
    };

  template<class _Val, class _Key, class _HashFcn, class _ExtractKey, 
	   class _EqualKey, class _Alloc = std::allocator<_Val> >
    class hashtable;

  template<class _Val, class _Key, class _HashFcn,
	   class _ExtractKey, class _EqualKey, class _Alloc>
    struct _Hashtable_iterator;

  template<class _Val, class _Key, class _HashFcn,
	   class _ExtractKey, class _EqualKey, class _Alloc>
    struct _Hashtable_const_iterator;

  template<class _Val, class _Key, class _HashFcn,
	   class _ExtractKey, class _EqualKey, class _Alloc>
    struct _Hashtable_iterator
    {
      typedef hashtable<_Val, _Key, _HashFcn, _ExtractKey, _EqualKey, _Alloc>
        _Hashtable;
      typedef _Hashtable_iterator<_Val, _Key, _HashFcn,
				  _ExtractKey, _EqualKey, _Alloc>
        iterator;
      typedef _Hashtable_const_iterator<_Val, _Key, _HashFcn,
					_ExtractKey, _EqualKey, _Alloc>
        const_iterator;
      typedef _Hashtable_node<_Val> _Node;
   