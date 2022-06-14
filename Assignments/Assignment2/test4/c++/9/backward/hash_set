// Hashing set implementation -*- C++ -*-

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
 * Copyright (c) 1996
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

/** @file backward/hash_set
 *  This file is a GNU extension to the Standard C++ Library (possibly
 *  containing extensions from the HP/SGI STL subset).
 */

#ifndef _BACKWARD_HASH_SET
#define _BACKWARD_HASH_SET 1

#ifndef _GLIBCXX_PERMIT_BACKWARD_HASH
#include <backward/backward_warning.h>
#endif

#include <bits/c++config.h>
#include <backward/hashtable.h>
#include <bits/concept_check.h>

namespace __gnu_cxx _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  using std::equal_to;
  using std::allocator;
  using std::pair;
  using std::_Identity;

  /**
   *  This is an SGI extension.
   *  @ingroup SGIextensions
   *  @doctodo
   */
  template<class _Value, class _HashFcn  = hash<_Value>,
	   class _EqualKey = equal_to<_Value>,
	   class _Alloc = allocator<_Value> >
    class hash_set
    {
      // concept requirements
      __glibcxx_class_requires(_Value, _SGIAssignableConcept)
      __glibcxx_class_requires3(_HashFcn, size_t, _Value, _UnaryFunctionConcept)
      __glibcxx_class_requires3(_EqualKey, _Value, _Value, _BinaryPredicateConcept)

    private:
      typedef hashtable<_Value, _Value, _HashFcn, _Identity<_Value>,
			_EqualKey, _Alloc> _Ht;
      _Ht _M_ht;

    public:
      typedef typename _Ht::key_type key_type;
      typedef typename _Ht::value_type value_type;
      typedef typename _Ht::hasher hasher;
      typedef typename _Ht::key_equal key_equal;
      
      typedef typename _Ht::size_type size_type;
      typedef typename _Ht::difference_type difference_type;
      typedef typename _Alloc::pointer pointer;
      typedef typename _Alloc::const_pointer const_pointer;
      typedef typename _Alloc::reference reference;
      typedef typename _Alloc::const_reference const_reference;
      
      typedef typename _Ht::const_iterator iterator;
      typedef typename _Ht::const_iterator const_iterator;
      
      typedef typename _Ht::allocator_ty