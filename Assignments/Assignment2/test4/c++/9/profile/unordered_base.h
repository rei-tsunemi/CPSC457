// Profiling unordered containers implementation details -*- C++ -*-

// Copyright (C) 2013-2019 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

/** @file profile/unordered_base.h
 *  This file is a GNU profile extension to the Standard C++ Library.
 */

#ifndef _GLIBCXX_PROFILE_UNORDERED
#define _GLIBCXX_PROFILE_UNORDERED 1

namespace std _GLIBCXX_VISIBILITY(default)
{
namespace __profile
{
  template<typename _UnorderedCont,
	   typename _Value, bool _Cache_hash_code>
    struct _Bucket_index_helper;

  template<typename _UnorderedCont, typename _Value>
    struct _Bucket_index_helper<_UnorderedCont, _Value, true>
    {
      static std::size_t
      bucket(const _UnorderedCont& __uc,
	     const __detail::_Hash_node<_Value, true>* __node)
      { return __node->_M_hash_code % __uc.bucket_count(); }
    };

  template<typename _UnorderedCont, typename _Value>
    struct _Bucket_index_helper<_UnorderedCont, _Value, false>
    {
      static std::size_t
      bucket(const _UnorderedCont& __uc,
	     const __detail::_Hash_node<_Value, false>* __node)
      { return __uc.bucket(__node->_M_v()); }
    };

  template<typename _UnorderedCont, typename _Key, typename _Mapped>
    struct _Bucket_index_helper<_UnorderedCont,
				std::pair<const _Key, _Mapped>, false>
    {
      typedef std::pair<const _Key, _Mapped> _Value;

      static std::size_t
      bucket(const _UnorderedCont& __uc,
	     const __detail::_Hash_node<_Value, false>* __node)
      { return __uc.bucket(__node->_M_v().first); }
    };

  template<typename _UnorderedCont, typename _Value, bool _Cache_hash_code>
    std::size_t
    __get_bucket_index(const _UnorderedCont& __uc,
		       const __detail::_Hash_node<_Value, _Cache_hash_code>* __node)
    {
      using __bucket_index_helper
	= _Bucket_index_helper<_UnorderedCont, _Value, _Cache_hash_code>;
      return __bucket_index_helper::bucket(__uc, __node);
    }

  template<typename _UnorderedCont,
	   typename _Value, bool _Cache_hash_code>
    struct _Equal_helper;

  template<typename _UnorderedCont, typename _Value>
    struct _Equal_helper<_UnorderedCont, _Value, true>
    {
      static std::size_t
      are_equal(const _UnorderedCont& __uc,
		const __detail::_Hash_node<_Value, true>* __lhs,
		const __detail::_Hash_node<_Value, true>* __rhs)
      {
	return __lhs->_M_hash_code == __rhs->_M_hash_code
	  && __uc.key_eq()(__lhs->_M_v(), __rhs->_M_v());
      }
    };

  template<typename _UnorderedCont,
	   typename _Value>
    struct _Equal_helper<_UnorderedCont, _Value, false>
    {
      static std::size_t
      are_equal(const _UnorderedCont& __uc,
		const __detail::_Hash_node<_Value, false>* __lhs,
		const __detail::_Hash_node<_Value, false>* __rhs)
      { return __uc.key_eq()(__lhs->_M_v(), __rhs->_M_v()); }
    };

  template<typename _UnorderedCont,
	   typename _Key, typename _Mapped>
    struct _Equal_helper<_UnorderedCont, std::pair<const _Key, _Mapped>, true>
    {
      typedef std::pair<const _Key, _Mapped> _Value;

      static std::size_t
      are_equal(const _UnorderedCont& __uc,
		const __detail::_Hash_node<_Value, true>* __lhs,
		const __detail::_Hash_node<_Value, true>* __rhs)
      {
	return __lhs->_M_hash_code == __rh