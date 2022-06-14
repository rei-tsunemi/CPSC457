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
 *  @file bits/regex_compiler.tcc
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{regex}
 */

// FIXME make comments doxygen format.

/*
// This compiler refers to "Regular Expression Matching Can Be Simple And Fast"
// (http://swtch.com/~rsc/regexp/regexp1.html),
// but doesn't strictly follow it.
//
// When compiling, states are *chained* instead of tree- or graph-constructed.
// It's more like structured programs: there's if statement and loop statement.
//
// For alternative structure (say "a|b"), aka "if statement", two branches
// should be constructed. However, these two shall merge to an "end_tag" at
// the end of this operator:
//
//                branch1
//              /        \
// => begin_tag            end_tag =>
//              \        /
//                branch2
//
// This is the difference between this implementation and that in Russ's
// article.
//
// That's why we introduced dummy node here ------ "end_tag" is a dummy node.
// All dummy nodes will be eliminated at the end of compilation.
*/

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

namespace __detail
{
  template<typename _TraitsT>
    _Compiler<_TraitsT>::
    _Compiler(_IterT __b, _IterT __e,
	      const typename _TraitsT::locale_type& __loc, _FlagT __flags)
    : _M_flags((__flags
		& (regex_constants::ECMAScript
		   | regex_constants::basic
		   | regex_constants::extended
		   | regex_constants::grep
		   | regex_constants::egrep
		   | regex_constants::awk))
	       ? __flags
	       : __flags | regex_constants::ECMAScript),
      _M_scanner(__b, __e, _M_flags, __loc),
      _M_nfa(make_shared<_RegexT>(__loc, _M_flags)),
      _M_traits(_M_nfa->_M_traits),
      _M_ctype(std::use_facet<_CtypeT>(__loc))
    {
      _StateSeqT __r(*_M_nfa, _M_nfa->_M_start());
      __r._M_append(_M_nfa->_M_insert_subexpr_begin());
      this->_M_disjunction();
      if (!_M_match_token(_ScannerT::_S_token_eof))
	__throw_regex_error(regex_constants::error_paren);
      __r._M_append(_M_pop());
      __glibcxx_assert(_M_stack.empty());
      __r._M_append(_M_nfa->_M_insert_subexpr_end());
      __r._M_append(_M_nfa->_M_insert_accept());
      _M_nfa->_M_eliminate_dummy();
    }

  template<typename _TraitsT>
    void
    _Compiler<_TraitsT>::
    _M_disjunction()
    {
      this->_M_alternative();
      while (_M_match_token(_ScannerT::_S_token_or))
	{
	  _StateSeqT __alt1 = _M_pop();
	  this->_M_alternative();
	  _StateSeqT __alt2 = _M_pop();
	  auto __end = _M_nfa->_M_insert_dummy();
	  __alt1._M_append(__end);
	  __alt2._M_append(__end);
	  // __alt2 is state._M_next, __alt1 is state._M_alt. The executor
	  // executes _M_alt before _M_next, as well as executing left
	  // alternative before right one.
	  _M_stack.push(_StateSeqT(*_M_nfa,
				   _M_nfa->_M_insert_alt(
				     __alt2._M_start, __alt1._M_start, false),
				   __end));
	}
    }

  template<typename _TraitsT>
    void
   