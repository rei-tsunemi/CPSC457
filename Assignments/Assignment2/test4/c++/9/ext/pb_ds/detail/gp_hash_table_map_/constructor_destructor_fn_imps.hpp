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
 * @file gp_hash_table_map_/constructor_destructor_fn_imps.hpp
 * Contains implementations of gp_ht_map_'s constructors, destructor,
 * and related functions.
 */

PB_DS_CLASS_T_DEC
typename PB_DS_CLASS_C_DEC::entry_allocator
PB_DS_CLASS_C_DEC::s_entry_allocator;

PB_DS_CLASS_T_DEC
template<typename It>
void
PB_DS_CLASS_C_DEC::
copy_from_range(It first_it, It last_it)
{
  while (first_it != last_it)
    insert(*(first_it++));
}

PB_DS_CLASS_T_DEC
PB_DS_CLASS_C_DEC::
PB_DS_GP_HASH_NAME() 
: ranged_probe_fn_base(resize_base::get_nearest_larger_size(1)),
  m_num_e(resize_base::get_nearest_larger_size(1)), m_num_used_e(0),
  m_entries(s_entry_allocator.allocate(m_num_e))
{
  initialize();
  PB_DS_ASSERT_VALID((*this))
}

PB_DS_CLASS_T_DEC
PB_DS_CLASS_C_DEC::
PB_DS_GP_HASH_NAME(const Hash_Fn& r_hash_fn)    
: ranged_probe_fn_base(resize_base::get_nearest_larger_size(1), r_hash_fn),
  m_num_e(resize_base::get_nearest_larger_size(1)), m_num_used_e(0),
  m_entries(s_entry_allocator.allocate(m_num_e))
{
  initialize();
  PB_DS_ASSERT_VALID((*this))
}

PB_DS_CLASS_T_DEC
PB_DS_CLASS_C_DEC::
PB_DS_GP_HASH_NAME(const Hash_Fn& r_hash_fn, const Eq_Fn& r_eq_fn) 
: hash_eq_fn_base(r_eq_fn),
  ranged_probe_fn_base(resize_base::get_nearest_larger_size(1), r_hash_fn),
  m_num_e(resize_base::get_nearest_larger_size(1)), m_num_used_e(0),
  m_entries(s_entry_allocator.allocate(m_num_e))
{
  initialize();
  PB_DS_ASSERT_VALID((*this))
}

PB_DS_CLASS_T_DEC
PB_DS_CLASS_C_DEC::
PB_DS_GP_HASH_NAME(const Hash_Fn& r_hash_fn, const Eq_Fn& r_eq_fn, 
		 const Comb_Probe_Fn& r_comb_hash_fn) 
: hash_eq_fn_base(r_eq_fn),
  ranged_probe_fn_base(resize_base::get_nearest_larger_size(1),
		       r_hash_fn, r_comb_hash_fn),
  m_num_e(resize_base::get_nearest_larger_size(1)), m_num_used_e(0),
  m_entries(s_entry_allocator.allocate(m_num_e))
{
  initialize();
  PB_DS_ASSERT_VALID((*this))
}

PB_DS_CLASS_T_DEC
PB_DS_CLASS_C_DEC::
PB_DS_GP_HASH_NAME(const Hash_Fn& r_hash_fn, const Eq_Fn& r_eq_fn, 
		 const Comb_Probe_Fn& comb_hash_fn, const Probe_Fn& prober) 
: hash_eq_fn_base(r_eq_fn),
  ranged_probe_fn_base(resize_base::get_nearest_larger_size(1),
		       r_hash_fn, comb_hash_fn, prober),
  m_num_e(resize_base::get_nearest_larger_size(1)), m_num_used_e(0),
  m_entries(s_entry_allocator.allocate(m_num_e))
{
  initialize();
  PB_DS_ASSERT_VALID((*this))
}

PB_DS_CLASS_T_DEC
PB_DS_CLASS_C_DEC::
PB_DS_GP_HASH_NAME(const Hash_Fn& r_hash_fn, const Eq_Fn& r_eq_fn, 
		 const Comb_Prob