// decimal classes -*- C++ -*-

// Copyright (C) 2009-2019 Free Software Foundation, Inc.

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

/** @file decimal/decimal.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{decimal}
 */

// ISO/IEC TR 24733
// Written by Janis Johnson <janis187@us.ibm.com>

#ifndef _GLIBCXX_DECIMAL_IMPL
#define _GLIBCXX_DECIMAL_IMPL 1

#pragma GCC system_header

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

namespace decimal
{
  // ISO/IEC TR 24733  3.2.[234].1  Construct/copy/destroy.

  inline decimal32::decimal32(decimal64 __r)	: __val(__r.__getval()) {}
  inline decimal32::decimal32(decimal128 __r)	: __val(__r.__getval()) {}
  inline decimal64::decimal64(decimal32 __r)	: __val(__r.__getval()) {}
  inline decimal64::decimal64(decimal128 __r)	: __val(__r.__getval()) {}
  inline decimal128::decimal128(decimal32 __r)	: __val(__r.__getval()) {}
  inline decimal128::decimal128(decimal64 __r)	: __val(__r.__getval()) {}

  // ISO/IEC TR 24733  3.2.[234].6  Compound assignment.

#define _DEFINE_DECIMAL_COMPOUND_ASSIGNMENT_DEC(_Op1, _Op2, _T1, _T2)	 \
  inline _T1& _T1::operator _Op1(_T2 __rhs)				 \
  {									 \
    __setval(__getval() _Op2 __rhs.__getval());				 \
    return *this;							 \
  }

#define _DEFINE_DECIMAL_COMPOUND_ASSIGNMENT_INT(_Op1, _Op2, _T1, _T2)	 \
  inline _T1& _T1::operator _Op1(_T2 __rhs)				 \
  {									 \
    __setval(__getval() _Op2 __rhs);					 \
    return *this;							 \
  }

#define _DEFINE_DECIMAL_COMPOUND_ASSIGNMENTS(_Op1, _Op2, _T1)		 \
  _DEFINE_DECIMAL_COMPOUND_ASSIGNMENT_DEC(_Op1, _Op2, _T1, decimal32)	 \
  _DEFINE_DECIMAL_COMPOUND_ASSIGNMENT_DEC(_Op1, _Op2, _T1, decimal64)	 \
  _DEFINE_DECIMAL_COMPOUND_ASSIGNMENT_DEC(_Op1, _Op2, _T1, decimal128)	 \
  _DEFINE_DECIMAL_COMPOUND_ASSIGNMENT_INT(_Op1, _Op2, _T1, int)		 \
  _DEFINE_DECIMAL_COMPOUND_ASSIGNMENT_INT(_Op1, _Op2, _T1, unsigned int) \
  _DEFINE_DECIMAL_COMPOUND_ASSIGNMENT_INT(_Op1, _Op2, _T1, long)	 \
  _DEFINE_DECIMAL_COMPOUND_ASSIGNMENT_INT(_Op1, _Op2, _T1, unsigned long)\
  _DEFINE_DECIMAL_COMPOUND_ASSIGNMENT_INT(_Op1, _Op2, _T1, long long)	 \
  _DEFINE_DECIMAL_COMPOUND_ASSIGNMENT_INT(_Op1, _Op2, _T1, unsigned long long)

  _DEFINE_DECIMAL_COMPOUND_ASSIGNMENTS(+=, +, decimal32)
  _DEFINE_DECIMAL_COMPOUND_ASSIGNMENTS(-=, -, decimal32)
  _DEFINE_DECIMAL_COMPOUND_ASSIGNMENTS(*=, *, decimal32)
  _DEFINE_DECIMAL_COMPOUND_ASSIGNMENTS(/=, /, decimal32)

  _DEFINE_DECIMAL_COMPOUND_ASSIGNMENTS(+=, +, decimal64)
  _DEFINE_DECIMAL_COMPOUND_ASSIGNMENTS(-=, -, decimal64)
  _DEFINE_DECIMAL_COMPOUND_ASSIGNMENTS(*=, *, decimal64)
  _DEFINE_DECIMAL_COMPOUND_ASSIGNMENTS(/=, /, decimal64)

  _DEFINE_DECIMAL_COMPOUND_ASSIGNMENTS(+=, +, decimal128)
  _DEFINE_DECIMAL_COMPOUND_ASSIGNMENTS(-=, -, decimal128)
  _DEFINE_DECIMAL_COMPOUND_ASSIGNMENTS(*=, *, decimal128)
  _DEFINE_DECIMAL_COMPOUND_ASSIGNMENTS(/=, /, decimal128)

#undef _DEFINE_DECIMAL_COMPOUND_ASSIGNMENT_DEC
#undef _DEFINE_DECIMAL_COMPOUND_ASSIGNMENT_INT
#undef _DEFINE_DECIMAL_COMPOUND_ASSIGNMENTS

  // Extension: Conversion to integral type.

  in