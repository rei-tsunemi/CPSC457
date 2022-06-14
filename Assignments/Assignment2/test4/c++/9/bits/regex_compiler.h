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
 *  @file bits/regex_compiler.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{regex}
 */

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION
_GLIBCXX_BEGIN_NAMESPACE_CXX11

  template<typename>
    class regex_traits;

_GLIBCXX_END_NAMESPACE_CXX11

namespace __detail
{
  /**
   * @addtogroup regex-detail
   * @{
   */

  template<typename, bool, bool>
    struct _BracketMatcher;

  /**
   * @brief Builds an NFA from an input iterator range.
   *
   * The %_TraitsT type should fulfill requirements [28.3].
   */
  template<typename _TraitsT>
    class _Compiler
    {
    public:
      typedef typename _TraitsT::char_type        _CharT;
      typedef const _CharT*                       _IterT;
      typedef _NFA<_TraitsT>              	  _RegexT;
      typedef regex_constants::syntax_option_type _FlagT;

      _Compiler(_IterT __b, _IterT __e,
		const typename _TraitsT::locale_type& __traits, _FlagT __flags);

      shared_ptr<const _RegexT>
      _M_get_nfa()
      { return std::move(_M_nfa); }

    private:
      typedef _Scanner<_CharT>               _ScannerT;
      typedef typename _TraitsT::string_type _StringT;
      typedef typename _ScannerT::_TokenT    _TokenT;
      typedef _StateSeq<_TraitsT>            _StateSeqT;
      typedef std::stack<_StateSeqT>         _StackT;
      typedef std::ctype<_CharT>             _CtypeT;

      // accepts a specific token or returns false.
      bool
      _M_match_token(_TokenT __token);

      void
      _M_disjunction();

      void
      _M_alternative();

      bool
      _M_term();

      bool
      _M_assertion();

      bool
      _M_quantifier();

      bool
      _M_atom();

      bool
      _M_bracket_expression();

      template<bool __icase, bool __collate>
	void
	_M_insert_any_matcher_ecma();

      template<bool __icase, bool __collate>
	void
	_M_insert_any_matcher_posix();

      template<bool __icase, bool __collate>
	void
	_M_insert_char_matcher();

      template<bool __icase, bool __collate>
	void
	_M_insert_character_class_matcher();

      template<bool __icase, bool __collate>
	void
	_M_insert_bracket_matcher(bool __neg);

      // Returns true if successfully matched one term and should continue.
      // Returns false if the compiler should move on.
      template<bool __icase, bool __collate>
	bool
	_M_expression_term(pair<bool, _CharT>& __last_char,
			   _BracketMatcher<_TraitsT, __icase, __collate>&
			   __matcher);

      int
      _M_cur_int_value(int __radix);

      bool
      _M_try_char();

      _StateSeqT
      _M_pop()
      {
	auto ret = _M_stack.top();
	_M_stack.pop();
	return ret;
      }

      _FlagT              _M_flags;
      _ScannerT           _M_scanner;
      shared_ptr<_RegexT> _M_nfa;
      _StringT            _M_value;
      _StackT             _M_stack;
      const _TraitsT&     _M_traits;
      const _CtypeT&      _M_ctype;
    };

  template<typename _Tp>
    