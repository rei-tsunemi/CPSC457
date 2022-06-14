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
 * @file binary_heap_/binary_heap_.hpp
 * Contains an implementation class for a binary heap.
 */

#ifndef PB_DS_BINARY_HEAP_HPP
#define PB_DS_BINARY_HEAP_HPP

#include <queue>
#include <algorithm>
#include <ext/pb_ds/detail/cond_dealtor.hpp>
#include <ext/pb_ds/detail/cond_dealtor.hpp>
#include <ext/pb_ds/detail/type_utils.hpp>
#include <ext/pb_ds/detail/binary_heap_/entry_cmp.hpp>
#include <ext/pb_ds/detail/binary_heap_/entry_pred.hpp>
#include <ext/pb_ds/detail/binary_heap_/resize_policy.hpp>
#include <ext/pb_ds/detail/binary_heap_/point_const_iterator.hpp>
#include <ext/pb_ds/detail/binary_heap_/const_iterator.hpp>
#ifdef PB_DS_BINARY_HEAP_TRACE_
#include <iostream>
#endif
#include <ext/pb_ds/detail/type_utils.hpp>
#include <debug/debug.h>

namespace __gnu_pbds
{
  namespace detail
  {
#define PB_DS_CLASS_T_DEC \
    template<typename Value_Type, typename Cmp_Fn, typename _Alloc>

#define PB_DS_CLASS_C_DEC \
    binary_heap<Value_Type, Cmp_Fn, _Alloc>

#define PB_DS_ENTRY_CMP_DEC \
    entry_cmp<Value_Type, Cmp_Fn, _Alloc, is_simple<Value_Type>::value>::type

#define PB_DS_RESIZE_POLICY_DEC	\
    __gnu_pbds::detail::resize_policy<typename _Alloc::size_type>

    /**
     *  Binary heaps composed of resize and compare policies.
     *
     *  @ingroup heap-detail
     *
     *  Based on CLRS.
     */
    template<typename Value_Type, typename Cmp_Fn, typename _Alloc>
    class binary_heap
    : public PB_DS_ENTRY_CMP_DEC, public PB_DS_RESIZE_POLICY_DEC
    {
    public:
      typedef Value_Type 				value_type;
      typedef Cmp_Fn 					cmp_fn;
      typedef _Alloc 					allocator_type;
      typedef typename _Alloc::size_type 		size_type;
      typedef typename _Alloc::difference_type 		difference_type;
      typedef typename PB_DS_ENTRY_CMP_DEC 		entry_cmp;
      typedef PB_DS_RESIZE_POLICY_DEC 			resize_policy;
      typedef cond_dealtor<value_type, _Alloc> 		cond_dealtor_t;

    private:
      enum
	{
	  simple_value = is_simple<value_type>::value
	};

      typedef integral_constant<int, simple_value> 	no_throw_copies_t;

      typedef typename _Alloc::template rebind<value_type>	__rebind_v;
      typedef typename __rebind_v::other 		value_allocator;

    public:
      typedef typename value_allocator::pointer		pointer;
      typedef typename value_allocator::const_pointer	const_pointer;
      typedef typename value_allocator::reference	reference;
      typedef typename value_allocator::const_reference	const_reference;

      type