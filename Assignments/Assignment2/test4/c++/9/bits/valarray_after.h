// The template and inlines for the -*- C++ -*- internal _Meta class.

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

/** @file bits/valarray_after.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{valarray}
 */

// Written by Gabriel Dos Reis <Gabriel.Dos-Reis@cmla.ens-cachan.fr>

#ifndef _VALARRAY_AFTER_H
#define _VALARRAY_AFTER_H 1

#pragma GCC system_header

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

namespace __detail
{
  //
  // gslice_array closure.
  //
  template<class _Dom>
    class _GBase
    {
    public:
      typedef typename _Dom::value_type value_type;
      
      _GBase (const _Dom& __e, const valarray<size_t>& __i)
      : _M_expr (__e), _M_index(__i) {}
      
      value_type
      operator[] (size_t __i) const
      { return _M_expr[_M_index[__i]]; }
      
      size_t
      size () const
      { return _M_index.size(); }

    private:
      typename _ValArrayRef<_Dom>::__type	_M_expr;
      const valarray<size_t>&			_M_index;
    };

  template<typename _Tp>
    class _GBase<_Array<_Tp> >
    {
    public:
      typedef _Tp value_type;
      
      _GBase (_Array<_Tp> __a, const valarray<size_t>& __i)
      : _M_array (__a), _M_index(__i) {}
      
      value_type
      operator[] (size_t __i) const
      { return _M_array._M_data[_M_index[__i]]; }
      
      size_t
      size () const
      { return _M_index.size(); }

    private:
      const _Array<_Tp>       _M_array;
      const valarray<size_t>& _M_index;
    };

  template<class _Dom>
    struct _GClos<_Expr, _Dom>
    : _GBase<_Dom>
    {
      typedef _GBase<_Dom> _Base;
      typedef typename _Base::value_type value_type;
      
      _GClos (const _Dom& __e, const valarray<size_t>& __i)
      : _Base (__e, __i) {}
    };

  template<typename _Tp>
    struct _GClos<_ValArray, _Tp>
    : _GBase<_Array<_Tp> >
    {
      typedef _GBase<_Array<_Tp> > _Base;
      typedef typename _Base::value_type value_type;
      
      _GClos (_Array<_Tp> __a, const valarray<size_t>& __i)
      : _Base (__a, __i) {}
    };

  //
  // indirect_array closure
  //
  template<class _Dom>
    class _IBase
    {
    public:
      typedef typename _Dom::value_type value_type;

      _IBase (const _Dom& __e, const valarray<size_t>& __i)
      : _M_expr (__e), _M_index (__i) {}
      
      value_type
      operator[] (size_t __i) const
      { return _M_expr[_M_index[__i]]; }
      
      size_t
      size() const
      { return _M_index.size(); }

    private:
      typename _ValArrayRef<_Dom>::__type	_M_expr;
      const valarray<size_t>&			_M_index;
    };

  template<class _Dom>
    struct _IClos<_Expr, _Dom>
    : _IBase<_Dom>
    {
      typedef _IBase<_Dom> _Base;
      typedef typename _Base::value_type value_type;
      
      _IClos (const _Dom& __e, const valarray<size_t>& __i)
      : _Base (__e, __i) {}
    };

  template<typename _Tp>
    struct _IClos<_ValArray, _Tp>
    : _IBase<valarray<_Tp> >
    {
      typedef _IBase<valarray<_Tp> > _Base;
      typedef _T