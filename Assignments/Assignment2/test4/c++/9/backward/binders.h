// Functor implementations -*- C++ -*-

// Copyright (C) 2001-2019 Free Software Foundation, Inc.
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

/*
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Hewlett-Packard Company makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 *
 * Copyright (c) 1996-1998
 * Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 */

/** @file backward/binders.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{functional}
 */

#ifndef _BACKWARD_BINDERS_H
#define _BACKWARD_BINDERS_H 1

// Suppress deprecated warning for this file.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  // 20.3.6 binders
  /** @defgroup binders Binder Classes
   * @ingroup functors
   *
   *  Binders turn functions/functors with two arguments into functors
   *  with a single argument, storing an argument to be applied later.
   *  For example, a variable @c B of type @c binder1st is constructed
   *  from a functor @c f and an argument @c x. Later, B's @c
   *  operator() is called with a single argument @c y. The return
   *  value is the value of @c f(x,y). @c B can be @a called with
   *  various arguments (y1, y2, ...) and will in turn call @c
   *  f(x,y1), @c f(x,y2), ...
   *
   *  The function @c bind1st is provided to save some typing. It takes the
   *  function and an argument as parameters, and returns an instance of
   *  @c binder1st.
   *
   *  The type @c binder2nd and its creator function @c bind2nd do the same
   *  thing, but the stored argument is passed as the second parameter instead
   *  of the first, e.g., @c bind2nd(std::minus<float>(),1.3) will create a
   *  functor whose @c operator() accepts a floating-point number, subtracts
   *  1.3 from it, and returns the result. (If @c bind1st had been used,
   *  the functor would perform <em>1.3 - x</em> instead.
   *
   *  Creator-wrapper functions like @c bind1st are intended to be used in
   *  calling algorithms. Their return values will be temporary objects.
   *  (The goal is to not require you to type names like
   *  @c std::binder1st<std::plus<int>> for declaring