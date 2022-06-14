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
 *  @file bits/regex_automaton.tcc
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{regex}
 */

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

namespace __detail
{
#ifdef _GLIBCXX_DEBUG
  inline std::ostream&
  _State_base::_M_print(std::ostream& ostr) const
  {
    switch (_M_opcode)
    {
      case _S_opcode_alternative:
      case _S_opcode_repeat:
	ostr << "alt next=" << _M_next << " alt=" << _M_alt;
	break;
      case _S_opcode_subexpr_begin:
	ostr << "subexpr begin next=" << _M_next << " index=" << _M_subexpr;
	break;
      case _S_opcode_subexpr_end:
	ostr << "subexpr end next=" << _M_next << " index=" << _M_subexpr;
	break;
      case _S_opcode_backref:
	ostr << "backref next=" << _M_next << " index=" << _M_backref_index;
	break;
      case _S_opcode_match:
	ostr << "match next=" << _M_next;
	break;
      case _S_opcode_accept:
	ostr << "accept next=" << _M_next;
	break;
      default:
	ostr << "unknown next=" << _M_next;
	break;
    }
    return ostr;
  }

  // Prints graphviz dot commands for state.
  inline std::ostream&
  _State_base::_M_dot(std::ostream& __ostr, _StateIdT __id) const
  {
    switch (_M_opcode)
    {
      case _S_opcode_alternative:
      case _S_opcode_repeat:
	__ostr << __id << " [label=\"" << __id << "\\nALT\"];\n"
	       << __id << " -> " << _M_next
	       << " [label=\"next\", tailport=\"s\"];\n"
	       << __id << " -> " << _M_alt
	       << " [label=\"alt\", tailport=\"n\"];\n";
	break;
      case _S_opcode_backref:
	__ostr << __id << " [label=\"" << __id << "\\nBACKREF "
	       << _M_subexpr << "\"];\n"
	       << __id << " -> " << _M_next << " [label=\"<match>\"];\n";
	break;
      case _S_opcode_line_begin_assertion:
	__ostr << __id << " [label=\"" << __id << "\\nLINE_BEGIN \"];\n"
	       << __id << " -> " << _M_next << " [label=\"epsilon\"];\n";
	break;
      case _S_opcode_line_end_assertion:
	__ostr << __id << " [label=\"" << __id << "\\nLINE_END \"];\n"
	       << __id << " -> " << _M_next << " [label=\"epsilon\"];\n";
	break;
      case _S_opcode_word_boundary:
	__ostr << __id << " [label=\"" << __id << "\\nWORD_BOUNDRY "
	       << _M_neg << "\"];\n"
	       << __id << " -> " << _M_next << " [label=\"epsilon\"];\n";
	break;
      case _S_opcode_subexpr_lookahead:
	__ostr << __id << " [label=\"" << __id << "\\nLOOK_AHEAD\"];\n"
	       << __id << " -> " << _M_next
	       << " [label=\"epsilon\", tailport=\"s\"];\n"
	       << __id << " -> " << _M_alt
	       << " [label=\"<assert>\", tailport=\"n\"];\n";
	break;
      case _S_opcode_subexpr_begin:
	__ostr << __id << " [label=\"" << __id << "\\nSBEGIN "
	       << _M_subexpr << "\"];\n"
	       << __id << " -> " << _M_next << " [label=\"epsilon\"];\n";
	break;
      case _S_opcode_subexpr_end:
	__ostr << __id << " [label=\"" << __id << "\\nSEND "
	       << _M_subexpr << "\"];\n"
	       << __id << " -> " << _M_next << " [label=\"epsilon\"];\n";
	break;
    