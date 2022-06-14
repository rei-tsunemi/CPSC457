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
 * @file pat_trie_/erase_fn_imps.hpp
 * Contains an implementation class for pat_trie.
 */

PB_DS_CLASS_T_DEC
inline bool
PB_DS_CLASS_C_DEC::
erase(key_const_reference r_key)
{
  node_pointer p_nd = find_imp(r_key);
  if (p_nd == 0 || p_nd->m_type == i_node)
    {
      PB_DS_CHECK_KEY_DOES_NOT_EXIST(r_key)
      return false;
    }

  _GLIBCXX_DEBUG_ASSERT(p_nd->m_type == leaf_node);
  if (!synth_access_traits::equal_keys(PB_DS_V2F(reinterpret_cast<leaf_pointer>(p_nd)->value()), r_key))
    {
      PB_DS_CHECK_KEY_DOES_NOT_EXIST(r_key)
      return false;
    }

  PB_DS_CHECK_KEY_EXISTS(r_key)
  erase_leaf(static_cast<leaf_pointer>(p_nd));
  PB_DS_ASSERT_VALID((*this))
  return true;
}

PB_DS_CLASS_T_DEC
void
PB_DS_CLASS_C_DEC::
erase_fixup(inode_pointer p_nd)
{
  _GLIBCXX_DEBUG_ASSERT(std::distance(p_nd->begin(), p_nd->end()) >= 1);
  if (std::distance(p_nd->begin(), p_nd->end()) == 1)
    {
      node_pointer p_parent = p_nd->m_p_parent;
      if (p_parent == m_p_head)
	m_p_head->m_p_parent = *p_nd->begin();
      else
	{
	  _GLIBCXX_DEBUG_ASSERT(p_parent->m_type == i_node);
	  node_pointer p_new_child = *p_nd->begin();

	  typedef inode_pointer inode_ptr;
	  inode_ptr p_internal = static_cast<inode_ptr>(p_parent);
	  p_internal->replace_child(p_new_child, pref_begin(p_new_child),
				    pref_end(p_new_child), this);
	}
      (*p_nd->begin())->m_p_parent = p_nd->m_p_parent;
      p_nd->~inode();
      s_inode_allocator.deallocate(p_nd, 1);

      if (p_parent == m_p_head)
	return;

      _GLIBCXX_DEBUG_ASSERT(p_parent->m_type == i_node);
      p_nd = static_cast<inode_pointer>(p_parent);
    }

  while (true)
    {
      _GLIBCXX_DEBUG_ASSERT(std::distance(p_nd->begin(), p_nd->end()) > 1);
      p_nd->update_prefixes(this);
      apply_update(p_nd, (node_update*)this);
      PB_DS_ASSERT_NODE_VALID(p_nd)
      if (p_nd->m_p_parent->m_type == head_node)
	return;

      _GLIBCXX_DEBUG_ASSERT(p_nd->m_p_parent->m_type == i_node);

      p_nd = static_cast<inode_pointer>(p_nd->m_p_parent);
    }
}

PB_DS_CLASS_T_DEC
inline void
PB_DS_CLASS_C_DEC::
actual_erase_leaf(leaf_pointer p_l)
{
  _GLIBCXX_DEBUG_ASSERT(m_size > 0);
  --m_size;
  _GLIBCXX_DEBUG_ONLY(debug_base::erase_existing(PB_DS_V2F(p_l->value())));
  p_l->~leaf();
  s_leaf_allocator.deallocate(p_l, 1);
}

PB_DS_CLASS_T_DEC
void
PB_DS_CLASS_C_DEC::
clear()
{
  if (!empty())
    {
      clear_imp(m_p_head->m_p_parent);
      m_size = 0;
      initialize();
      _GLIBCXX_DEBUG_ONL