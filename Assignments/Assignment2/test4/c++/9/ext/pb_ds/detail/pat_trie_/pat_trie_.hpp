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
 * @file pat_trie_/pat_trie_.hpp
 * Contains an implementation class for a patricia tree.
 */

#include <iterator>
#include <utility>
#include <algorithm>
#include <functional>
#include <assert.h>
#include <list>
#include <ext/pb_ds/exception.hpp>
#include <ext/pb_ds/tag_and_trait.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include <ext/pb_ds/detail/cond_dealtor.hpp>
#include <ext/pb_ds/detail/type_utils.hpp>
#include <ext/pb_ds/detail/types_traits.hpp>
#include <ext/pb_ds/detail/eq_fn/eq_by_less.hpp>
#include <ext/pb_ds/detail/pat_trie_/synth_access_traits.hpp>
#include <ext/pb_ds/detail/pat_trie_/pat_trie_base.hpp>
#ifdef _GLIBCXX_DEBUG
#include <ext/pb_ds/detail/debug_map_base.hpp>
#endif
#include <debug/debug.h>

namespace __gnu_pbds
{
  namespace detail
  {
#ifdef PB_DS_DATA_TRUE_INDICATOR
#define PB_DS_PAT_TRIE_NAME pat_trie_map
#endif

#ifdef PB_DS_DATA_FALSE_INDICATOR
#define PB_DS_PAT_TRIE_NAME pat_trie_set
#endif

#define PB_DS_CLASS_T_DEC \
    template<typename Key, typename Mapped, typename Node_And_It_Traits, \
	     typename _Alloc>

#define PB_DS_CLASS_C_DEC \
    PB_DS_PAT_TRIE_NAME<Key, Mapped, Node_And_It_Traits, _Alloc>

#define PB_DS_PAT_TRIE_TRAITS_BASE \
    types_traits<Key, Mapped, _Alloc, false>

#ifdef _GLIBCXX_DEBUG
#define PB_DS_DEBUG_MAP_BASE_C_DEC \
    debug_map_base<Key,	eq_by_less<Key, std::less<Key> >, \
		 typename _Alloc::template rebind<Key>::other::const_reference>
#endif


    /**
     *  @brief PATRICIA trie.
     *  @ingroup branch-detail
     *
     *  This implementation loosely borrows ideas from:
     *  1) Fast Mergeable Integer Maps, Okasaki, Gill 1998
     *  2) Ptset: Sets of integers implemented as Patricia trees,
     *     Jean-Christophe Filliatr, 2000
     */
    template<typename Key, typename Mapped, typename Node_And_It_Traits,
	     typename _Alloc>
    class PB_DS_PAT_TRIE_NAME :
#ifdef _GLIBCXX_DEBUG
      public PB_DS_DEBUG_MAP_BASE_C_DEC,
#endif
      public Node_And_It_Traits::synth_access_traits,
      public Node_And_It_Traits::node_update,
      public PB_DS_PAT_TRIE_TRAITS_BASE,
      public pat_trie_base
    {
    private:
      typedef pat_trie_base				base_type;
      typedef PB_DS_PAT_TRIE_TRAITS_BASE 	       	traits_base;
      typedef Node_And_It_Traits			traits_type;

      typedef typename traits_type::synth_access_traits synth_access_traits;
      typedef typename synth_access_traits::const_iterator a_const_iterator;

      typedef typename traits_type::node 		n