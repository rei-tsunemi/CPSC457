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
 * @file list_update_map_/lu_map_.hpp
 * Contains a list update map.
 */

#include <utility>
#include <iterator>
#include <ext/pb_ds/detail/cond_dealtor.hpp>
#include <ext/pb_ds/tag_and_trait.hpp>
#include <ext/pb_ds/detail/types_traits.hpp>
#include <ext/pb_ds/detail/list_update_map_/entry_metadata_base.hpp>
#include <ext/pb_ds/exception.hpp>
#ifdef _GLIBCXX_DEBUG
#include <ext/pb_ds/detail/debug_map_base.hpp>
#endif
#ifdef PB_DS_LU_MAP_TRACE_
#include <iostream>
#endif
#include <debug/debug.h>

namespace __gnu_pbds
{
  namespace detail
  {
#ifdef PB_DS_DATA_TRUE_INDICATOR
#define PB_DS_LU_NAME lu_map
#endif

#ifdef PB_DS_DATA_FALSE_INDICATOR
#define PB_DS_LU_NAME lu_set
#endif

#define PB_DS_CLASS_T_DEC \
    template<typename Key, typename Mapped, typename Eq_Fn, \
	     typename _Alloc, typename Update_Policy>

#define PB_DS_CLASS_C_DEC \
    PB_DS_LU_NAME<Key, Mapped, Eq_Fn, _Alloc, Update_Policy>

#define PB_DS_LU_TRAITS_BASE \
    types_traits<Key, Mapped, _Alloc, false>

#ifdef _GLIBCXX_DEBUG
#define PB_DS_DEBUG_MAP_BASE_C_DEC \
    debug_map_base<Key, Eq_Fn, \
	      typename _Alloc::template rebind<Key>::other::const_reference>
#endif

    /// list-based (with updates) associative container.
    /// Skip to the lu, my darling.
    template<typename Key,
	     typename Mapped,
	     typename Eq_Fn,
	     typename _Alloc,
	     typename Update_Policy>
    class PB_DS_LU_NAME :
#ifdef _GLIBCXX_DEBUG
      protected PB_DS_DEBUG_MAP_BASE_C_DEC,
#endif
      public PB_DS_LU_TRAITS_BASE
    {
    private:
      typedef PB_DS_LU_TRAITS_BASE 	       	traits_base;

      struct entry
     : public lu_map_entry_metadata_base<typename Update_Policy::metadata_type>
      {
	typename traits_base::value_type m_value;
	typename _Alloc::template rebind<entry>::other::pointer m_p_next;
      };

      typedef typename _Alloc::template rebind<entry>::other entry_allocator;
      typedef typename entry_allocator::pointer entry_pointer;
      typedef typename entry_allocator::const_pointer const_entry_pointer;
      typedef typename entry_allocator::reference entry_reference;
      typedef typename entry_allocator::const_reference const_entry_reference;

      typedef typename _Alloc::template rebind<entry_pointer>::other entry_pointer_allocator;
      typedef typename entry_pointer_allocator::pointer entry_pointer_array;

      typedef typename traits_base::value_type value_type_;
      typedef typename traits_base::pointer pointer_;
      typede