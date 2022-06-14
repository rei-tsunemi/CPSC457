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
 * @file hash_policy.hpp
 * Contains hash-related policies.
 */

#ifndef PB_DS_HASH_POLICY_HPP
#define PB_DS_HASH_POLICY_HPP

#include <bits/c++config.h>
#include <algorithm>
#include <vector>
#include <cmath>
#include <ext/pb_ds/exception.hpp>
#include <ext/pb_ds/detail/type_utils.hpp>
#include <ext/pb_ds/detail/hash_fn/mask_based_range_hashing.hpp>
#include <ext/pb_ds/detail/hash_fn/mod_based_range_hashing.hpp>
#include <ext/pb_ds/detail/resize_policy/hash_load_check_resize_trigger_size_base.hpp>

namespace __gnu_pbds
{
#define PB_DS_CLASS_T_DEC template<typename Size_Type>
#define PB_DS_CLASS_C_DEC linear_probe_fn<Size_Type>

  /// A probe sequence policy using fixed increments.
  template<typename Size_Type = std::size_t>
  class linear_probe_fn
  {
  public:
    typedef Size_Type size_type;

    void
    swap(PB_DS_CLASS_C_DEC& other);

  protected:
    /// Returns the i-th offset from the hash value.
    inline size_type
    operator()(size_type i) const;
  };

#include <ext/pb_ds/detail/hash_fn/linear_probe_fn_imp.hpp>

#undef PB_DS_CLASS_T_DEC
#undef PB_DS_CLASS_C_DEC

#define PB_DS_CLASS_T_DEC template<typename Size_Type>
#define PB_DS_CLASS_C_DEC quadratic_probe_fn<Size_Type>

  /// A probe sequence policy using square increments.
  template<typename Size_Type = std::size_t>
  class quadratic_probe_fn
  {
  public:
    typedef Size_Type size_type;

    void
    swap(PB_DS_CLASS_C_DEC& other);

  protected:
    /// Returns the i-th offset from the hash value.
    inline size_type
    operator()(size_type i) const;
  };

#include <ext/pb_ds/detail/hash_fn/quadratic_probe_fn_imp.hpp>

#undef PB_DS_CLASS_T_DEC
#undef PB_DS_CLASS_C_DEC

#define PB_DS_CLASS_T_DEC template<typename Size_Type>
#define PB_DS_CLASS_C_DEC direct_mask_range_hashing<Size_Type>

  /// A mask range-hashing class (uses a bitmask).
  template<typename Size_Type = std::size_t>
  class direct_mask_range_hashing
  : public detail::mask_based_range_hashing<Size_Type>
  {
  private:
    typedef detail::mask_based_range_hashing<Size_Type> mask_based_base;

  public:
    typedef Size_Type size_type;

    void
    swap(PB_DS_CLASS_C_DEC& other);

  protected:
    void
    notify_resized(size_type size);

    /// Transforms the __hash value hash into a ranged-hash value
    /// (using a bit-mask).
    inline size_type
    operator()(size_type hash) const;
  };

#include <ext/pb_ds/detail/hash_fn/direct_mask_range_hashing_imp.hpp>

#undef PB_DS_CLASS_T_DEC
#undef PB_DS_