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
 *  @file bits/regex_scanner.tcc
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{regex}
 */

// FIXME make comments doxygen format.

// N3376 specified 6 regex styles: ECMAScript, basic, extended, grep, egrep
// and awk
// 1) grep is basic except '\n' is treated as '|'
// 2) egrep is extended except '\n' is treated as '|'
// 3) awk is extended except special escaping rules, and there's no
//    back-reference.
//
// References:
//
// ECMAScript: ECMA-262 15.10
//
// basic, extended:
// http://pubs.opengroup.org/onlinepubs/009695399/basedefs/xbd_chap09.html
//
// awk: http://pubs.opengroup.org/onlinepubs/000095399/utilities/awk.html

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

namespace __detail
{
  template<typename _CharT>
    _Scanner<_CharT>::
    _Scanner(typename _Scanner::_IterT __begin,
	     typename _Scanner::_IterT __end,
	     _FlagT __flags, std::locale __loc)
    : _ScannerBase(__flags),
      _M_current(__begin), _M_end(__end),
      _M_ctype(std::use_facet<_CtypeT>(__loc)),
      _M_eat_escape(_M_is_ecma()
		    ? &_Scanner::_M_eat_escape_ecma
		    : &_Scanner::_M_eat_escape_posix)
    { _M_advance(); }

  template<typename _CharT>
    void
    _Scanner<_CharT>::
    _M_advance()
    {
      if (_M_current == _M_end)
	{
	  _M_token = _S_token_eof;
	  return;
	}

      if (_M_state == _S_state_normal)
	_M_scan_normal();
      else if (_M_state == _S_state_in_bracket)
	_M_scan_in_bracket();
      else if (_M_state == _S_state_in_brace)
	_M_scan_in_brace();
      else
	{
	  __glibcxx_assert(false);
	}
    }

  // Differences between styles:
  // 1) "\(", "\)", "\{" in basic. It's not escaping.
  // 2) "(?:", "(?=", "(?!" in ECMAScript.
  template<typename _CharT>
    void
    _Scanner<_CharT>::
    _M_scan_normal()
    {
      auto __c = *_M_current++;

      if (std::strchr(_M_spec_char, _M_ctype.narrow(__c, ' ')) == nullptr)
	{
	  _M_token = _S_token_ord_char;
	  _M_value.assign(1, __c);
	  return;
	}
      if (__c == '\\')
	{
	  if (_M_current == _M_end)
	    __throw_regex_error(
	      regex_constants::error_escape,
	      "Unexpected end of regex when escaping.");

	  if (!_M_is_basic()
	      || (*_M_current != '('
		  && *_M_current != ')'
		  && *_M_current != '{'))
	    {
	      (this->*_M_eat_escape)();
	      return;
	    }
	  __c = *_M_current++;
	}
      if (__c == '(')
	{
	  if (_M_is_ecma() && *_M_current == '?')
	    {
	      if (++_M_current == _M_end)
		__throw_regex_error(
		  regex_constants::error_paren,
		  "Unexpected end of regex when in an open parenthesis.");

	      if (*_M_current == ':')
		{
		  ++_M_current;
		  _M_token = _S_token_subexpr_no_group_begin;
		}
	      else if (*_M_current == '=')
		{
		  ++_M_current;
		  _M_token = _S_token_subexpr_lookahead_begin;
		  _M_value.assign(1, 'p');
		}
	      else if (*_M_current == '!')
		{
		  ++_M_current;
		  _M_token = _S_token_subexpr_lookahead_begin;
		  _M_va