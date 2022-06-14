// Debug-mode error formatting implementation -*- C++ -*-

// Copyright (C) 2003-2019 Free Software Foundation, Inc.
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

/** @file debug/formatter.h
 *  This file is a GNU debug extension to the Standard C++ Library.
 */

#ifndef _GLIBCXX_DEBUG_FORMATTER_H
#define _GLIBCXX_DEBUG_FORMATTER_H 1

#include <bits/c++config.h>

#if __cpp_rtti
# include <typeinfo>
# define _GLIBCXX_TYPEID(_Type) &typeid(_Type)
#else
namespace std
{
  class type_info;
}
# define _GLIBCXX_TYPEID(_Type) 0
#endif

#if __cplusplus >= 201103L
namespace __gnu_cxx
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

template<typename _Iterator, typename _Container>
  class __normal_iterator;

_GLIBCXX_END_NAMESPACE_VERSION
}

namespace std
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

template<typename _Iterator>
  class reverse_iterator;

template<typename _Iterator>
  class move_iterator;

_GLIBCXX_END_NAMESPACE_VERSION
}
#endif

namespace __gnu_debug
{
  using std::type_info;

  template<typename _Iterator>
    bool __check_singular(const _Iterator&);

  class _Safe_sequence_base;

  template<typename _Iterator, typename _Sequence, typename _Category>
    class _Safe_iterator;

  template<typename _Iterator, typename _Sequence>
    class _Safe_local_iterator;

  template<typename _Sequence>
    class _Safe_sequence;

  enum _Debug_msg_id
  {
    // General checks
    __msg_valid_range,
    __msg_insert_singular,
    __msg_insert_different,
    __msg_erase_bad,
    __msg_erase_different,
    __msg_subscript_oob,
    __msg_empty,
    __msg_unpartitioned,
    __msg_unpartitioned_pred,
    __msg_unsorted,
    __msg_unsorted_pred,
    __msg_not_heap,
    __msg_not_heap_pred,
    // std::bitset checks
    __msg_bad_bitset_write,
    __msg_bad_bitset_read,
    __msg_bad_bitset_flip,
    // std::list checks
    __msg_self_splice,
    __msg_splice_alloc,
    __msg_splice_bad,
    __msg_splice_other,
    __msg_splice_overlap,
    // iterator checks
    __msg_init_singular,
    __msg_init_copy_singular,
    __msg_init_const_singular,
    __msg_copy_singular,
    __msg_bad_deref,
    __msg_bad_inc,
    __msg_bad_dec,
    __msg_iter_subscript_oob,
    __msg_advance_oob,
    __msg_retreat_oob,
    __msg_iter_compare_bad,
    __msg_compare_different,
    __msg_iter_order_bad,
    __msg_order_different,
    __msg_distance_bad,
    __msg_distance_different,
    // istream_iterator
    __msg_deref_istream,
    __msg_inc_istream,
    // ostream_iterator
    __msg_output_ostream,
    // istreambuf_iterator
    __msg_deref_istreambuf,
    __msg_inc_istreambuf,
    // forward_list
    __msg_insert_after_end,
    __msg_erase_after_bad,
    __msg_valid_range2,
    // unordered container local iterators
    __msg_local_iter_compare_bad,
    __msg_non_empty_range,
    // self move assign
    __msg_self_move_assign,
    // unordered container buckets
    __msg_bucket_index_oob,
    __msg_valid_load_factor,
    // others
    __msg_equal_allocs,
    __msg_insert_range_from_self,
    __msg_irreflexive_ordering
  };

  class _Error_formatter
  {
    // Tags denoting the type of parameter for construction
    struct _Is_iterator { };
    struct _Is