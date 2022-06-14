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
 * @file ov_tree_map_/constructors_destructor_fn_imps.hpp
 * Contains an implementation class for ov_tree_.
 */

PB_DS_CLASS_T_DEC
typename PB_DS_CLASS_C_DEC::value_allocator
PB_DS_CLASS_C_DEC::s_value_alloc;

PB_DS_CLASS_T_DEC
typename PB_DS_CLASS_C_DEC::metadata_allocator
PB_DS_CLASS_C_DEC::s_metadata_alloc;

PB_DS_CLASS_T_DEC
PB_DS_CLASS_C_DEC::
PB_DS_OV_TREE_NAME() :
  m_a_values(0),
  m_a_metadata(0),
  m_end_it(0),
  m_size(0)
{ PB_DS_ASSERT_VALID((*this)) }

PB_DS_CLASS_T_DEC
PB_DS_CLASS_C_DEC::
PB_DS_OV_TREE_NAME(const Cmp_Fn& r_cmp_fn) :
  cmp_fn(r_cmp_fn),
  m_a_values(0),
  m_a_metadata(0),
  m_end_it(0),
  m_size(0)
{ PB_DS_ASSERT_VALID((*this)) }

PB_DS_CLASS_T_DEC
PB_DS_CLASS_C_DEC::
PB_DS_OV_TREE_NAME(const Cmp_Fn& r_cmp_fn, const node_update& r_nodeu) :
  cmp_fn(r_cmp_fn),
  node_update(r_nodeu),
  m_a_values(0),
  m_a_metadata(0),
  m_end_it(0),
  m_size(0)
{ PB_DS_ASSERT_VALID((*this)) }

PB_DS_CLASS_T_DEC
PB_DS_CLASS_C_DEC::
PB_DS_OV_TREE_NAME(const PB_DS_CLASS_C_DEC& other) :
#ifdef PB_DS_TREE_TRACE
  trace_base(other),
#endif
  cmp_fn(other),
  node_update(other),
  m_a_values(0),
  m_a_metadata(0),
  m_end_it(0),
  m_size(0)
{
  copy_from_ordered_range(other.begin(), other.end());
  PB_DS_ASSERT_VALID((*this))
}

PB_DS_CLASS_T_DEC
template<typename It>
inline void
PB_DS_CLASS_C_DEC::
copy_from_range(It first_it, It last_it)
{
#ifdef PB_DS_DATA_TRUE_INDICATOR
  typedef std::map<key_type, mapped_type, Cmp_Fn,
		   typename _Alloc::template rebind<value_type>::other>
    map_type;
#else
  typedef std::set<key_type, Cmp_Fn,
		   typename _Alloc::template rebind<Key>::other>
    map_type;
#endif

  map_type m(first_it, last_it);
  copy_from_ordered_range(m.begin(), m.end());
  PB_DS_ASSERT_VALID((*this))
}

PB_DS_CLASS_T_DEC
template<typename It>
void
PB_DS_CLASS_C_DEC::
copy_from_ordered_range(It first_it, It last_it)
{
  const size_type len = std::distance(first_it, last_it);
  if (len == 0)
    return;

  value_vector a_values = s_value_alloc.allocate(len);
  iterator target_it = a_values;
  It source_it = first_it;
  It source_end_it = last_it;

  cond_dtor<size_type> cd(a_values, target_it, len);
  while (source_it != source_end_it)
    {
      void* __v = const_cast<void*>(static_cast<const void*>(target_it));
      new (__v) value_type(*source_it++);
      ++target_it;
    }

  reallocate_metadata((node_update*)this, len);
  cd.set_no_action();
  m_a_values = a_values;
  m_size = len;
  m_end_it = m_a_values + m_