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
 * @file rb_tree_map_/rb_tree_.hpp
 * Contains an implementation for Red Black trees.
 */

#include <ext/pb_ds/detail/standard_policies.hpp>
#include <utility>
#include <vector>
#include <assert.h>
#include <debug/debug.h>

namespace __gnu_pbds
{
  namespace detail
  {
#define PB_DS_CLASS_T_DEC \
    template<typename Key, typename Mapped, typename Cmp_Fn, \
	     typename Node_And_It_Traits, typename _Alloc>

#ifdef PB_DS_DATA_TRUE_INDICATOR
# define PB_DS_RB_TREE_NAME rb_tree_map
# define PB_DS_RB_TREE_BASE_NAME bin_search_tree_map
#endif

#ifdef PB_DS_DATA_FALSE_INDICATOR
# define PB_DS_RB_TREE_NAME rb_tree_set
# define PB_DS_RB_TREE_BASE_NAME bin_search_tree_set
#endif

#define PB_DS_CLASS_C_DEC \
    PB_DS_RB_TREE_NAME<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>

#define PB_DS_RB_TREE_BASE \
    PB_DS_RB_TREE_BASE_NAME<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>


    /**
     *  @brief Red-Black tree.
     *  @ingroup branch-detail
     *
     *  This implementation uses an idea from the SGI STL (using a
     *  @a header node which is needed for efficient iteration).
     */
    template<typename Key,
	     typename Mapped,
	     typename Cmp_Fn,
	     typename Node_And_It_Traits,
	     typename _Alloc>
    class PB_DS_RB_TREE_NAME : public PB_DS_RB_TREE_BASE
    {
    private:
      typedef PB_DS_RB_TREE_BASE 		       	 base_type;
      typedef typename base_type::node_pointer 		 node_pointer;

    public:
      typedef rb_tree_tag 				 container_category;
      typedef Cmp_Fn 					 cmp_fn;
      typedef _Alloc 					 allocator_type;
      typedef typename _Alloc::size_type 		 size_type;
      typedef typename _Alloc::difference_type 		 difference_type;
      typedef typename base_type::key_type 		 key_type;
      typedef typename base_type::key_pointer 		 key_pointer;
      typedef typename base_type::key_const_pointer 	 key_const_pointer;
      typedef typename base_type::key_reference 	 key_reference;
      typedef typename base_type::key_const_reference 	 key_const_reference;
      typedef typename base_type::mapped_type 		 mapped_type;
      typedef typename base_type::mapped_pointer 	 mapped_pointer;
      typedef typename base_type::mapped_const_pointer 	 mapped_const_pointer;
      typedef typename base_type::mapped_reference 	 mapped_reference;
      typedef typename base_type::mapped_const_reference mapped_const_reference;
      typedef typename base_type::value_type 		 value_type;
      typedef typename base_