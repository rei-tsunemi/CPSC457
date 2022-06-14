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
 * @file cc_hash_table_map_/cc_ht_map_.hpp
 * Contains an implementation class for cc_ht_map_.
 */

#include <utility>
#include <iterator>
#include <ext/pb_ds/detail/cond_dealtor.hpp>
#include <ext/pb_ds/tag_and_trait.hpp>
#include <ext/pb_ds/detail/hash_fn/ranged_hash_fn.hpp>
#include <ext/pb_ds/detail/types_traits.hpp>
#include <ext/pb_ds/exception.hpp>
#include <ext/pb_ds/detail/eq_fn/hash_eq_fn.hpp>
#ifdef _GLIBCXX_DEBUG
#include <ext/pb_ds/detail/debug_map_base.hpp>
#endif
#ifdef PB_DS_HT_MAP_TRACE_
#include <iostream>
#endif
#include <debug/debug.h>

namespace __gnu_pbds
{
  namespace detail
  {
#ifdef PB_DS_DATA_TRUE_INDICATOR
#define PB_DS_CC_HASH_NAME cc_ht_map
#endif

#ifdef PB_DS_DATA_FALSE_INDICATOR
#define PB_DS_CC_HASH_NAME cc_ht_set
#endif

#define PB_DS_CLASS_T_DEC \
    template<typename Key, typename Mapped, typename Hash_Fn, \
	     typename Eq_Fn, typename _Alloc, bool Store_Hash, \
	     typename Comb_Hash_Fn, typename Resize_Policy>

#define PB_DS_CLASS_C_DEC \
    PB_DS_CC_HASH_NAME<Key, Mapped, Hash_Fn, Eq_Fn, _Alloc,	\
		     Store_Hash, Comb_Hash_Fn, Resize_Policy>

#define PB_DS_HASH_EQ_FN_C_DEC \
    hash_eq_fn<Key, Eq_Fn, _Alloc, Store_Hash>

#define PB_DS_RANGED_HASH_FN_C_DEC \
    ranged_hash_fn<Key,	Hash_Fn, _Alloc, Comb_Hash_Fn, Store_Hash>

#define PB_DS_CC_HASH_TRAITS_BASE \
    types_traits<Key, Mapped, _Alloc, Store_Hash>

#ifdef _GLIBCXX_DEBUG
#define PB_DS_DEBUG_MAP_BASE_C_DEC \
    debug_map_base<Key,	Eq_Fn, \
		  typename _Alloc::template rebind<Key>::other::const_reference>
#endif


    /**
     *  A collision-chaining hash-based container.
     *
     *
     *  @ingroup hash-detail
     *
     *  @tparam Key 	    	Key type.
     *
     *  @tparam Mapped 	    	Map type.
     *
     *  @tparam Hash_Fn	      	Hashing functor.
     *                          Default is __gnu_cxx::hash.
     *
     *  @tparam Eq_Fn	      	Equal functor.
     *                          Default std::equal_to<Key>
     *
     *  @tparam _Alloc 	    	Allocator type.
     *
     *  @tparam Store_Hash    	If key type stores extra metadata.
     *                          Defaults to false.
     *
     *  @tparam Comb_Hash_Fn	Combining hash functor.
     *                          If Hash_Fn is not null_type, then this
     *                          is the ranged-hash functor; otherwise,
     *                          this is the range-hashing functor.
     *                    XXX(See Design::Hash-Based Containers::Hash