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
 * @file pat_trie_/split_fn_imps.hpp
 * Contains an implementation class for pat_trie.
 */

PB_DS_CLASS_T_DEC
void
PB_DS_CLASS_C_DEC::
split(key_const_reference r_key, PB_DS_CLASS_C_DEC& other)
{
  PB_DS_ASSERT_VALID((*this))
  PB_DS_ASSERT_VALID(other)
  branch_bag bag;
  leaf_pointer p_split_lf = split_prep(r_key, other, bag);
  if (p_split_lf == 0)
    {
      _GLIBCXX_DEBUG_ASSERT(bag.empty());
      PB_DS_ASSERT_VALID((*this))
      PB_DS_ASSERT_VALID(other)
      return;
    }

  _GLIBCXX_DEBUG_ASSERT(!bag.empty());
  other.clear();

  m_p_head->m_p_parent = rec_split(m_p_head->m_p_parent, pref_begin(p_split_lf),
				   pref_end(p_split_lf), other, bag);

  m_p_head->m_p_parent->m_p_parent = m_p_head;

  head_pointer __ohead = other.m_p_head;
  __ohead->m_p_max = m_p_head->m_p_max;
  m_p_head->m_p_max = rightmost_descendant(m_p_head->m_p_parent);
  __ohead->m_p_min = other.leftmost_descendant(__ohead->m_p_parent);

  other.m_size = std::distance(other.PB_DS_CLASS_C_DEC::begin(),
			       other.PB_DS_CLASS_C_DEC::end());
  m_size -= other.m_size;
  PB_DS_ASSERT_VALID((*this))
  PB_DS_ASSERT_VALID(other)
}

PB_DS_CLASS_T_DEC
typename PB_DS_CLASS_C_DEC::leaf_pointer
PB_DS_CLASS_C_DEC::
split_prep(key_const_reference r_key, PB_DS_CLASS_C_DEC& other,
	   branch_bag& r_bag)
{
  _GLIBCXX_DEBUG_ASSERT(r_bag.empty());
  if (m_size == 0)
    {
      other.clear();
      PB_DS_ASSERT_VALID((*this))
      PB_DS_ASSERT_VALID(other)
      return 0;
    }

  if (synth_access_traits::cmp_keys(r_key,
				    PB_DS_V2F(static_cast<leaf_const_pointer>(m_p_head->m_p_min)->value())))
    {
      other.clear();
      value_swap(other);
      PB_DS_ASSERT_VALID((*this))
      PB_DS_ASSERT_VALID(other)
      return 0;
    }

  if (!synth_access_traits::cmp_keys(r_key,
				       PB_DS_V2F(static_cast<leaf_const_pointer>(m_p_head->m_p_max)->value())))
    {
      PB_DS_ASSERT_VALID((*this))
      PB_DS_ASSERT_VALID(other)
      return 0;
    }

  iterator it = lower_bound(r_key);

  if (!synth_access_traits::equal_keys(PB_DS_V2F(*it), r_key))
    --it;

  node_pointer p_nd = it.m_p_nd;
  _GLIBCXX_DEBUG_ASSERT(p_nd->m_type == leaf_node);
  leaf_pointer p_ret_l = static_cast<leaf_pointer>(p_nd);
  while (p_nd->m_type != head_node)
    {
      r_bag.add_branch();
      p_nd = p_nd->m_p_parent;
    }
  _GLIBCXX_DEBUG_ONLY(debug_base::split(r_key,(synth_access_traits&)(*this), other);)

  return p_ret_l;
}

PB_DS_CLASS_T_DEC
typename PB_DS_CLASS_C_DEC::node_p