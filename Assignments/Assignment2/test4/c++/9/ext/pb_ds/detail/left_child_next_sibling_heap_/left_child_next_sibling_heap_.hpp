// -*- C++ -*-

// Copyright (C) 2005-2019 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License as published by the Free Software
// Foundation; either version 3, or (at your option) any later
// version.

// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

// Copyright (C) 2004 Ami Tavory and Vladimir Dreizin, IBM-HRL.

// Permission to use, copy, modify, sell, and distribute this software
// is hereby granted without fee, provided that the above copyright
// notice appears in all copies, and that both that copyright notice
// and this permission notice appear in supporting documentation. None
// of the above authors, nor IBM Haifa Research Laboratories, make any
// representation about the suitability of this software for any
// purpose. It is provided "as is" without express or implied
// warranty.

/**
 * @file left_child_next_sibling_heap_/left_child_next_sibling_heap_.hpp
 * Contains an implementation class for a basic heap.
 */

#ifndef PB_DS_LEFT_CHILD_NEXT_SIBLING_HEAP_HPP
#define PB_DS_LEFT_CHILD_NEXT_SIBLING_HEAP_HPP

/*
 * Based on CLRS.
 */

#include <iterator>
#include <ext/pb_ds/detail/cond_dealtor.hpp>
#include <ext/pb_ds/detail/type_utils.hpp>
#include <ext/pb_ds/detail/left_child_next_sibling_heap_/node.hpp>
#include <ext/pb_ds/detail/left_child_next_sibling_heap_/point_const_iterator.hpp>
#include <ext/pb_ds/detail/left_child_next_sibling_heap_/const_iterator.hpp>
#ifdef PB_DS_LC_NS_HEAP_TRACE_
#include <iostream>
#endif
#include <debug/debug.h>

namespace __gnu_pbds
{
  namespace detail
  {
#ifdef _GLIBCXX_DEBUG
#define PB_DS_CLASS_T_DEC \
    template<typename Value_Type, typename Cmp_Fn, typename Node_Metadata, \
	     typename _Alloc, bool Single_Link_Roots>

#define PB_DS_CLASS_C_DEC \
    left_child_next_sibling_heap<Value_Type, Cmp_Fn, Node_Metadata,	\
				  _Alloc, Single_Link_Roots>
#else
#define PB_DS_CLASS_T_DEC \
    template<typename Value_Type, typename Cmp_Fn, typename Node_Metadata, \
	     typename _Alloc>

#define PB_DS_CLASS_C_DEC \
    left_child_next_sibling_heap<Value_Type, Cmp_Fn, Node_Metadata, _Alloc>
#endif

    /// Base class for a basic heap.
    template<typename Value_Type,
	     typename Cmp_Fn,
	     typename Node_Metadata,
	     typename _Alloc
#ifdef _GLIBCXX_DEBUG
	     ,bool Single_Link_Roots>
#else
	     >
#endif
    class left_child_next_sibling_heap : public Cmp_Fn
    {
    protected:
      typedef
      typename _Alloc::template rebind<
      left_child_next_sibling_heap_node_<Value_Type, Node_Metadata,
					 _Alloc> >::other
      node_allocator;

      typedef typename node_allocator::value_type     	node;
      typedef typename node_allocator::pointer 		node_pointer;
      typedef typename node_allocator::const_pointer	node_const_pointer;
      typedef Node_Metadata node_metadata;
      typedef std::pair< node_pointer, node_pointer> 	node_pointer_pair;

    private:
      typedef cond_dealtor< node, _Alloc> 		cond_dealtor_t;

      enum
	{
	  simple_value = is_simple<Value_Type>::value
	};

      typedef integral_constant<int, simple_value> 	no_throw_copies_t;
      typedef typename _Alloc::template rebind<Value_Type>	__rebind_v;

    public:
      typedef typename _Alloc::size_type 		size_type;
      typedef typename _Alloc::difference_type 	difference_type;
      typedef Val