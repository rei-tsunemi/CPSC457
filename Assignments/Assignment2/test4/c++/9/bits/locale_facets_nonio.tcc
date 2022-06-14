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

/** @file bits/locale_facets_nonio.tcc
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{locale}
 */

#ifndef _LOCALE_FACETS_NONIO_TCC
#define _LOCALE_FACETS_NONIO_TCC 1

#pragma GCC system_header

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  template<typename _CharT, bool _Intl>
    struct __use_cache<__moneypunct_cache<_CharT, _Intl> >
    {
      const __moneypunct_cache<_CharT, _Intl>*
      operator() (const locale& __loc) const
      {
	const size_t __i = moneypunct<_CharT, _Intl>::id._M_id();
	const locale::facet** __caches = __loc._M_impl->_M_caches;
	if (!__caches[__i])
	  {
	    __moneypunct_cache<_CharT, _Intl>* __tmp = 0;
	    __try
	      {
		__tmp = new __moneypunct_cache<_CharT, _Intl>;
		__tmp->_M_cache(__loc);
	      }
	    __catch(...)
	      {
		delete __tmp;
		__throw_exception_again;
	      }
	    __loc._M_impl->_M_install_cache(__tmp, __i);
	  }
	return static_cast<
	  const __moneypunct_cache<_CharT, _Intl>*>(__caches[__i]);
      }
    };

  template<typename _CharT, bool _Intl>
    void
    __moneypunct_cache<_CharT, _Intl>::_M_cache(const locale& __loc)
    {
      const moneypunct<_CharT, _Intl>& __mp =
	use_facet<moneypunct<_CharT, _Intl> >(__loc);

      _M_decimal_point = __mp.decimal_point();
      _M_thousands_sep = __mp.thousands_sep();
      _M_frac_digits = __mp.frac_digits();

      char* __grouping = 0;
      _CharT* __curr_symbol = 0;
      _CharT* __positive_sign = 0;
      _CharT* __negative_sign = 0;     
      __try
	{
	  const string& __g = __mp.grouping();
	  _M_grouping_size = __g.size();
	  __grouping = new char[_M_grouping_size];
	  __g.copy(__grouping, _M_grouping_size);
	  _M_use_grouping = (_M_grouping_size
			     && static_cast<signed char>(__grouping[0]) > 0
			     && (__grouping[0]
				 != __gnu_cxx::__numeric_traits<char>::__max));

	  const basic_string<_CharT>& __cs = __mp.curr_symbol();
	  _M_curr_symbol_size = __cs.size();
	  __curr_symbol = new _CharT[_M_curr_symbol_size];
	  __cs.copy(__curr_symbol, _M_curr_symbol_size);

	  const basic_string<_CharT>& __ps = __mp.positive_sign();
	  _M_positive_sign_size = __ps.size();
	  __positive_sign = new _CharT[_M_positive_sign_size];
	  __ps.copy(__positive_sign, _M_positive_sign_size);

	  const basic_string<_CharT>& __ns = __mp.negative_sign();
	  _M_negative_sign_size = __ns.size();
	  __negative_sign = new _CharT[_M_negative_sign_size];
	  __ns.copy(__negative_sign, _M_negative_sign_size);

	  _M_pos_format = __mp.pos_format();
	  _M_neg_format = __mp.neg_format();

	  const ctype<_CharT>& __ct = use_facet<ctype<_CharT> >(__loc);
	  __ct.widen(money_base::_S_atoms,
		     money_base::_S_atoms + money_base::_S_end, _M_atoms);

	  _M_grouping = __grouping;
	  _M_curr_symbol = __curr_symbol;
	  _M_positive_sign = __positive_sign;
	  _M_negative_sign = __negative_sign;
	  _M_allocated = true;
	}
      __catch(...)
	{
	  delete [] __grouping;
	  delete [] __curr_