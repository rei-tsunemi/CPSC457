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
 * @file ov_tree_map_/ov_tree_map_.hpp
 * Contains an implementation class for ov_tree.
 */

#include <map>
#include <set>
#include <ext/pb_ds/exception.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include <ext/pb_ds/detail/eq_fn/eq_by_less.hpp>
#include <ext/pb_ds/detail/types_traits.hpp>
#include <ext/pb_ds/detail/type_utils.hpp>
#include <ext/pb_ds/detail/tree_trace_base.hpp>
#ifdef _GLIBCXX_DEBUG
#include <ext/pb_ds/detail/debug_map_base.hpp>
#endif
#include <utility>
#include <functional>
#include <algorithm>
#include <vector>
#include <assert.h>
#include <debug/debug.h>

namespace __gnu_pbds
{
  namespace detail
  {
#ifdef PB_DS_DATA_TRUE_INDICATOR
#define PB_DS_OV_TREE_NAME ov_tree_map
#define PB_DS_CONST_NODE_ITERATOR_NAME ov_tree_node_const_iterator_map
#endif

#ifdef PB_DS_DATA_FALSE_INDICATOR
#define PB_DS_OV_TREE_NAME ov_tree_set
#define PB_DS_CONST_NODE_ITERATOR_NAME ov_tree_node_const_iterator_set
#endif

#define PB_DS_CLASS_T_DEC \
    template<typename Key, typename Mapped, typename Cmp_Fn, \
	     typename Node_And_It_Traits, typename _Alloc>

#define PB_DS_CLASS_C_DEC \
   PB_DS_OV_TREE_NAME<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>

#define PB_DS_OV_TREE_TRAITS_BASE \
    types_traits<Key, Mapped, _Alloc, false>

#ifdef _GLIBCXX_DEBUG
#define PB_DS_DEBUG_MAP_BASE_C_DEC \
    debug_map_base<Key, eq_by_less<Key, Cmp_Fn>, \
       	typename _Alloc::template rebind<Key>::other::const_reference>
#endif

#ifdef PB_DS_TREE_TRACE
#define PB_DS_TREE_TRACE_BASE_C_DEC \
    tree_trace_base<typename Node_And_It_Traits::node_const_iterator,	\
		    typename Node_And_It_Traits::node_iterator,		\
		    Cmp_Fn, false, _Alloc>
#endif

#ifndef PB_DS_CHECK_KEY_EXISTS
#  error Missing definition
#endif

    /**
     *  @brief Ordered-vector tree associative-container.
     *  @ingroup branch-detail
     */
    template<typename Key, typename Mapped, typename Cmp_Fn,
	     typename Node_And_It_Traits, typename _Alloc>
    class PB_DS_OV_TREE_NAME :
#ifdef _GLIBCXX_DEBUG
      protected PB_DS_DEBUG_MAP_BASE_C_DEC,
#endif
#ifdef PB_DS_TREE_TRACE
      public PB_DS_TREE_TRACE_BASE_C_DEC,
#endif
      public Cmp_Fn,
      public Node_And_It_Traits::node_update,
      public PB_DS_OV_TREE_TRAITS_BASE
    {
    private:
      typedef PB_DS_OV_TREE_TRAITS_BASE	       		traits_base;
      typedef Node_And_It_Traits			traits_type;

      typedef typename remove_const<typename traits_base::value_type>::type non_const_value_type;

      typedef