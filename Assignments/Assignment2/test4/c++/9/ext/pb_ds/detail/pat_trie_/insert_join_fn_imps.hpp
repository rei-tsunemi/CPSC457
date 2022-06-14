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
 * @file pat_trie_/insert_join_fn_imps.hpp
 * Contains an implementation class for pat_trie.
 */

PB_DS_CLASS_T_DEC
void
PB_DS_CLASS_C_DEC::
join(PB_DS_CLASS_C_DEC& other)
{
  PB_DS_ASSERT_VALID((*this))
  PB_DS_ASSERT_VALID(other)
  branch_bag bag;
  if (!join_prep(other, bag))
    {
      PB_DS_ASSERT_VALID((*this))
      PB_DS_ASSERT_VALID(other)
      return;
    }

  m_p_head->m_p_parent = rec_join(m_p_head->m_p_parent,
				  other.m_p_head->m_p_parent, 0, bag);

  m_p_head->m_p_parent->m_p_parent = m_p_head;
  m_size += other.m_size;
  other.initialize();
  PB_DS_ASSERT_VALID(other)
  m_p_head->m_p_min = leftmost_descendant(m_p_head->m_p_parent);
  m_p_head->m_p_max = rightmost_descendant(m_p_head->m_p_parent);
  PB_DS_ASSERT_VALID((*this))
}

PB_DS_CLASS_T_DEC
bool
PB_DS_CLASS_C_DEC::
join_prep(PB_DS_CLASS_C_DEC& other, branch_bag& r_bag)
{
  PB_DS_ASSERT_VALID((*this))
  PB_DS_ASSERT_VALID(other)
  if (other.m_size == 0)
    return false;

  if (m_size == 0)
    {
      value_swap(other);
      return false;
    }

  const bool greater =
    synth_access_traits::cmp_keys(PB_DS_V2F(static_cast<leaf_const_pointer>(m_p_head->m_p_max)->value()),
				    PB_DS_V2F(static_cast<leaf_const_pointer>(other.m_p_head->m_p_min)->value()));

  const bool lesser =
    synth_access_traits::cmp_keys(PB_DS_V2F(static_cast<leaf_const_pointer>(other.m_p_head->m_p_max)->value()),
				    PB_DS_V2F(static_cast<leaf_const_pointer>(m_p_head->m_p_min)->value()));

  if (!greater && !lesser)
    __throw_join_error();

  rec_join_prep(m_p_head->m_p_parent, other.m_p_head->m_p_parent, r_bag);
  _GLIBCXX_DEBUG_ONLY(debug_base::join(other, false);)
  return true;
}

PB_DS_CLASS_T_DEC
void
PB_DS_CLASS_C_DEC::
rec_join_prep(node_const_pointer p_l, node_const_pointer p_r, 
	      branch_bag& r_bag)
{
  if (p_l->m_type == leaf_node)
    {
      if (p_r->m_type == leaf_node)
	{
	  rec_join_prep(static_cast<leaf_const_pointer>(p_l),
			static_cast<leaf_const_pointer>(p_r), r_bag);
	  return;
	}

      _GLIBCXX_DEBUG_ASSERT(p_r->m_type == i_node);
      rec_join_prep(static_cast<leaf_const_pointer>(p_l),
		    static_cast<inode_const_pointer>(p_r), r_bag);
      return;
    }

  _GLIBCXX_DEBUG_ASSERT(p_l->m_type == i_node);
  if (p_r->m_type == leaf_node)
    {
      rec_join_prep(static_cast<inode_const_pointer>(p_l),
		    static_cast<leaf_const_pointer>(p_r), r_bag);
      return;
    }

  _GLIBCXX_DEBUG_ASSERT(p_r->m_type == i_node);

  rec_join_prep(static_cas