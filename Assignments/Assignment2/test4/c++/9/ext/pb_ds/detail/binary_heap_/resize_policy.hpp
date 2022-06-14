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
 * @file binary_heap_/resize_policy.hpp
 * Contains an implementation class for a binary_heap.
 */

#ifndef PB_DS_BINARY_HEAP_RESIZE_POLICY_HPP
#define PB_DS_BINARY_HEAP_RESIZE_POLICY_HPP

#include <debug/debug.h>

namespace __gnu_pbds
{
  namespace detail
  {
    /// Resize policy for binary heap.
    template<typename _Tp>
    class resize_policy
    {
    private:
      enum
	{
	  ratio = 8,
	  factor = 2
	};

      /// Next shrink size.
      _Tp 		m_shrink_size;

      /// Next grow size.
      _Tp 		m_grow_size;

    public:
      typedef _Tp	size_type;

      static const _Tp	min_size = 16;

      resize_policy() : m_shrink_size(0), m_grow_size(min_size)
      { PB_DS_ASSERT_VALID((*this)) }

      resize_policy(const resize_policy& other)
      : m_shrink_size(other.m_shrink_size), m_grow_size(other.m_grow_size)
      { PB_DS_ASSERT_VALID((*this)) }

      inline void
      swap(resize_policy<_Tp>&);

      inline bool
      resize_needed_for_grow(size_type) const;

      inline bool
      resize_needed_for_shrink(size_type) const;

      inline bool
      grow_needed(size_type) const;

      inline bool
      shrink_needed(size_type) const;

      inline size_type
      get_new_size_for_grow() const;

      inline size_type
      get_new_size_for_shrink() const;

      inline size_type
      get_new_size_for_arbitrary(size_type) const;

      inline void
      notify_grow_resize();

      inline void
      notify_shrink_resize();

      void
      notify_arbitrary(size_type);

#ifdef _GLIBCXX_DEBUG
      void
      assert_valid(const char*, int) const;
#endif

#ifdef PB_DS_BINARY_HEAP_TRACE_
      void
      trace() const;
#endif
    };

    template<typename _Tp>
      const _Tp resize_policy<_Tp>::min_size;

    template<typename _Tp>
    inline void
    resize_policy<_Tp>::
    swap(resize_policy<_Tp>& other)
    {
      std::swap(m_shrink_size, other.m_shrink_size);
      std::swap(m_grow_size, other.m_grow_size);
    }

    template<typename _Tp>
    inline bool
    resize_policy<_Tp>::
    resize_needed_for_grow(size_type size) const
    {
      _GLIBCXX_DEBUG_ASSERT(size <= m_grow_size);
      return size == m_grow_size;
    }

    template<typename _Tp>
    inline bool
    resize_policy<_Tp>::
    resize_needed_for_shrink(size_type size) const
    {
      _GLIBCXX_DEBUG_ASSERT(size <= m_grow_size);
      return size == m_shrink_size;
    }

    template<typename _Tp>
    inline typename resize_policy<_Tp>::size_type
   