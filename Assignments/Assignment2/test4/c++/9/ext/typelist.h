// -*- C++ -*-

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

// Copyright (C) 2004 Ami Tavory and Vladimir Dreizin, IBM-HRL.

// Permission to use, copy, modify, sell, and distribute this software
// is hereby granted without fee, provided that the above copyright
// notice appears in all copies, and that both that copyright notice and
// this permission notice appear in supporting documentation. None of
// the above authors, nor IBM Haifa Research Laboratories, make any
// representation about the suitability of this software for any
// purpose. It is provided "as is" without express or implied warranty.

/**
 *  @file ext/typelist.h
 *  This file is a GNU extension to the Standard C++ Library.
 *
 *  Contains typelist_chain definitions.
 *  Typelists are an idea by Andrei Alexandrescu.
 */

#ifndef _TYPELIST_H
#define _TYPELIST_H 1

#include <ext/type_traits.h>

namespace __gnu_cxx _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

/** @namespace __gnu_cxx::typelist
 *  @brief GNU typelist extensions for public compile-time use.
*/
namespace typelist
{
  struct null_type { };

  template<typename Root>
    struct node
    {
      typedef Root 	root;
    };

  // Forward declarations of functors.
  template<typename Hd, typename Typelist>
    struct chain
    {
      typedef Hd 	head;
      typedef Typelist 	tail;
    };

  // Apply all typelist types to unary functor.
  template<typename Fn, typename Typelist>
    void
    apply(Fn&, Typelist);

  /// Apply all typelist types to generator functor.
  template<typename Gn, typename Typelist>
    void
    apply_generator(Gn&, Typelist);

  // Apply all typelist types and values to generator functor.
  template<typename Gn, typename TypelistT, typename TypelistV>
    void
    apply_generator(Gn&, TypelistT, TypelistV);

  template<typename Typelist0, typename Typelist1>
    struct append;

  template<typename Typelist_Typelist>
    struct append_typelist;

  template<typename Typelist, typename T>
    struct contains;

  template<typename Typelist, template<typename T> class Pred>
    struct filter;

  template<typename Typelist, int i>
    struct at_index;

  template<typename Typelist, template<typename T> class Transform>
    struct transform;

  template<typename Typelist_Typelist>
    struct flatten;

  template<typename Typelist>
    struct from_first;

  template<typename T1>
    struct create1;

  template<typename T1, typename T2>
    struct create2;

  template<typename T1, typename T2, typename T3>
    struct create3;

  template<typename T1, typename T2, typename T3, typename T4>
    struct create4;

  template<typename T1, typename T2, typename T3, typename T4, typename T5>
    struct create5;

  template<typename T1, typename T2, typename T3,
	   typename T4, typename T5, typename T6>
    struct create6;

namespace detail
{
  template<typename Fn, typename Typelist_Chain>
    struct apply_;

  template<typename Fn, typename Hd, typename Tl>
    struct apply_<Fn, chain<Hd, Tl> >
    {
      void
      operator()(Fn& f)
      {
	f.operator()(Hd());
	apply_<Fn, Tl> next;
	