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
 * @file thin_heap_/thin_heap_.hpp
 * Contains an implementation class for a thin heap.
 */

#ifndef PB_DS_THIN_HEAP_HPP
#define PB_DS_THIN_HEAP_HPP

#include <algorithm>
#include <ext/pb_ds/detail/cond_dealtor.hpp>
#include <ext/pb_ds/detail/type_utils.hpp>
#include <ext/pb_ds/detail/left_child_next_sibling_heap_/left_child_next_sibling_heap_.hpp>
#include <debug/debug.h>

namespace __gnu_pbds
{
  namespace detail
  {
#define PB_DS_CLASS_T_DEC \
    template<typename Value_Type, typename Cmp_Fn, typename _Alloc>

#define PB_DS_CLASS_C_DEC \
    thin_heap<Value_Type, Cmp_Fn, _Alloc>

#ifdef _GLIBCXX_DEBUG
#define PB_DS_BASE_T_P \
    <Value_Type, Cmp_Fn, typename _Alloc::size_type, _Alloc, true>
#else
#define PB_DS_BASE_T_P \
    <Value_Type, Cmp_Fn, typename _Alloc::size_type, _Alloc>
#endif


    /**
     *  Thin heap.
     *
     *  @ingroup heap-detail
     *
     *  See Tarjan and Kaplan.
     */
    template<typename Value_Type, typename Cmp_Fn, typename _Alloc>
    class thin_heap
    : public left_child_next_sibling_heap PB_DS_BASE_T_P
    {
    private:
      typedef typename _Alloc::template rebind<Value_Type>::other __rebind_a;
      typedef left_child_next_sibling_heap PB_DS_BASE_T_P base_type;

    protected:
      typedef typename base_type::node 			node;
      typedef typename base_type::node_pointer 		node_pointer;
      typedef typename base_type::node_const_pointer 	node_const_pointer;

    public:
      typedef Value_Type 				value_type;
      typedef Cmp_Fn 					cmp_fn;
      typedef _Alloc 					allocator_type;
      typedef typename _Alloc::size_type 		size_type;
      typedef typename _Alloc::difference_type 		difference_type;

      typedef typename __rebind_a::pointer		pointer;
      typedef typename __rebind_a::const_pointer	const_pointer;
      typedef typename __rebind_a::reference		reference;
      typedef typename __rebind_a::const_reference     	const_reference;

      typedef typename base_type::point_iterator 	point_iterator;
      typedef typename base_type::point_const_iterator 	point_const_iterator;
      typedef typename base_type::iterator 		iterator;
      typedef typename base_type::const_iterator 	const_iterator;


      inline point_iterator
      push(const_reference);

      void
      modify(point_iterator, const_reference);

      inline const_reference
      top() const;

      void
      pop();

      void
      erase(point_iterator);

      inline void
      clear();

      template<typena