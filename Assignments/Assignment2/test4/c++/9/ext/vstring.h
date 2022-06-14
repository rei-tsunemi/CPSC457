// Versatile string -*- C++ -*-

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

/** @file ext/vstring.h
 *  This file is a GNU extension to the Standard C++ Library.
 */

#ifndef _VSTRING_H
#define _VSTRING_H 1

#pragma GCC system_header

#if __cplusplus >= 201103L
#include <initializer_list>
#endif

#include <ext/vstring_util.h>
#include <ext/rc_string_base.h>
#include <ext/sso_string_base.h>

namespace __gnu_cxx _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  /**
   *  @class __versa_string vstring.h
   *  @brief  Template class __versa_string. 
   *  @ingroup extensions
   *
   *  Data structure managing sequences of characters and
   *  character-like objects. 
   */
  template<typename _CharT, typename _Traits, typename _Alloc,
	   template <typename, typename, typename> class _Base>
    class __versa_string
    : private _Base<_CharT, _Traits, _Alloc>
    {
      typedef _Base<_CharT, _Traits, _Alloc>                __vstring_base;    
      typedef typename __vstring_base::_CharT_alloc_type    _CharT_alloc_type;

      // Types:
    public:
      typedef _Traits					    traits_type;
      typedef typename _Traits::char_type		    value_type;
      typedef _Alloc					    allocator_type;
      typedef typename _CharT_alloc_type::size_type	    size_type;
      typedef typename _CharT_alloc_type::difference_type   difference_type;
      typedef value_type&               	            reference;
      typedef const value_type&                             const_reference;
      typedef typename _CharT_alloc_type::pointer	    pointer;
      typedef typename _CharT_alloc_type::const_pointer	    const_pointer;
      typedef __gnu_cxx::__normal_iterator<pointer, __versa_string>  iterator;
      typedef __gnu_cxx::__normal_iterator<const_pointer, __versa_string>
                                                            const_iterator;
      typedef std::reverse_iterator<const_iterator>	const_reverse_iterator;
      typedef std::reverse_iterator<iterator>		    reverse_iterator;

      // Data Member (public):
      ///  Value returned by various member functions when they fail.
      static const size_type	npos = static_cast<size_type>(-1);

    private:
      size_type
      _M_check(size_type __pos, const char* __s) const
      {
	if (__pos > this->size())
	  std::__throw_out_of_range_fmt(__N("%s: __pos (which is %zu) > "
					    "this->size() (which is %zu)"),
					__s, __pos, this->size());
	return __pos;
      }

      void
      _M_check_length(size_type __n1, size_type __n2, const char* __s) const
      {
	if (this->max_size() - (this->size() - __n1) < __n2)
	  std::__throw_length_error(__N(__s));
      }

      // NB: _M_limit doesn't check for a bad __pos value.
      size_type
      _M_limit(size_type __pos, size_type __off) const _GLIBCXX_NOEXCEPT
      {
	const bool __testoff =  __off < this->size() - __pos;
	return __testoff ? __off : this->size() - __pos;
      }

      // True if _Rep and source do not overlap.
      bool
      _M_disjunct(const _CharT* __s) const _GLIBCXX_NOEXCEPT
      {
	return (std::less<const _Ch