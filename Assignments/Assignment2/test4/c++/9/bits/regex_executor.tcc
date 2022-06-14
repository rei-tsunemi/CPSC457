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
 *  @file bits/regex_executor.tcc
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{regex}
 */

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

namespace __detail
{
  template<typename _BiIter, typename _Alloc, typename _TraitsT,
	   bool __dfs_mode>
    bool _Executor<_BiIter, _Alloc, _TraitsT, __dfs_mode>::
    _M_search()
    {
      if (_M_search_from_first())
	return true;
      if (_M_flags & regex_constants::match_continuous)
	return false;
      _M_flags |= regex_constants::match_prev_avail;
      while (_M_begin != _M_end)
	{
	  ++_M_begin;
	  if (_M_search_from_first())
	    return true;
	}
      return false;
    }

  // The _M_main function operates in different modes, DFS mode or BFS mode,
  // indicated by template parameter __dfs_mode, and dispatches to one of the
  // _M_main_dispatch overloads.
  //
  // ------------------------------------------------------------
  //
  // DFS mode:
  //
  // It applies a Depth-First-Search (aka backtracking) on given NFA and input
  // string.
  // At the very beginning the executor stands in the start state, then it
  // tries every possible state transition in current state recursively. Some
  // state transitions consume input string, say, a single-char-matcher or a
  // back-reference matcher; some don't, like assertion or other anchor nodes.
  // When the input is exhausted and/or the current state is an accepting
  // state, the whole executor returns true.
  //
  // TODO: This approach is exponentially slow for certain input.
  //       Try to compile the NFA to a DFA.
  //
  // Time complexity: \Omega(match_length), O(2^(_M_nfa.size()))
  // Space complexity: \theta(match_results.size() + match_length)
  //
  template<typename _BiIter, typename _Alloc, typename _TraitsT,
	   bool __dfs_mode>
    bool _Executor<_BiIter, _Alloc, _TraitsT, __dfs_mode>::
    _M_main_dispatch(_Match_mode __match_mode, __dfs)
    {
      _M_has_sol = false;
      *_M_states._M_get_sol_pos() = _BiIter();
      _M_cur_results = _M_results;
      _M_dfs(__match_mode, _M_states._M_start);
      return _M_has_sol;
    }

  // ------------------------------------------------------------
  //
  // BFS mode:
  //
  // Russ Cox's article (http://swtch.com/~rsc/regexp/regexp1.html)
  // explained this algorithm clearly.
  //
  // It first computes epsilon closure (states that can be achieved without
  // consuming characters) for every state that's still matching,
  // using the same DFS algorithm, but doesn't re-enter states (using
  // _M_states._M_visited to check), nor follow _S_opcode_match.
  //
  // Then apply DFS using every _S_opcode_match (in _M_states._M_match_queue)
  // as the start state.
  //
  // It significantly reduces potential duplicate states, so has a better
  // upper bound; but it requires more overhead.
  //
  // Time complexity: \Omega(match_length * match_results.size())
  //                  O(match_leng