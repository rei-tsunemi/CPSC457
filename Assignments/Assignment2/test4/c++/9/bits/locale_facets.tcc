// Locale support -*- C++ -*-

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

/** @file bits/locale_facets.tcc
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{locale}
 */

#ifndef _LOCALE_FACETS_TCC
#define _LOCALE_FACETS_TCC 1

#pragma GCC system_header

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  // Routine to access a cache for the facet.  If the cache didn't
  // exist before, it gets constructed on the fly.
  template<typename _Facet>
    struct __use_cache
    {
      const _Facet*
      operator() (const locale& __loc) const;
    };

  // Specializations.
  template<typename _CharT>
    struct __use_cache<__numpunct_cache<_CharT> >
    {
      const __numpunct_cache<_CharT>*
      operator() (const locale& __loc) const
      {
	const size_t __i = numpunct<_CharT>::id._M_id();
	const locale::facet** __caches = __loc._M_impl->_M_caches;
	if (!__caches[__i])
	  {
	    __numpunct_cache<_CharT>* __tmp = 0;
	    __try
	      {
		__tmp = new __numpunct_cache<_CharT>;
		__tmp->_M_cache(__loc);
	      }
	    __catch(...)
	      {
		delete __tmp;
		__throw_exception_again;
	      }
	    __loc._M_impl->_M_install_cache(__tmp, __i);
	  }
	return static_cast<const __numpunct_cache<_CharT>*>(__caches[__i]);
      }
    };

  template<typename _CharT>
    void
    __numpunct_cache<_CharT>::_M_cache(const locale& __loc)
    {
      const numpunct<_CharT>& __np = use_facet<numpunct<_CharT> >(__loc);

      char* __grouping = 0;
      _CharT* __truename = 0;
      _CharT* __falsename = 0;
      __try
	{
	  const string& __g = __np.grouping();
	  _M_grouping_size = __g.size();
	  __grouping = new char[_M_grouping_size];
	  __g.copy(__grouping, _M_grouping_size);
	  _M_use_grouping = (_M_grouping_size
			     && static_cast<signed char>(__grouping[0]) > 0
			     && (__grouping[0]
				 != __gnu_cxx::__numeric_traits<char>::__max));

	  const basic_string<_CharT>& __tn = __np.truename();
	  _M_truename_size = __tn.size();
	  __truename = new _CharT[_M_truename_size];
	  __tn.copy(__truename, _M_truename_size);

	  const basic_string<_CharT>& __fn = __np.falsename();
	  _M_falsename_size = __fn.size();
	  __falsename = new _CharT[_M_falsename_size];
	  __fn.copy(__falsename, _M_falsename_size);

	  _M_decimal_point = __np.decimal_point();
	  _M_thousands_sep = __np.thousands_sep();

	  const ctype<_CharT>& __ct = use_facet<ctype<_CharT> >(__loc);
	  __ct.widen(__num_base::_S_atoms_out,
		     __num_base::_S_atoms_out
		     + __num_base::_S_oend, _M_atoms_out);
	  __ct.widen(__num_base::_S_atoms_in,
		     __num_base::_S_atoms_in
		     + __num_base::_S_iend, _M_atoms_in);

	  _M_grouping = __grouping;
	  _M_truename = __truename;
	  _M_falsename = __falsename;
	  _M_allocated = true;
	}
      __catch(...)
	{
	  delete [] __grouping;
	  delete [] __truename;
	  delete [] __falsename;
	  __throw_exception_again;
	}
    }

  // Used by both numeric and monetary facets.
  // Check to make sure that the __grouping_tmp string constructed in
  // money_get o