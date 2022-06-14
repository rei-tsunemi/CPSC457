// Internal policy header for TR1 unordered_set and unordered_map -*- C++ -*-

// Copyright (C) 2010-2019 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

/** @file tr1/hashtable_policy.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. 
 *  @headername{tr1/unordered_map, tr1/unordered_set}
 */

namespace std _GLIBCXX_VISIBILITY(default)
{ 
_GLIBCXX_BEGIN_NAMESPACE_VERSION

namespace tr1
{
namespace __detail
{
  // Helper function: return distance(first, last) for forward
  // iterators, or 0 for input iterators.
  template<class _Iterator>
    inline typename std::iterator_traits<_Iterator>::difference_type
    __distance_fw(_Iterator __first, _Iterator __last,
		  std::input_iterator_tag)
    { return 0; }

  template<class _Iterator>
    inline typename std::iterator_traits<_Iterator>::difference_type
    __distance_fw(_Iterator __first, _Iterator __last,
		  std::forward_iterator_tag)
    { return std::distance(__first, __last); }

  template<class _Iterator>
    inline typename std::iterator_traits<_Iterator>::difference_type
    __distance_fw(_Iterator __first, _Iterator __last)
    {
      typedef typename std::iterator_traits<_Iterator>::iterator_category _Tag;
      return __distance_fw(__first, __last, _Tag());
    }

  // Auxiliary types used for all instantiations of _Hashtable: nodes
  // and iterators.
  
  // Nodes, used to wrap elements stored in the hash table.  A policy
  // template parameter of class template _Hashtable controls whether
  // nodes also store a hash code. In some cases (e.g. strings) this
  // may be a performance win.
  template<typename _Value, bool __cache_hash_code>
    struct _Hash_node;

  template<typename _Value>
    struct _Hash_node<_Value, true>
    {
      _Value       _M_v;
      std::size_t  _M_hash_code;
      _Hash_node*  _M_next;
    };

  template<typename _Value>
    struct _Hash_node<_Value, false>
    {
      _Value       _M_v;
      _Hash_node*  _M_next;
    };

  // Local iterators, used to iterate within a bucket but not between
  // buckets.
  template<typename _Value, bool __cache>
    struct _Node_iterator_base
    {
      _Node_iterator_base(_Hash_node<_Value, __cache>* __p)
      : _M_cur(__p) { }
      
      void
      _M_incr()
      { _M_cur = _M_cur->_M_next; }

      _Hash_node<_Value, __cache>*  _M_cur;
    };

  template<typename _Value, bool __cache>
    inline bool
    operator==(const _Node_iterator_base<_Value, __cache>& __x,
	       const _Node_iterator_base<_Value, __cache>& __y)
    { return __x._M_cur == __y._M_cur; }

  template<typename _Value, bool __cache>
    inline bool
    operator!=(const _Node_iterator_base<_Value, __cache>& __x,
	       const _Node_iterator_base<_Value, __cache>& __y)
    { return __x._M_cur != __y._M_cur; }

  template<typename _Value, bool __constant_iterators, bool __cache>
    struct _Node_iterator
    : public _Node_iterator_base<_Value, __cache>
    {
      typedef _Value                                   value_type;
      typedef typename
      __gnu_cxx::__conditional_type<__constant_iterators,
				  