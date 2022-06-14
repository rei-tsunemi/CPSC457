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
 * @file rb_tree_map_/split_join_fn_imps.hpp
 * Contains an implementation for rb_tree_.
 */

PB_DS_CLASS_T_DEC
inline void
PB_DS_CLASS_C_DEC::
join(PB_DS_CLASS_C_DEC& other)
{
  PB_DS_ASSERT_VALID((*this))
  PB_DS_ASSERT_VALID(other)
  if (base_type::join_prep(other) == false)
    {
      PB_DS_ASSERT_VALID((*this))
      PB_DS_ASSERT_VALID(other)
      return;
    }

  const node_pointer p_x = other.split_min();
  join_imp(p_x, other.m_p_head->m_p_parent);
  base_type::join_finish(other);
  PB_DS_ASSERT_VALID((*this))
  PB_DS_ASSERT_VALID(other)
 }

PB_DS_CLASS_T_DEC
void
PB_DS_CLASS_C_DEC::
join_imp(node_pointer p_x, node_pointer p_r)
{
  _GLIBCXX_DEBUG_ASSERT(p_x != 0);
  if (p_r != 0)
    p_r->m_red = false;

  const size_type h = black_height(base_type::m_p_head->m_p_parent);
  const size_type other_h = black_height(p_r);
  node_pointer p_x_l;
  node_pointer p_x_r;
  std::pair<node_pointer, node_pointer> join_pos;
  const bool right_join = h >= other_h;
  if (right_join)
    {
      join_pos = find_join_pos_right(base_type::m_p_head->m_p_parent, 
				     h, other_h);
      p_x_l = join_pos.first;
      p_x_r = p_r;
    }
  else
    {
      p_x_l = base_type::m_p_head->m_p_parent;
      base_type::m_p_head->m_p_parent = p_r;
      if (p_r != 0)
	p_r->m_p_parent = base_type::m_p_head;

      join_pos = find_join_pos_left(base_type::m_p_head->m_p_parent, 
				    h, other_h);
      p_x_r = join_pos.first;
    }

  node_pointer p_parent = join_pos.second;
  if (p_parent == base_type::m_p_head)
    {
      base_type::m_p_head->m_p_parent = p_x;
      p_x->m_p_parent = base_type::m_p_head;
    }
  else
    {
      p_x->m_p_parent = p_parent;
      if (right_join)
	p_x->m_p_parent->m_p_right = p_x;
      else
	p_x->m_p_parent->m_p_left = p_x;
    }

  p_x->m_p_left = p_x_l;
  if (p_x_l != 0)
    p_x_l->m_p_parent = p_x;

  p_x->m_p_right = p_x_r;
  if (p_x_r != 0)
    p_x_r->m_p_parent = p_x;

  p_x->m_red = true;

  base_type::initialize_min_max();
  PB_DS_STRUCT_ONLY_ASSERT_VALID((*this))
  base_type::update_to_top(p_x, (node_update* )this);
  insert_fixup(p_x);
  PB_DS_STRUCT_ONLY_ASSERT_VALID((*this))
}

PB_DS_CLASS_T_DEC
inline typename PB_DS_CLASS_C_DEC::node_pointer
PB_DS_CLASS_C_DEC::
split_min()
{
  node_pointer p_min = base_type::m_p_head->m_p_left;

#ifdef _GLIBCXX_DEBUG
  const node_pointer p_head = base_type::m_p_head;
  _GLIBCXX_DEBUG_ASSERT(p_min != p_head);
#endif 

  remove_node(p_min);
  return p_min;
}

PB_DS_CLASS_T_