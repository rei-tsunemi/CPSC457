// Locale support -*- C++ -*-

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

/** @file bits/locale_classes.tcc
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{locale}
 */

//
// ISO C++ 14882: 22.1  Locales
//

#ifndef _LOCALE_CLASSES_TCC
#define _LOCALE_CLASSES_TCC 1

#pragma GCC system_header

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  template<typename _Facet>
    locale::
    locale(const locale& __other, _Facet* __f)
    {
      _M_impl = new _Impl(*__other._M_impl, 1);

      __try
	{ _M_impl->_M_install_facet(&_Facet::id, __f); }
      __catch(...)
	{
	  _M_impl->_M_remove_reference();
	  __throw_exception_again;
	}
      delete [] _M_impl->_M_names[0];
      _M_impl->_M_names[0] = 0;   // Unnamed.
    }

  template<typename _Facet>
    locale
    locale::
    combine(const locale& __other) const
    {
      _Impl* __tmp = new _Impl(*_M_impl, 1);
      __try
	{
	  __tmp->_M_replace_facet(__other._M_impl, &_Facet::id);
	}
      __catch(...)
	{
	  __tmp->_M_remove_reference();
	  __throw_exception_again;
	}
      return locale(__tmp);
    }

  template<typename _CharT, typename _Traits, typename _Alloc>
    bool
    locale::
    operator()(const basic_string<_CharT, _Traits, _Alloc>& __s1,
	       const basic_string<_CharT, _Traits, _Alloc>& __s2) const
    {
      typedef std::collate<_CharT> __collate_type;
      const __collate_type& __collate = use_facet<__collate_type>(*this);
      return (__collate.compare(__s1.data(), __s1.data() + __s1.length(),
				__s2.data(), __s2.data() + __s2.length()) < 0);
    }

  /**
   *  @brief  Test for the presence of a facet.
   *  @ingroup locales
   *
   *  has_facet tests the locale argument for the presence of the facet type
   *  provided as the template parameter.  Facets derived from the facet
   *  parameter will also return true.
   *
   *  @tparam  _Facet  The facet type to test the presence of.
   *  @param  __loc  The locale to test.
   *  @return  true if @p __loc contains a facet of type _Facet, else false.
  */
  template<typename _Facet>
    bool
    has_facet(const locale& __loc) throw()
    {
      const size_t __i = _Facet::id._M_id();
      const locale::facet** __facets = __loc._M_impl->_M_facets;
      return (__i < __loc._M_impl->_M_facets_size
#if __cpp_rtti
	      && dynamic_cast<const _Facet*>(__facets[__i]));
#else
              && static_cast<const _Facet*>(__facets[__i]));
#endif
    }

  /**
   *  @brief  Return a facet.
   *  @ingroup locales
   *
   *  use_facet looks for and returns a reference to a facet of type Facet
   *  where Facet is the template parameter.  If has_facet(locale) is true,
   *  there is a suitable facet to return.  It throws std::bad_cast if the
   *  locale doesn't contain a facet of type Facet.
   *
   *  @tparam  _Facet  The facet type to access.
   *  @param  __loc  The locale to use.
   *  @return  Reference to facet of type Facet.
   *  @throw  std::bad_cast if @p __loc doesn't contain a facet of type _Facet.
  */
  template<typena