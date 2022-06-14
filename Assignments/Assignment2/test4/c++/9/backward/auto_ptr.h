// auto_ptr implementation -*- C++ -*-

// Copyright (C) 2007-2019 Free Software Foundation, Inc.
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

/** @file backward/auto_ptr.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{memory}
 */

#ifndef _BACKWARD_AUTO_PTR_H
#define _BACKWARD_AUTO_PTR_H 1

#include <bits/c++config.h>
#include <debug/debug.h>

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  /**
   *  A wrapper class to provide auto_ptr with reference semantics.
   *  For example, an auto_ptr can be assigned (or constructed from)
   *  the result of a function which returns an auto_ptr by value.
   *
   *  All the auto_ptr_ref stuff should happen behind the scenes.
   */
  template<typename _Tp1>
    struct auto_ptr_ref
    {
      _Tp1* _M_ptr;
      
      explicit
      auto_ptr_ref(_Tp1* __p): _M_ptr(__p) { }
    } _GLIBCXX_DEPRECATED;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

  /**
   *  @brief  A simple smart pointer providing strict ownership semantics.
   *
   *  The Standard says:
   *  <pre>
   *  An @c auto_ptr owns the object it holds a pointer to.  Copying
   *  an @c auto_ptr copies the pointer and transfers ownership to the
   *  destination.  If more than one @c auto_ptr owns the same object
   *  at the same time the behavior of the program is undefined.
   *
   *  The uses of @c auto_ptr include providing temporary
   *  exception-safety for dynamically allocated memory, passing
   *  ownership of dynamically allocated memory to a function, and
   *  returning dynamically allocated memory from a function.  @c
   *  auto_ptr does not meet the CopyConstructible and Assignable
   *  requirements for Standard Library <a
   *  href="tables.html#65">container</a> elements and thus
   *  instantiating a Standard Library container with an @c auto_ptr
   *  results in undefined behavior.
   *  </pre>
   *  Quoted from [20.4.5]/3.
   *
   *  Good examples of what can and cannot be done with auto_ptr can
   *  be found in the libstdc++ testsuite.
   *
   *  _GLIBCXX_RESOLVE_LIB_DEFECTS
   *  127.  auto_ptr<> conversion issues
   *  These resolutions have all been incorporated.
   */
  template<typename _Tp>
    class auto_ptr
    {
    private:
      _Tp* _M_ptr;
      
    public:
      /// The pointed-to type.
      typedef _Tp element_type;
      
      /**
       *  @brief  An %auto_ptr is usually constructed from a raw pointer.
       *  @param  __p  A pointer (defaults to NULL).
       *
       *  This object now @e owns the object pointed to by @a __p.
       */
      explicit
      auto_ptr(element_type* __p = 0) throw() : _M_ptr(__p) { }

      /**
       *  @brief  An %auto_ptr can be constructed from another %auto_ptr.
       *  @param  __a  Another %auto_ptr of the same type.
       *
       *  This object now @e owns the object previously owned by @a __a,
       *  which has given up ownership.
       */
      auto_ptr(auto_ptr& __a) throw() : _M_ptr(__a.release()) { }

      /**
       *  @brief  An %auto_ptr can be constructed