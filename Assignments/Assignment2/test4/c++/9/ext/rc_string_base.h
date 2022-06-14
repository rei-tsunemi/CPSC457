// Reference-counted versatile string base -*- C++ -*-

// Copyright (C) 2005-2019 Free Software Foundation, Inc.
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

/** @file ext/rc_string_base.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{ext/vstring.h}
 */

#ifndef _RC_STRING_BASE_H
#define _RC_STRING_BASE_H 1

#include <ext/atomicity.h>
#include <bits/stl_iterator_base_funcs.h>

namespace __gnu_cxx _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  /**
   *  Documentation?  What's that?
   *  Nathan Myers <ncm@cantrip.org>.
   *
   *  A string looks like this:
   *
   *  @code
   *                                        [_Rep]
   *                                        _M_length
   *   [__rc_string_base<char_type>]        _M_capacity
   *   _M_dataplus                          _M_refcount
   *   _M_p ---------------->               unnamed array of char_type
   *  @endcode
   *
   *  Where the _M_p points to the first character in the string, and
   *  you cast it to a pointer-to-_Rep and subtract 1 to get a
   *  pointer to the header.
   *
   *  This approach has the enormous advantage that a string object
   *  requires only one allocation.  All the ugliness is confined
   *  within a single pair of inline functions, which each compile to
   *  a single @a add instruction: _Rep::_M_refdata(), and
   *  __rc_string_base::_M_rep(); and the allocation function which gets a
   *  block of raw bytes and with room enough and constructs a _Rep
   *  object at the front.
   *
   *  The reason you want _M_data pointing to the character array and
   *  not the _Rep is so that the debugger can see the string
   *  contents. (Probably we should add a non-inline member to get
   *  the _Rep for the debugger to use, so users can check the actual
   *  string length.)
   *
   *  Note that the _Rep object is a POD so that you can have a
   *  static <em>empty string</em> _Rep object already @a constructed before
   *  static constructors have run.  The reference-count encoding is
   *  chosen so that a 0 indicates one reference, so you never try to
   *  destroy the empty-string _Rep object.
   *
   *  All but the last paragraph is considered pretty conventional
   *  for a C++ string implementation.
  */
 template<typename _CharT, typename _Traits, typename _Alloc>
    class __rc_string_base
    : protected __vstring_utility<_CharT, _Traits, _Alloc>
    {
    public:
      typedef _Traits					    traits_type;
      typedef typename _Traits::char_type		    value_type;
      typedef _Alloc					    allocator_type;

      typedef __vstring_utility<_CharT, _Traits, _Alloc>    _Util_Base;
      typedef typename _Util_Base::_CharT_alloc_type        _CharT_alloc_type;
      typedef typename _CharT_alloc_type::size_type	    size_type;

    private:
      // _Rep: string representation
      //   Invariants:
      //   1. String really contains _M_length + 1 characters: due to 21.3.4
      //      must be kept null-terminated.
      //   2. _M_capacity >= _M_length
      //      Allocated memory is always (_M_capacity + 1) * sizeof(