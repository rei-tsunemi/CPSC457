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
 * @file hash_load_check_resize_trigger_imp.hpp
 * Contains a resize trigger implementation.
 */

#define PB_DS_ASSERT_VALID(X)						\
  _GLIBCXX_DEBUG_ONLY(X.assert_valid(__FILE__, __LINE__);)

PB_DS_CLASS_T_DEC
PB_DS_CLASS_C_DEC::
hash_load_check_resize_trigger(float load_min, float load_max)
: m_load_min(load_min), m_load_max(load_max), m_next_shrink_size(0),
  m_next_grow_size(0), m_resize_needed(false)
{ PB_DS_ASSERT_VALID((*this)) }

PB_DS_CLASS_T_DEC
inline void
PB_DS_CLASS_C_DEC::
notify_find_search_start()
{ PB_DS_ASSERT_VALID((*this)) }

PB_DS_CLASS_T_DEC
inline void
PB_DS_CLASS_C_DEC::
notify_find_search_collision()
{ PB_DS_ASSERT_VALID((*this)) }

PB_DS_CLASS_T_DEC
inline void
PB_DS_CLASS_C_DEC::
notify_find_search_end()
{ PB_DS_ASSERT_VALID((*this)) }

PB_DS_CLASS_T_DEC
inline void
PB_DS_CLASS_C_DEC::
notify_insert_search_start()
{ PB_DS_ASSERT_VALID((*this)) }

PB_DS_CLASS_T_DEC
inline void
PB_DS_CLASS_C_DEC::
notify_insert_search_collision()
{ PB_DS_ASSERT_VALID((*this)) }

PB_DS_CLASS_T_DEC
inline void
PB_DS_CLASS_C_DEC::
notify_insert_search_end()
{ PB_DS_ASSERT_VALID((*this)) }

PB_DS_CLASS_T_DEC
inline void
PB_DS_CLASS_C_DEC::
notify_erase_search_start()
{ PB_DS_ASSERT_VALID((*this)) }

PB_DS_CLASS_T_DEC
inline void
PB_DS_CLASS_C_DEC::
notify_erase_search_collision()
{ PB_DS_ASSERT_VALID((*this)) }

PB_DS_CLASS_T_DEC
inline void
PB_DS_CLASS_C_DEC::
notify_erase_search_end()
{ PB_DS_ASSERT_VALID((*this)) }

PB_DS_CLASS_T_DEC
inline void
PB_DS_CLASS_C_DEC::
notify_inserted(size_type num_entries)
{
  m_resize_needed = (num_entries >= m_next_grow_size);
  size_base::set_size(num_entries);
  PB_DS_ASSERT_VALID((*this))
}

PB_DS_CLASS_T_DEC
inline void
PB_DS_CLASS_C_DEC::
notify_erased(size_type num_entries)
{
  size_base::set_size(num_entries);
  m_resize_needed = num_entries <= m_next_shrink_size;
  PB_DS_ASSERT_VALID((*this))
}

PB_DS_CLASS_T_DEC
inline bool
PB_DS_CLASS_C_DEC::
is_resize_needed() const
{
  PB_DS_ASSERT_VALID((*this))
  return m_resize_needed;
}

PB_DS_CLASS_T_DEC
inline bool
PB_DS_CLASS_C_DEC::
is_grow_needed(size_type /*size*/, size_type num_entries) const
{
  _GLIBCXX_DEBUG_ASSERT(m_resize_needed);
  return num_entries >= m_next_grow_size;
}

PB_DS_CLASS_T_DEC
PB_DS_CLASS_C_DEC::
~hash_load_check_resize_trigger() { }

PB_DS_CLASS_T_DEC
void
PB_DS_CLASS_C_DEC::
notify_resized(size_type new_size)
{
  m_resize_needed = false;
  m_next_grow_size = size_type(m_load_max * new_size - 1);
  m_next_shrink_size 