// random number generation -*- C++ -*-

// Copyright (C) 2009-2019 Free Software Foundation, Inc.
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
 * @file tr1/random.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{tr1/random}
 */

#ifndef _GLIBCXX_TR1_RANDOM_H
#define _GLIBCXX_TR1_RANDOM_H 1

#pragma GCC system_header

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

namespace tr1
{
  // [5.1] Random number generation

  /**
   * @addtogroup tr1_random Random Number Generation
   * A facility for generating random numbers on selected distributions.
   * @{
   */

  /*
   * Implementation-space details.
   */
  namespace __detail
  {
    template<typename _UIntType, int __w, 
	     bool = __w < std::numeric_limits<_UIntType>::digits>
      struct _Shift
      { static const _UIntType __value = 0; };

    template<typename _UIntType, int __w>
      struct _Shift<_UIntType, __w, true>
      { static const _UIntType __value = _UIntType(1) << __w; };

    template<typename _Tp, _Tp __a, _Tp __c, _Tp __m, bool>
      struct _Mod;

    // Dispatch based on modulus value to prevent divide-by-zero compile-time
    // errors when m == 0.
    template<typename _Tp, _Tp __a, _Tp __c, _Tp __m>
      inline _Tp
      __mod(_Tp __x)
      { return _Mod<_Tp, __a, __c, __m, __m == 0>::__calc(__x); }

    typedef __gnu_cxx::__conditional_type<(sizeof(unsigned) == 4),
		    unsigned, unsigned long>::__type _UInt32Type;

    /*
     * An adaptor class for converting the output of any Generator into
     * the input for a specific Distribution.
     */
    template<typename _Engine, typename _Distribution>
      struct _Adaptor
      { 
	typedef typename remove_reference<_Engine>::type _BEngine;
	typedef typename _BEngine::result_type           _Engine_result_type;
	typedef typename _Distribution::input_type       result_type;

      public:
	_Adaptor(const _Engine& __g)
	: _M_g(__g) { }

	result_type
	min() const
	{
	  result_type __return_value;
	  if (is_integral<_Engine_result_type>::value
	      && is_integral<result_type>::value)
	    __return_value = _M_g.min();
	  else
	    __return_value = result_type(0);
	  return __return_value;
	}

	result_type
	max() const
	{
	  result_type __return_value;
	  if (is_integral<_Engine_result_type>::value
	      && is_integral<result_type>::value)
	    __return_value = _M_g.max();
	  else if (!is_integral<result_type>::value)
	    __return_value = result_type(1);
	  else
	    __return_value = std::numeric_limits<result_type>::max() - 1;
	  return __return_value;
	}

	/*
	 * Converts a value generated by the adapted random number generator
	 * into a value in the input domain for the dependent random number
	 * distribution.
	 *
	 * Because the type traits are compile time constants only the
	 * appropriate clause of the if statements will actually be emitted
	 * by the compiler.
	 */
	result_type
	operator()()
	{
	  result_type __return_value;
	  if (is_integral<_Engine_result_type>::value
	      && is_integral<result_type>::value)
	    __return_value = _M_g();
	  else if 