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
 * @file pat_trie_/pat_trie_base.hpp
 * Contains the base class for a patricia tree.
 */

#ifndef PB_DS_PAT_TRIE_BASE
#define PB_DS_PAT_TRIE_BASE

#include <debug/debug.h>

namespace __gnu_pbds
{
  namespace detail
  {
    /// Base type for PATRICIA trees.
    struct pat_trie_base
    {
      /**
       *  @brief  Three types of nodes.
       *
       *  i_node is used by _Inode, leaf_node by _Leaf, and head_node by _Head.
       */
      enum node_type
	{
	  i_node,
	  leaf_node,
	  head_node
	};

      /// Metadata base primary template.
      template<typename Metadata, typename _Alloc>
	struct _Metadata
	{
	  typedef Metadata     					metadata_type;
	  typedef _Alloc     					allocator_type;
	  typedef typename _Alloc::template rebind<Metadata>	__rebind_m;
	  typedef typename __rebind_m::other::const_reference  const_reference;

	  const_reference
	  get_metadata() const
	  { return m_metadata; }

	  metadata_type 					m_metadata;
	};

      /// Specialization for null metadata.
      template<typename _Alloc>
	struct _Metadata<null_type, _Alloc>
	{
	  typedef null_type 					metadata_type;
	  typedef _Alloc     					allocator_type;
	};


      /// Node base.
      template<typename _ATraits, typename Metadata>
      struct _Node_base
      : public Metadata
      {
      private:
	typedef typename Metadata::allocator_type		_Alloc;

      public:
	typedef _Alloc						allocator_type;
	typedef _ATraits					access_traits;
	typedef typename _ATraits::type_traits			type_traits;
	typedef typename _Alloc::template rebind<_Node_base>	__rebind_n;
	typedef typename __rebind_n::other::pointer 		node_pointer;

	node_pointer 						m_p_parent;
	const node_type 	       				m_type;

	_Node_base(node_type type) : m_type(type)
	{ }

	typedef typename _Alloc::template rebind<_ATraits>    __rebind_at;
	typedef typename __rebind_at::other::const_pointer    a_const_pointer;
	typedef typename _ATraits::const_iterator	      a_const_iterator;

#ifdef _GLIBCXX_DEBUG
	typedef std::pair<a_const_iterator, a_const_iterator> node_debug_info;

	void
	assert_valid(a_const_pointer p_traits,
		     const char* __file, int __line) const
	{ assert_valid_imp(p_traits, __file, __line); }

	virtual node_debug_info
	assert_valid_imp(a_const_pointer, const char*, int) const = 0;
#endif
      };


    /// Head node for PATRICIA tree.
    template<typename _ATraits, typename Metadata>
    struct _Head
    : public _Node_base<_ATraits, Metadata>
    {
      typedef _Node_base<_