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
 * @file splay_tree_/splay_tree_.hpp
 * Contains an implementation class for splay trees.
 */
/*
 * This implementation uses an idea from the SGI STL (using a @a header node
 *    which is needed for efficient iteration). Following is the SGI STL
 *    copyright.
 *
 * Copyright (c) 1996,1997
 * Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.    Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.    It is provided "as is" without express or implied warranty.
 *
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.    Hewlett-Packard Company makes no
 * representations about the suitability of this software for any
 * purpose.    It is provided "as is" without express or implied warranty.
 *
 *
 */

#include <utility>
#include <vector>
#include <assert.h>
#include <debug/debug.h>

namespace __gnu_pbds
{
  namespace detail
  {
#ifdef PB_DS_DATA_TRUE_INDICATOR
# define PB_DS_S_TREE_NAME splay_tree_map
# define PB_DS_S_TREE_BASE_NAME bin_search_tree_map
#endif

#ifdef PB_DS_DATA_FALSE_INDICATOR
# define PB_DS_S_TREE_NAME splay_tree_set
# define PB_DS_S_TREE_BASE_NAME bin_search_tree_set
#endif

#define PB_DS_CLASS_T_DEC \
    template<typename Key, typename Mapped, typename Cmp_Fn, \
	     typename Node_And_It_Traits, typename _Alloc>

#define PB_DS_CLASS_C_DEC \
    PB_DS_S_TREE_NAME<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>

#define PB_DS_S_TREE_BASE \
    PB_DS_S_TREE_BASE_NAME<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>


    /**
     *  @brief Splay tree.
     *  @ingroup branch-detail
     */
    template<typename Key, typename Mapped, typename Cmp_Fn,
	     typename Node_And_It_Traits, typename _Alloc>
    class PB_DS_S_TREE_NAME : public PB_DS_S_TREE_BASE
    {
    private:
      typedef PB_DS_S_TREE_BASE 		       	 base_type;
#ifdef _GLIBCXX_DEBUG
      typedef