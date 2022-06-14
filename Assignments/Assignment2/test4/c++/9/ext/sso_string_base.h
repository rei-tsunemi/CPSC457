// Short-string-optimized versatile string base -*- C++ -*-

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

/** @file ext/sso_string_base.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{ext/vstring.h}
 */

#ifndef _SSO_STRING_BASE_H
#define _SSO_STRING_BASE_H 1

namespace __gnu_cxx _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  template<typename _CharT, typename _Traits, typename _Alloc>
    class __sso_string_base
    : protected __vstring_utility<_CharT, _Traits, _Alloc>
    {
    public:
      typedef _Traits					    traits_type;
      typedef typename _Traits::char_type		    value_type;

      typedef __vstring_utility<_CharT, _Traits, _Alloc>    _Util_Base;
      typedef typename _Util_Base::_CharT_alloc_type        _CharT_alloc_type;
      typedef typename _CharT_alloc_type::size_type	    size_type;
      
    private:
      // Data Members:
      typename _Util_Base::template _Alloc_hider<_CharT_alloc_type>
                                                            _M_dataplus;
      size_type                                             _M_string_length;

      enum { _S_local_capacity = 15 };
      
      union
      {
	_CharT           _M_local_data[_S_local_capacity + 1];
	size_type        _M_allocated_capacity;
      };

      void
      _M_data(_CharT* __p)
      { _M_dataplus._M_p = __p; }

      void
      _M_length(size_type __length)
      { _M_string_length = __length; }

      void
      _M_capacity(size_type __capacity)
      { _M_allocated_capacity = __capacity; }

      bool
      _M_is_local() const
      { return _M_data() == _M_local_data; }

      // Create & Destroy
      _CharT*
      _M_create(size_type&, size_type);
      
      void
      _M_dispose()
      {
	if (!_M_is_local())
	  _M_destroy(_M_allocated_capacity);
      }

      void
      _M_destroy(size_type __size) throw()
      { _M_get_allocator().deallocate(_M_data(), __size + 1); }

      // _M_construct_aux is used to implement the 21.3.1 para 15 which
      // requires special behaviour if _InIterator is an integral type
      template<typename _InIterator>
        void
        _M_construct_aux(_InIterator __beg, _InIterator __end, 
			 std::__false_type)
	{
          typedef typename iterator_traits<_InIterator>::iterator_category _Tag;
          _M_construct(__beg, __end, _Tag());
	}

      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // 438. Ambiguity in the "do the right thing" clause
      template<typename _Integer>
        void
        _M_construct_aux(_Integer __beg, _Integer __end, std::__true_type)
	{ _M_construct_aux_2(static_cast<size_type>(__beg), __end); }

      void
      _M_construct_aux_2(size_type __req, _CharT __c)
      { _M_construct(__req, __c); }

      template<typename _InIterator>
        void
        _M_construct(_InIterator __beg, _InIterator __end)
	{
	  typedef typename std::__is_integer<_InIterator>::__type _Integral;
	  _M_construct_aux(__beg, __end, _Integral());
        }

      // For Input Iterators, used in istreambuf_iterators, 