// -*- C++ -*-

// Copyright (C) 2004-2019 Free Software Foundation, Inc.
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

// (C) Copyright Jeremy Siek 2000. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.
//

/** @file bits/boost_concept_check.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{iterator}
 */

// GCC Note:  based on version 1.12.0 of the Boost library.

#ifndef _BOOST_CONCEPT_CHECK_H
#define _BOOST_CONCEPT_CHECK_H 1

#pragma GCC system_header

#include <bits/c++config.h>
#include <bits/stl_iterator_base_types.h>    // for traits and tags

namespace __gnu_cxx _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"

#define _IsUnused __attribute__ ((__unused__))

// When the C-C code is in use, we would like this function to do as little
// as possible at runtime, use as few resources as possible, and hopefully
// be elided out of existence... hmmm.
template <class _Concept>
inline void __function_requires()
{
  void (_Concept::*__x)() _IsUnused = &_Concept::__constraints;
}

// No definition: if this is referenced, there's a problem with
// the instantiating type not being one of the required integer types.
// Unfortunately, this results in a link-time error, not a compile-time error.
void __error_type_must_be_an_integer_type();
void __error_type_must_be_an_unsigned_integer_type();
void __error_type_must_be_a_signed_integer_type();

// ??? Should the "concept_checking*" structs begin with more than _ ?
#define _GLIBCXX_CLASS_REQUIRES(_type_var, _ns, _concept) \
  typedef void (_ns::_concept <_type_var>::* _func##_type_var##_concept)(); \
  template <_func##_type_var##_concept _Tp1> \
  struct _concept_checking##_type_var##_concept { }; \
  typedef _concept_checking##_type_var##_concept< \
    &_ns::_concept <_type_var>::__constraints> \
    _concept_checking_typedef##_type_var##_concept

#define _GLIBCXX_CLASS_REQUIRES2(_type_var1, _type_var2, _ns, _concept) \
  typedef void (_ns::_concept <_type_var1,_type_var2>::* _func##_type_var1##_type_var2##_concept)(); \
  template <_func##_type_var1##_type_var2##_concept _Tp1> \
  struct _concept_checking##_type_var1##_type_var2##_concept { }; \
  typedef _concept_checking##_type_var1##_type_var2##_concept< \
    &_ns::_concept <_type_var1,_type_var2>::__constraints> \
    _concept_checking_typedef##_type_var1##_type_var2##_concept

#define _GLIBCXX_CLASS_REQUIRES3(_type_var1, _type_var2, _type_var3, _ns, _concept) \
  typedef void (_ns::_concept <_type_var1,_type_var2,_type_var3>::* _func##_type_var1##_type_var2##_type_var3##_concept)(); \
  template <_func##_type_var1##_type_var2##_type_var3##_concept _Tp1> \
  struct _concept_checking##_type_var1##_type_var2##_type_var3##_concept { }; \
  typedef _concept_checking##_type_var1##_type_var2##_type_var3##_concept< \
