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
 * @file priority_queue.hpp
 * Contains priority_queues.
 */

#ifndef PB_DS_PRIORITY_QUEUE_HPP
#define PB_DS_PRIORITY_QUEUE_HPP

#include <bits/c++config.h>
#include <ext/pb_ds/tag_and_trait.hpp>
#include <ext/pb_ds/detail/priority_queue_base_dispatch.hpp>
#include <ext/pb_ds/detail/standard_policies.hpp>

namespace __gnu_pbds
{
  /**
   *  @defgroup heap-based Heap-Based
   *  @ingroup containers-pbds
   *  @{
   */

  /**
   *  @defgroup heap-detail Base and Policy Classes
   *  @ingroup heap-based
   */

  /**
   *  A priority queue composed of one specific heap policy.
   *
   *  @tparam _Tv 	    	Value type.
   *  @tparam Cmp_Fn	    	Comparison functor.
   *  @tparam Tag 	    	Instantiating data structure type,
   *			    	see container_tag.
   *  @tparam _Alloc 	    	Allocator type.
   *
   *  Base is dispatched at compile time via Tag, from the following
   *  choices: binary_heap_tag, binomial_heap_tag, pairing_heap_tag,
   *           rc_binomial_heap_tag, thin_heap_tag
   *
   *  Base choices are: detail::binary_heap, detail::binomial_heap,
   *                    detail::pairing_heap, detail::rc_binomial_heap,
   *                    detail::thin_heap.
   */
   template<typename _Tv,
	   typename Cmp_Fn = std::less<_Tv>,
	   typename Tag = pairing_heap_tag,
	   typename _Alloc = std::allocator<char> >
  class priority_queue
  : public detail::container_base_dispatch<_Tv, Cmp_Fn, _Alloc, Tag>::type
  {
  public:
    typedef _Tv 					value_type;
    typedef Cmp_Fn 					cmp_fn;
    typedef Tag 					container_category;
    typedef _Alloc 					allocator_type;
    typedef typename allocator_type::size_type 		size_type;
    typedef typename allocator_type::difference_type 	difference_type;

  private:
    typedef typename detail::container_base_dispatch<_Tv, Cmp_Fn, _Alloc,
						     Tag>::type
 							base_type;
    typedef typename _Alloc::template rebind<_Tv>   	__rebind_v;
    typedef typename __rebind_v::other			__rebind_va;

 public:
    typedef typename __rebind_va::reference 		reference;
    typedef typename __rebind_va::const_reference 	const_reference;
    typedef typename __rebind_va::pointer 	   	pointer;
    typedef typename __rebind_va::const_pointer 	const_pointer;

    typedef typename base_type::point_iterator 		point_iterator;
    typedef typename base_type::point_const_iterator 	point_const_iterator;
    typedef typename base_type::iterator 		iterator;
    typedef typename base_type::const_iterator 		const_iterat