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
 * @file tag_and_trait.hpp
 * Contains tags and traits, e.g., ones describing underlying
 * data structures.
 */

#ifndef PB_DS_TAG_AND_TRAIT_HPP
#define PB_DS_TAG_AND_TRAIT_HPP

#include <bits/c++config.h>
#include <ext/pb_ds/detail/type_utils.hpp>

/**
 * @namespace __gnu_pbds
 * @brief GNU extensions for policy-based data structures for public use.
 */
namespace __gnu_pbds
{
  /** @defgroup pbds Policy-Based Data Structures
   *  @ingroup extensions
   *
   *  This is a library of policy-based elementary data structures:
   *  associative containers and priority queues. It is designed for
   *  high-performance, flexibility, semantic safety, and conformance
   *  to the corresponding containers in std (except for some points
   *  where it differs by design).
   *
   *  For details, see:
   *  http://gcc.gnu.org/onlinedocs/libstdc++/ext/pb_ds/index.html
   *
   *  @{
   */

  /**
   *  @defgroup tags Tags
   *  @{   
   */
  /// A trivial iterator tag. Signifies that the iterators has none of
  /// std::iterators's movement abilities.
  struct trivial_iterator_tag
  { };

  /// Prohibit moving trivial iterators.
  typedef void trivial_iterator_difference_type;


  /**
   *  @defgroup invalidation_tags  Invalidation Guarantees
   *  @ingroup tags
   *  @{
   */

  /**
   *  Signifies a basic invalidation guarantee that any iterator,
   *  pointer, or reference to a container object's mapped value type
   *  is valid as long as the container is not modified.
   */
  struct basic_invalidation_guarantee
  { };

  /**
   *  Signifies an invalidation guarantee that includes all those of
   *  its base, and additionally, that any point-type iterator,
   *  pointer, or reference to a container object's mapped value type
   *  is valid as long as its corresponding entry has not be erased,
   *  regardless of modifications to the container object.
   */
  struct point_invalidation_guarantee : public basic_invalidation_guarantee
  { };

  /**
   *  Signifies an invalidation guarantee that includes all those of
   *  its base, and additionally, that any range-type iterator
   *  (including the returns of begin() and end()) is in the correct
   *  relative positions to other range-type iterators as long as its
   *  corresponding entry has not be erased, regardless of
   *  modifications to the container object.
   */
  struct range_invalidation_guarantee : public point_invalidation_guarantee
  { };
  //@}


  /**
   *  @defgroup ds_tags Data Str