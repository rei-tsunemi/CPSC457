// The template and inlines for the -*- C++ -*- mask_array class.

// Copyright (C) 1997-2019 Free Software Foundation, Inc.
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

/** @file bits/mask_array.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{valarray}
 */

// Written by Gabriel Dos Reis <Gabriel.Dos-Reis@DPTMaths.ENS-Cachan.Fr>

#ifndef _MASK_ARRAY_H
#define _MASK_ARRAY_H 1

#pragma GCC system_header

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  /**
   * @addtogroup numeric_arrays
   * @{
   */

  /**
   *  @brief  Reference to selected subset of an array.
   *
   *  A mask_array is a reference to the actual elements of an array specified
   *  by a bitmask in the form of an array of bool.  The way to get a
   *  mask_array is to call operator[](valarray<bool>) on a valarray.  The
   *  returned mask_array then permits carrying operations out on the
   *  referenced subset of elements in the original valarray.
   *
   *  For example, if a mask_array is obtained using the array (false, true,
   *  false, true) as an argument, the mask array has two elements referring
   *  to array[1] and array[3] in the underlying array.
   *
   *  @param  Tp  Element type.
   */
  template <class _Tp>
    class mask_array
    {
    public:
      typedef _Tp value_type;

      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // 253. valarray helper functions are almost entirely useless

      ///  Copy constructor.  Both slices refer to the same underlying array.
      mask_array (const mask_array&);
      
      ///  Assignment operator.  Assigns elements to corresponding elements
      ///  of @a a.
      mask_array& operator=(const mask_array&);

      void operator=(const valarray<_Tp>&) const;
      ///  Multiply slice elements by corresponding elements of @a v.
      void operator*=(const valarray<_Tp>&) const;
      ///  Divide slice elements by corresponding elements of @a v.
      void operator/=(const valarray<_Tp>&) const;
      ///  Modulo slice elements by corresponding elements of @a v.
      void operator%=(const valarray<_Tp>&) const;
      ///  Add corresponding elements of @a v to slice elements.
      void operator+=(const valarray<_Tp>&) const;
      ///  Subtract corresponding elements of @a v from slice elements.
      void operator-=(const valarray<_Tp>&) const;
      ///  Logical xor slice elements with corresponding elements of @a v.
      void operator^=(const valarray<_Tp>&) const;
      ///  Logical and slice elements with corresponding elements of @a v.
      void operator&=(const valarray<_Tp>&) const;
      ///  Logical or slice elements with corresponding elements of @a v.
      void operator|=(const valarray<_Tp>&) const;
      ///  Left shift slice elements by corresponding elements of @a v.
      void operator<<=(const valarray<_Tp>&) const;
      ///  Right shift slice elements by corresponding elements of @a v.
      void operator>>=(const valarray<_Tp>&) const;
      ///  Assign all slice elements to @a t.
      void operator=(const _Tp&) const;

        //        ~mask_array ();