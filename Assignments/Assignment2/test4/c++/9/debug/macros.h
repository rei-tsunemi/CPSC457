// Debugging support implementation -*- C++ -*-

// Copyright (C) 2003-2019 Free Software Foundation, Inc.
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

/** @file debug/macros.h
 *  This file is a GNU debug extension to the Standard C++ Library.
 */

#ifndef _GLIBCXX_DEBUG_MACROS_H
#define _GLIBCXX_DEBUG_MACROS_H 1

/**
 * Macros used by the implementation to verify certain
 * properties. These macros may only be used directly by the debug
 * wrappers. Note that these are macros (instead of the more obviously
 * @a correct choice of making them functions) because we need line and
 * file information at the call site, to minimize the distance between
 * the user error and where the error is reported.
 *
 */
#define _GLIBCXX_DEBUG_VERIFY_COND_AT(_Cond,_ErrMsg,_File,_Line,_Func)	\
  if (! (_Cond))							\
    __gnu_debug::_Error_formatter::_S_at(_File, _Line, _Func)		\
      ._ErrMsg._M_error()

#define _GLIBCXX_DEBUG_VERIFY_AT_F(_Cond,_ErrMsg,_File,_Line,_Func)	\
  do									\
  {									\
    _GLIBCXX_DEBUG_VERIFY_COND_AT(_Cond,_ErrMsg,_File,_Line,_Func);	\
  } while (false)

#define _GLIBCXX_DEBUG_VERIFY_AT(_Cond,_ErrMsg,_File,_Line)		\
  _GLIBCXX_DEBUG_VERIFY_AT_F(_Cond,_ErrMsg,_File,_Line,__PRETTY_FUNCTION__)

#define _GLIBCXX_DEBUG_VERIFY(_Cond,_ErrMsg)				\
  _GLIBCXX_DEBUG_VERIFY_AT_F(_Cond, _ErrMsg, __FILE__, __LINE__,	\
			     __PRETTY_FUNCTION__)

// Verify that [_First, _Last) forms a valid iterator range.
#define __glibcxx_check_valid_range(_First,_Last)			\
_GLIBCXX_DEBUG_VERIFY(__gnu_debug::__valid_range(_First, _Last),	\
		      _M_message(__gnu_debug::__msg_valid_range)	\
		      ._M_iterator(_First, #_First)			\
		      ._M_iterator(_Last, #_Last))

#define __glibcxx_check_valid_range_at(_First,_Last,_File,_Line,_Func)	\
_GLIBCXX_DEBUG_VERIFY_AT_F(__gnu_debug::__valid_range(_First, _Last),	\
			   _M_message(__gnu_debug::__msg_valid_range)	\
			   ._M_iterator(_First, #_First)		\
			   ._M_iterator(_Last, #_Last),			\
			   _File,_Line,_Func)

#define __glibcxx_check_valid_range2(_First,_Last,_Dist)		\
_GLIBCXX_DEBUG_VERIFY(__gnu_debug::__valid_range(_First, _Last, _Dist),	\
		      _M_message(__gnu_debug::__msg_valid_range)	\
		      ._M_iterator(_First, #_First)			\
		      ._M_iterator(_Last, #_Last))

#define __glibcxx_check_valid_constructor_range(_First,_Last)		\
  __gnu_debug::__check_valid_range(_First, _Last,			\
				   __FILE__, __LINE__, __PRETTY_FUNCTION__)

// Verify that [_First, _Last) forms a non-empty iterator range.
#define __glibcxx_check_non_empty_range(_First,_Last)			\
_GLIBCXX_DEBUG_VERIFY(_First != _Last,					\
		      _M_message(__gnu_debug::__msg_non_empty_range)	\
		      ._M_iterator(_First, #_First)			\
		      ._M_iterator(_Last, #_Last))

// Verify that [_First, _First + _Size) forms a valid range.
#define __glibcxx_check_can_increment(_First,_Size)			\
_GLIBCXX_DEBUG_VERIFY(__gnu_debug::__can_advance(_First, _Size),	\
		      _M_message(__gnu_debug::__msg_iter_subscript_oob)	\
		      ._M_iterator(_First, #_First)			\
		      ._M_integer(_Size, #_Size))

#define __glibcxx_check_can_increment_range(_First1,_Last1,_First2)	\
  do									\
  {									\
  