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
 *  @file bits/regex_automaton.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{regex}
 */

// This macro defines the maximal state number a NFA can have.
#ifndef _GLIBCXX_REGEX_STATE_LIMIT
#define _GLIBCXX_REGEX_STATE_LIMIT 100000
#endif

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

namespace __detail
{
  /**
   *  @defgroup regex-detail Base and Implementation Classes
   *  @ingroup regex
   *  @{
   */

  typedef long _StateIdT;
  static const _StateIdT _S_invalid_state_id  = -1;

  template<typename _CharT>
    using _Matcher = std::function<bool (_CharT)>;

  /// Operation codes that define the type of transitions within the base NFA
  /// that represents the regular expression.
  enum _Opcode : int
  {
      _S_opcode_unknown,
      _S_opcode_alternative,
      _S_opcode_repeat,
      _S_opcode_backref,
      _S_opcode_line_begin_assertion,
      _S_opcode_line_end_assertion,
      _S_opcode_word_boundary,
      _S_opcode_subexpr_lookahead,
      _S_opcode_subexpr_begin,
      _S_opcode_subexpr_end,
      _S_opcode_dummy,
      _S_opcode_match,
      _S_opcode_accept,
  };

  struct _State_base
  {
  protected:
    _Opcode      _M_opcode;           // type of outgoing transition

  public:
    _StateIdT    _M_next;             // outgoing transition
    union // Since they are mutually exclusive.
    {
      size_t _M_subexpr;        // for _S_opcode_subexpr_*
      size_t _M_backref_index;  // for _S_opcode_backref
      struct
      {
	// for _S_opcode_alternative, _S_opcode_repeat and
	// _S_opcode_subexpr_lookahead
	_StateIdT  _M_alt;
	// for _S_opcode_word_boundary or _S_opcode_subexpr_lookahead or
	// quantifiers (ungreedy if set true)
	bool       _M_neg;
      };
      // For _S_opcode_match
      __gnu_cxx::__aligned_membuf<_Matcher<char>> _M_matcher_storage;
    };

  protected:
    explicit _State_base(_Opcode __opcode)
    : _M_opcode(__opcode), _M_next(_S_invalid_state_id)
    { }

  public:
    bool
    _M_has_alt()
    {
      return _M_opcode == _S_opcode_alternative
	|| _M_opcode == _S_opcode_repeat
	|| _M_opcode == _S_opcode_subexpr_lookahead;
    }

#ifdef _GLIBCXX_DEBUG
    std::ostream&
    _M_print(std::ostream& ostr) const;

    // Prints graphviz dot commands for state.
    std::ostream&
    _M_dot(std::ostream& __ostr, _StateIdT __id) const;
#endif
  };

  template<typename _Char_type>
    struct _State : _State_base
    {
      typedef _Matcher<_Char_type> _MatcherT;
      static_assert(sizeof(_MatcherT) == sizeof(_Matcher<char>),
		    "std::function<bool(T)> has the same size as "
		    "std::function<bool(char)>");
      static_assert(alignof(_MatcherT) == alignof(_Matcher<char>),
		    "std::function<bool(T)> has the same alignment as "
		    "std::function<bool(char)>");

      explicit
      _State(_Opcode __opcode) : _State_base(__opcode)
      {
	if (_M_opcode() == _S_opcode_match)
	  new (this->_M_matcher_storage.