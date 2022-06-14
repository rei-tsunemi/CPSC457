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
 * @file hash_prime_size_policy_imp.hpp
 * Contains a resize size policy implementation.
 */

#pragma GCC system_header

namespace detail
{
  enum
    {
      num_distinct_sizes_32_bit = 30,
      num_distinct_sizes_64_bit = 62,
      num_distinct_sizes = sizeof(std::size_t) != 8 ? 
            num_distinct_sizes_32_bit : num_distinct_sizes_64_bit,	
    };

  // Originally taken from the SGI implementation; acknowledged in the docs.
  // Further modified (for 64 bits) from tr1's hashtable.
  static const std::size_t g_a_sizes[num_distinct_sizes_64_bit] =
    {
      /* 0     */              5ul,
      /* 1     */              11ul, 
      /* 2     */              23ul, 
      /* 3     */              47ul, 
      /* 4     */              97ul, 
      /* 5     */              199ul, 
      /* 6     */              409ul, 
      /* 7     */              823ul, 
      /* 8     */              1741ul, 
      /* 9     */              3469ul, 
      /* 10    */              6949ul, 
      /* 11    */              14033ul, 
      /* 12    */              28411ul, 
      /* 13    */              57557ul, 
      /* 14    */              116731ul, 
      /* 15    */              236897ul,
      /* 16    */              480881ul, 
      /* 17    */              976369ul,
      /* 18    */              1982627ul, 
      /* 19    */              4026031ul,
      /* 20    */              8175383ul, 
      /* 21    */              16601593ul, 
      /* 22    */              33712729ul,
      /* 23    */              68460391ul, 
      /* 24    */              139022417ul, 
      /* 25    */              282312799ul, 
      /* 26    */              573292817ul, 
      /* 27    */              1164186217ul,
      /* 28    */              2364114217ul, 
      /* 29    */              4294967291ul,
      /* 30    */ (std::size_t)8589934583ull,
      /* 31    */ (std::size_t)17179869143ull,
      /* 32    */ (std::size_t)34359738337ull,
      /* 33    */ (std::size_t)68719476731ull,
      /* 34    */ (std::size_t)137438953447ull,
      /* 35    */ (std::size_t)274877906899ull,
      /* 36    */ (std::size_t)549755813881ull,
      /* 37    */ (std::size_t)1099511627689ull,
      /* 38    */ (std::size_t)2199023255531ull,
      /* 39    */ (std::size_t)4398046511093ull,
      /* 40    */ (std::size_t)8796093022151ull,
      /* 41    */ (std::size_t)17592186044399ull,
      /* 42    */ (std::size_t)35184372088777ull,
      /* 43    */ (std::size_t)70368744177