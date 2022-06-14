// class template regex -*- C++ -*-

// Copyright (C) 2010-2019 Free Software Foundation, Inc.
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
 *  @file bits/regex.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{regex}
 */

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION
_GLIBCXX_BEGIN_NAMESPACE_CXX11
  template<typename, typename>
    class basic_regex;

  template<typename, typename>
    class match_results;

_GLIBCXX_END_NAMESPACE_CXX11

namespace __detail
{
  enum class _RegexExecutorPolicy : int { _S_auto, _S_alternate };

  template<typename _BiIter, typename _Alloc,
	   typename _CharT, typename _TraitsT,
	   _RegexExecutorPolicy __policy,
	   bool __match_mode>
    bool
    __regex_algo_impl(_BiIter			      __s,
		      _BiIter			      __e,
		      match_results<_BiIter, _Alloc>&      __m,
		      const basic_regex<_CharT, _TraitsT>& __re,
		      regex_constants::match_flag_type     __flags);

  template<typename, typename, typename, bool>
    class _Executor;
}

_GLIBCXX_BEGIN_NAMESPACE_CXX11

  /**
   * @addtogroup regex
   * @{
   */

  /**
   * @brief Describes aspects of a regular expression.
   *
   * A regular expression traits class that satisfies the requirements of
   * section [28.7].
   *
   * The class %regex is parameterized around a set of related types and
   * functions used to complete the definition of its semantics.  This class
   * satisfies the requirements of such a traits class.
   */
  template<typename _Ch_type>
    struct regex_traits
    {
    public:
      typedef _Ch_type				char_type;
      typedef std::basic_string<char_type>	string_type;
      typedef std::locale			locale_type;
    private:
      struct _RegexMask
	{
	  typedef std::ctype_base::mask _BaseType;
	  _BaseType _M_base;
	  unsigned char _M_extended;
	  static constexpr unsigned char _S_under = 1 << 0;
	  static constexpr unsigned char _S_valid_mask = 0x1;

	  constexpr _RegexMask(_BaseType __base = 0,
			       unsigned char __extended = 0)
	  : _M_base(__base), _M_extended(__extended)
	  { }

	  constexpr _RegexMask
	  operator&(_RegexMask __other) const
	  {
	    return _RegexMask(_M_base & __other._M_base,
			      _M_extended & __other._M_extended);
	  }

	  constexpr _RegexMask
	  operator|(_RegexMask __other) const
	  {
	    return _RegexMask(_M_base | __other._M_base,
			      _M_extended | __other._M_extended);
	  }

	  constexpr _RegexMask
	  operator^(_RegexMask __other) const
	  {
	    return _RegexMask(_M_base ^ __other._M_base,
			      _M_extended ^ __other._M_extended);
	  }

	  constexpr _RegexMask
	  operator~() const
	  { return _RegexMask(~_M_base, ~_M_extended); }

	  _RegexMask&
	  operator&=(_RegexMask __other)
	  { return *this = (*this) & __other; }

	  _RegexMask&
	  operator|=(_RegexMask __other)
	  { return *this = (*this) | __other; }

	  _RegexMask&
	  operator^=(_RegexMask __other)
	  { return *this = (*this) ^ __other; }

	  constexpr bool
	  operator==(_RegexMask __other) const
	  {
	    return (_M_extended & _S_valid_mask)
		   == (__other._M_extended & _S_vali