// Locale support (codecvt) -*- C++ -*-

// Copyright (C) 2000-2019 Free Software Foundation, Inc.
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

/** @file bits/codecvt.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{locale}
 */

//
// ISO C++ 14882: 22.2.1.5 Template class codecvt
//

// Written by Benjamin Kosnik <bkoz@redhat.com>

#ifndef _CODECVT_H
#define _CODECVT_H 1

#pragma GCC system_header

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  /// Empty base class for codecvt facet [22.2.1.5].
  class codecvt_base
  {
  public:
    enum result
    {
      ok,
      partial,
      error,
      noconv
    };
  };

  /**
   *  @brief  Common base for codecvt functions.
   *
   *  This template class provides implementations of the public functions
   *  that forward to the protected virtual functions.
   *
   *  This template also provides abstract stubs for the protected virtual
   *  functions.
  */
  template<typename _InternT, typename _ExternT, typename _StateT>
    class __codecvt_abstract_base
    : public locale::facet, public codecvt_base
    {
    public:
      // Types:
      typedef codecvt_base::result	result;
      typedef _InternT			intern_type;
      typedef _ExternT			extern_type;
      typedef _StateT			state_type;

      // 22.2.1.5.1 codecvt members
      /**
       *  @brief  Convert from internal to external character set.
       *
       *  Converts input string of intern_type to output string of
       *  extern_type.  This is analogous to wcsrtombs.  It does this by
       *  calling codecvt::do_out.
       *
       *  The source and destination character sets are determined by the
       *  facet's locale, internal and external types.
       *
       *  The characters in [from,from_end) are converted and written to
       *  [to,to_end).  from_next and to_next are set to point to the
       *  character following the last successfully converted character,
       *  respectively.  If the result needed no conversion, from_next and
       *  to_next are not affected.
       *
       *  The @a state argument should be initialized if the input is at the
       *  beginning and carried from a previous call if continuing
       *  conversion.  There are no guarantees about how @a state is used.
       *
       *  The result returned is a member of codecvt_base::result.  If
       *  all the input is converted, returns codecvt_base::ok.  If no
       *  conversion is necessary, returns codecvt_base::noconv.  If
       *  the input ends early or there is insufficient space in the
       *  output, returns codecvt_base::partial.  Otherwise the
       *  conversion failed and codecvt_base::error is returned.
       *
       *  @param  __state  Persistent conversion state data.
       *  @param  __from  Start of input.
       *  @param  __from_end  End of input.
       *  @param  __from_next  Returns start of unconverted data.
       *  @param  __to  Start of output buffer.
       *  @param  __to_end  End of output buffer.
       *  @param  __to_next  Returns start of unused output