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
 *  @file bits/regex_scanner.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{regex}
 */

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

namespace __detail
{
  /**
   * @addtogroup regex-detail
   * @{
   */

  struct _ScannerBase
  {
  public:
    /// Token types returned from the scanner.
    enum _TokenT : unsigned
    {
      _S_token_anychar,
      _S_token_ord_char,
      _S_token_oct_num,
      _S_token_hex_num,
      _S_token_backref,
      _S_token_subexpr_begin,
      _S_token_subexpr_no_group_begin,
      _S_token_subexpr_lookahead_begin, // neg if _M_value[0] == 'n'
      _S_token_subexpr_end,
      _S_token_bracket_begin,
      _S_token_bracket_neg_begin,
      _S_token_bracket_end,
      _S_token_interval_begin,
      _S_token_interval_end,
      _S_token_quoted_class,
      _S_token_char_class_name,
      _S_token_collsymbol,
      _S_token_equiv_class_name,
      _S_token_opt,
      _S_token_or,
      _S_token_closure0,
      _S_token_closure1,
      _S_token_line_begin,
      _S_token_line_end,
      _S_token_word_bound, // neg if _M_value[0] == 'n'
      _S_token_comma,
      _S_token_dup_count,
      _S_token_eof,
      _S_token_bracket_dash,
      _S_token_unknown = -1u
    };

  protected:
    typedef regex_constants::syntax_option_type _FlagT;

    enum _StateT
    {
      _S_state_normal,
      _S_state_in_brace,
      _S_state_in_bracket,
    };

  protected:
    _ScannerBase(_FlagT __flags)
    : _M_state(_S_state_normal),
    _M_flags(__flags),
    _M_escape_tbl(_M_is_ecma()
		  ? _M_ecma_escape_tbl
		  : _M_awk_escape_tbl),
    _M_spec_char(_M_is_ecma()
		 ? _M_ecma_spec_char
		 : _M_flags & regex_constants::basic
		 ? _M_basic_spec_char
		 : _M_flags & regex_constants::extended
		 ? _M_extended_spec_char
		 : _M_flags & regex_constants::grep
		 ?  ".[\\*^$\n"
		 : _M_flags & regex_constants::egrep
		 ? ".[\\()*+?{|^$\n"
		 : _M_flags & regex_constants::awk
		 ? _M_extended_spec_char
		 : nullptr),
    _M_at_bracket_start(false)
    { __glibcxx_assert(_M_spec_char); }

  protected:
    const char*
    _M_find_escape(char __c)
    {
      auto __it = _M_escape_tbl;
      for (; __it->first != '\0'; ++__it)
	if (__it->first == __c)
	  return &__it->second;
      return nullptr;
    }

    bool
    _M_is_ecma() const
    { return _M_flags & regex_constants::ECMAScript; }

    bool
    _M_is_basic() const
    { return _M_flags & (regex_constants::basic | regex_constants::grep); }

    bool
    _M_is_extended() const
    {
      return _M_flags & (regex_constants::extended
			 | regex_constants::egrep
			 | regex_constants::awk);
    }

    bool
    _M_is_grep() const
    { return _M_flags & (regex_constants::grep | regex_constants::egrep); }

    bool
    _M_is_awk() const
    { return _M_flags & regex_constants::awk; }

  protected:
    // TODO: Make them static in the next abi change.
    const std::pair<char, _TokenT> _M_token_tb