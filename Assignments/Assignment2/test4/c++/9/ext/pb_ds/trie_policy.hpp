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
 * @file trie_policy.hpp
 * Contains trie-related policies.
 */

#ifndef PB_DS_TRIE_POLICY_HPP
#define PB_DS_TRIE_POLICY_HPP

#include <bits/c++config.h>
#include <string>
#include <ext/pb_ds/detail/type_utils.hpp>
#include <ext/pb_ds/detail/trie_policy/trie_policy_base.hpp>

namespace __gnu_pbds
{
#define PB_DS_CLASS_T_DEC \
  template<typename String, typename String::value_type Min_E_Val, \
	   typename String::value_type Max_E_Val, bool Reverse, \
	   typename _Alloc>

#define PB_DS_CLASS_C_DEC \
  trie_string_access_traits<String, Min_E_Val,Max_E_Val,Reverse,_Alloc>

  /**
   *  Element access traits for string types.
   *
   *  @tparam String 	    	String type.
   *  @tparam Min_E_Val        	Minimal element value.
   *  @tparam Max_E_Val	    	Maximum element value.
   *  @tparam Reverse	        Reverse iteration should be used.
   *                            Default: false.
   *  @tparam _Alloc 	    	Allocator type.
   */
  template<typename String = std::string,
	   typename String::value_type Min_E_Val = detail::__numeric_traits<typename String::value_type>::__min,
	   typename String::value_type Max_E_Val = detail::__numeric_traits<typename String::value_type>::__max,
	   bool Reverse = false,
	   typename _Alloc = std::allocator<char> >
  struct trie_string_access_traits
  {
  public:
    typedef typename _Alloc::size_type			  size_type;
    typedef String 					  key_type;
    typedef typename _Alloc::template rebind<key_type>	  __rebind_k;
    typedef typename __rebind_k::other::const_reference   key_const_reference;

    enum
      {
	reverse = Reverse
      };

    /// Element const iterator type.
    typedef typename detail::__conditional_type<Reverse, \
		       typename String::const_reverse_iterator, \
		       typename String::const_iterator>::__type const_iterator;

    /// Element type.
    typedef typename std::iterator_traits<const_iterator>::value_type e_type;

    enum
      {
	min_e_val = Min_E_Val,
	max_e_val = Max_E_Val,
	max_size = max_e_val - min_e_val + 1
      };
    PB_DS_STATIC_ASSERT(min_max_size, max_size >= 2);

    /// Returns a const_iterator to the first element of
    /// key_const_reference agumnet.
    inline static const_iterator
    begin(key_const_reference);

    /// Returns a const_iterator to the after-last element of
    /// key_const_reference argument.
    inline static const_iterator
    end(key_const_reference);

    /// Maps an element to a position.
    inline static size_type
   