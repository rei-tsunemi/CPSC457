// class template regex -*- C++ -*-

// Copyright (C) 2013-2019 Free Software Foundation, Inc.
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
 *  @file bits/regex.tcc
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{regex}
 */

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

namespace __detail
{
  // Result of merging regex_match and regex_search.
  //
  // __policy now can be _S_auto (auto dispatch) and _S_alternate (use
  // the other one if possible, for test purpose).
  //
  // That __match_mode is true means regex_match, else regex_search.
  template<typename _BiIter, typename _Alloc,
	   typename _CharT, typename _TraitsT,
	   _RegexExecutorPolicy __policy,
	   bool __match_mode>
    bool
    __regex_algo_impl(_BiIter                              __s,
		      _BiIter                              __e,
		      match_results<_BiIter, _Alloc>&      __m,
		      const basic_regex<_CharT, _TraitsT>& __re,
		      regex_constants::match_flag_type     __flags)
    {
      if (__re._M_automaton == nullptr)
	return false;

      typename match_results<_BiIter, _Alloc>::_Base_type& __res = __m;
      __m._M_begin = __s;
      __m._M_resize(__re._M_automaton->_M_sub_count());
      for (auto& __it : __res)
	__it.matched = false;

      bool __ret;
      if ((__re.flags() & regex_constants::__polynomial)
	  || (__policy == _RegexExecutorPolicy::_S_alternate
	      && !__re._M_automaton->_M_has_backref))
	{
	  _Executor<_BiIter, _Alloc, _TraitsT, false>
	    __executor(__s, __e, __m, __re, __flags);
	  if (__match_mode)
	    __ret = __executor._M_match();
	  else
	    __ret = __executor._M_search();
	}
      else
	{
	  _Executor<_BiIter, _Alloc, _TraitsT, true>
	    __executor(__s, __e, __m, __re, __flags);
	  if (__match_mode)
	    __ret = __executor._M_match();
	  else
	    __ret = __executor._M_search();
	}
      if (__ret)
	{
	  for (auto& __it : __res)
	    if (!__it.matched)
	      __it.first = __it.second = __e;
	  auto& __pre = __m._M_prefix();
	  auto& __suf = __m._M_suffix();
	  if (__match_mode)
	    {
	      __pre.matched = false;
	      __pre.first = __s;
	      __pre.second = __s;
	      __suf.matched = false;
	      __suf.first = __e;
	      __suf.second = __e;
	    }
	  else
	    {
	      __pre.first = __s;
	      __pre.second = __res[0].first;
	      __pre.matched = (__pre.first != __pre.second);
	      __suf.first = __res[0].second;
	      __suf.second = __e;
	      __suf.matched = (__suf.first != __suf.second);
	    }
	}
      else
	{
	  __m._M_resize(0);
	  for (auto& __it : __res)
	    {
	      __it.matched = false;
	      __it.first = __it.second = __e;
	    }
	}
      return __ret;
    }
}

  template<typename _Ch_type>
  template<typename _Fwd_iter>
    typename regex_traits<_Ch_type>::string_type
    regex_traits<_Ch_type>::
    lookup_collatename(_Fwd_iter __first, _Fwd_iter __last) const
    {
      typedef std::ctype<char_type> __ctype_type;
      const __ctype_type& __fctyp(use_facet<__ctype_type>(_M_locale));

      static const char* __collatenames[] =
	{
	  "NUL",
	  "SOH",