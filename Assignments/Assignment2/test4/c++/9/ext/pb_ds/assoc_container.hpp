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
 * @file assoc_container.hpp
 * Contains associative containers.
 */

#ifndef PB_DS_ASSOC_CNTNR_HPP
#define PB_DS_ASSOC_CNTNR_HPP

#include <bits/c++config.h>
#include <ext/typelist.h>
#include <ext/pb_ds/tag_and_trait.hpp>
#include <ext/pb_ds/detail/standard_policies.hpp>
#include <ext/pb_ds/detail/container_base_dispatch.hpp>
#include <ext/pb_ds/detail/branch_policy/traits.hpp>

namespace __gnu_pbds
{
  /**
   *  @defgroup containers-pbds Containers
   *  @ingroup pbds
   *  @{
   */

  /**
   *  @defgroup hash-based Hash-Based
   *  @ingroup containers-pbds
   *  @{
   */
#define PB_DS_HASH_BASE \
  detail::container_base_dispatch<Key, Mapped, _Alloc, Tag, \
    typename __gnu_cxx::typelist::append< \
    typename __gnu_cxx::typelist::create4<Hash_Fn, Eq_Fn, Resize_Policy, \
    detail::integral_constant<int, Store_Hash> >::type, Policy_Tl>::type>::type

  /**
   *  @defgroup hash-detail Base and Policy Classes
   *  @ingroup hash-based
   */

  /**
   *  A hashed container abstraction.
   *
   *  @tparam Key 	    	Key type.
   *  @tparam Mapped 	    	Map type.
   *  @tparam Hash_Fn	    	Hashing functor.
   *  @tparam Eq_Fn	    	Equal functor.
   *  @tparam Resize_Policy 	Resizes hash.
   *  @tparam Store_Hash    	Indicates whether the hash value
   *                            will be stored along with each key.
   *  @tparam Tag 	    	Instantiating data structure type,
   *			    	see container_tag.
   *  @tparam Policy_TL	    	Policy typelist.
   *  @tparam _Alloc 	    	Allocator type.
   *
   *  Base is dispatched at compile time via Tag, from the following
   *  choices: cc_hash_tag, gp_hash_tag, and descendants of basic_hash_tag.
   *
   *  Base choices are: detail::cc_ht_map, detail::gp_ht_map
   */
  template<typename Key,
	   typename Mapped,
	   typename Hash_Fn,
	   typename Eq_Fn,
	   typename Resize_Policy,
	   bool Store_Hash,
	   typename Tag,
	   typename Policy_Tl,
	   typename _Alloc>
  class basic_hash_table : public PB_DS_HASH_BASE
  {
  private:
    typedef typename PB_DS_HASH_BASE 		base_type;

  public:
    virtual
    ~basic_hash_table() { }

  protected:
    basic_hash_table() { }

    basic_hash_table(const basic_hash_table& other)
    : base_type((const base_type&)other) { }

    template<typename T0>
      basic_hash_table(T0 t0) : base_type(t0) { }

    template<typename T0, typename T1>
      basic_hash_table(T0 t0, T1 t1) : base_type(t0, t1) { }

    template<typename T0, typename T1, typename T2>
     