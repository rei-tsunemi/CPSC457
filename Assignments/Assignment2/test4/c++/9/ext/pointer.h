// Custom pointer adapter and sample storage policies

// Copyright (C) 2008-2019 Free Software Foundation, Inc.
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

/**
 *  @file ext/pointer.h
 *  This file is a GNU extension to the Standard C++ Library.
 *
 *  @author Bob Walters
 *
 * Provides reusable _Pointer_adapter for assisting in the development of
 * custom pointer types that can be used with the standard containers via
 * the allocator::pointer and allocator::const_pointer typedefs.
 */

#ifndef _POINTER_H
#define _POINTER_H 1

#pragma GCC system_header

#include <iosfwd>
#include <bits/stl_iterator_base_types.h>
#include <ext/cast.h>
#include <ext/type_traits.h>
#if __cplusplus >= 201103L
# include <bits/move.h>
# include <bits/ptr_traits.h>
#endif

namespace __gnu_cxx _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  /** 
   * @brief A storage policy for use with _Pointer_adapter<> which yields a
   *        standard pointer.
   * 
   *  A _Storage_policy is required to provide 4 things:
   *    1) A get() API for returning the stored pointer value.
   *    2) An set() API for storing a pointer value.
   *    3) An element_type typedef to define the type this points to.
   *    4) An operator<() to support pointer comparison.
   *    5) An operator==() to support pointer comparison.
   */
  template<typename _Tp> 
    class _Std_pointer_impl 
    {
    public:
      // the type this pointer points to.
      typedef _Tp element_type;
  
      // A method to fetch the pointer value as a standard T* value;
      inline _Tp* 
      get() const 
      { return _M_value; }
  
      // A method to set the pointer value, from a standard T* value;
      inline void 
      set(element_type* __arg) 
      { _M_value = __arg; }
  
      // Comparison of pointers
      inline bool
      operator<(const _Std_pointer_impl& __rarg) const
      { return (_M_value < __rarg._M_value); }
  
      inline bool
      operator==(const _Std_pointer_impl& __rarg) const
      { return (_M_value == __rarg._M_value); }

    private:
      element_type* _M_value;
    };

  /**
   * @brief A storage policy for use with _Pointer_adapter<> which stores
   *        the pointer's address as an offset value which is relative to
   *        its own address.
   * 
   * This is intended for pointers within shared memory regions which
   * might be mapped at different addresses by different processes.
   * For null pointers, a value of 1 is used.  (0 is legitimate
   * sometimes for nodes in circularly linked lists) This value was
   * chosen as the least likely to generate an incorrect null, As
   * there is no reason why any normal pointer would point 1 byte into
   * its own pointer address.
   */
  template<typename _Tp> 
    class _Relative_pointer_impl 
    {
    public:
      typedef _Tp element_type;
  
      _Tp*
      get() const 
      {
        if (_M_diff == 1)
          return 0;
        else
          return reinterpret_cast<_Tp*>(reinterpret_cast<_UIntPtrType>(this)
					+ _M_diff);
      }
  
      void 
      set(_Tp* __arg)
      {
        if (!__arg)
          _M_diff = 1;
        else
          _M_d