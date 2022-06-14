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

//
// ISO C++ 14882: 22.2.1.5 Template class codecvt
//

// Written by Benjamin Kosnik <bkoz@redhat.com>

/** @file ext/codecvt_specializations.h
 *  This file is a GNU extension to the Standard C++ Library.
 */

#ifndef _EXT_CODECVT_SPECIALIZATIONS_H
#define _EXT_CODECVT_SPECIALIZATIONS_H 1

#include <bits/c++config.h>
#include <locale>
#include <iconv.h>

namespace __gnu_cxx _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION
_GLIBCXX_BEGIN_NAMESPACE_CXX11

  /// Extension to use iconv for dealing with character encodings.
  // This includes conversions and comparisons between various character
  // sets.  This object encapsulates data that may need to be shared between
  // char_traits, codecvt and ctype.
  class encoding_state
  {
  public:
    // Types: 
    // NB: A conversion descriptor subsumes and enhances the
    // functionality of a simple state type such as mbstate_t.
    typedef iconv_t	descriptor_type;
    
  protected:
    // Name of internal character set encoding.
    std::string	       	_M_int_enc;

    // Name of external character set encoding.
    std::string  	_M_ext_enc;

    // Conversion descriptor between external encoding to internal encoding.
    descriptor_type	_M_in_desc;

    // Conversion descriptor between internal encoding to external encoding.
    descriptor_type	_M_out_desc;

    // The byte-order marker for the external encoding, if necessary.
    int			_M_ext_bom;

    // The byte-order marker for the internal encoding, if necessary.
    int			_M_int_bom;

    // Number of external bytes needed to construct one complete
    // character in the internal encoding.
    // NB: -1 indicates variable, or stateful, encodings.
    int 		_M_bytes;

  public:
    explicit 
    encoding_state() 
    : _M_in_desc(0), _M_out_desc(0), _M_ext_bom(0), _M_int_bom(0), _M_bytes(0)
    { }

    explicit 
    encoding_state(const char* __int, const char* __ext, 
		   int __ibom = 0, int __ebom = 0, int __bytes = 1)
    : _M_int_enc(__int), _M_ext_enc(__ext), _M_in_desc(0), _M_out_desc(0), 
      _M_ext_bom(__ebom), _M_int_bom(__ibom), _M_bytes(__bytes)
    { init(); }

    // 21.1.2 traits typedefs
    // p4
    // typedef STATE_T state_type
    // requires: state_type shall meet the requirements of
    // CopyConstructible types (20.1.3)
    // NB: This does not preserve the actual state of the conversion
    // descriptor member, but it does duplicate the encoding
    // information.
    encoding_state(const encoding_state& __obj) : _M_in_desc(0), _M_out_desc(0)
    { construct(__obj); }

    // Need assignment operator as well.
    encoding_state&
    operator=(const encoding_state& __obj)
    {
      construct(__obj);
      return *this;
    }

    ~encoding_state()
    { destroy(); } 

    bool
    good() const throw()
    { 
      const descriptor_type __err = (iconv_t)(-1);
      bool __test = _M_in_desc && _M_in_desc != __err; 
      __test &=  _M_out_desc && _M_out_desc != __err;
      return __test;
    }
    
    int
   