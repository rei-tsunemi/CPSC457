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
 * @file ov_tree_map_/node_iterators.hpp
 * Contains an implementation class for ov_tree_.
 */

#ifndef PB_DS_OV_TREE_NODE_ITERATORS_HPP
#define PB_DS_OV_TREE_NODE_ITERATORS_HPP

#include <ext/pb_ds/tag_and_trait.hpp>
#include <ext/pb_ds/detail/type_utils.hpp>
#include <debug/debug.h>

namespace __gnu_pbds
{
  namespace detail
  {
#define PB_DS_OV_TREE_CONST_NODE_ITERATOR_C_DEC	\
    ov_tree_node_const_it_<Value_Type, Metadata_Type, _Alloc>

    /// Const node reference.
    template<typename Value_Type, typename Metadata_Type, typename _Alloc>
    class ov_tree_node_const_it_
    {

    protected:
      typedef
      typename _Alloc::template rebind<
      Value_Type>::other::pointer
      pointer;

      typedef
      typename _Alloc::template rebind<
	Value_Type>::other::const_pointer
      const_pointer;

      typedef
      typename _Alloc::template rebind<
	Metadata_Type>::other::const_pointer
      const_metadata_pointer;

      typedef PB_DS_OV_TREE_CONST_NODE_ITERATOR_C_DEC this_type;

    protected:

      template<typename Ptr>
      inline static Ptr
      mid_pointer(Ptr p_begin, Ptr p_end)
      {
	_GLIBCXX_DEBUG_ASSERT(p_end >= p_begin);
	return (p_begin + (p_end - p_begin) / 2);
      }

    public:

      typedef trivial_iterator_tag iterator_category;

      typedef trivial_iterator_difference_type difference_type;

      typedef
      typename _Alloc::template rebind<
	Value_Type>::other::const_pointer
      value_type;

      typedef
      typename _Alloc::template rebind<
	typename remove_const<
	Value_Type>::type>::other::const_pointer
      reference;

      typedef
      typename _Alloc::template rebind<
	typename remove_const<
	Value_Type>::type>::other::const_pointer
      const_reference;

      typedef Metadata_Type metadata_type;

      typedef
      typename _Alloc::template rebind<
	metadata_type>::other::const_reference
      metadata_const_reference;

    public:
      inline
      ov_tree_node_const_it_(const_pointer p_nd = 0,  const_pointer p_begin_nd = 0,  const_pointer p_end_nd = 0,  const_metadata_pointer p_metadata = 0) : m_p_value(const_cast<pointer>(p_nd)), m_p_begin_value(const_cast<pointer>(p_begin_nd)), m_p_end_value(const_cast<pointer>(p_end_nd)), m_p_metadata(p_metadata)
      { }

      inline const_reference
      operator*() const
      { return m_p_value; }

      inline metadata_const_reference
      get_metadata() const
      {
	enum
	  {
	    has_metadata = !is_same<Metadata_Type, nul