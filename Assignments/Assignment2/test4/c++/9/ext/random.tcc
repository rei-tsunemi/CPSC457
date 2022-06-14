// Random number extensions -*- C++ -*-

// Copyright (C) 2012-2019 Free Software Foundation, Inc.
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

/** @file ext/random.tcc
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{ext/random}
 */

#ifndef _EXT_RANDOM_TCC
#define _EXT_RANDOM_TCC 1

#pragma GCC system_header

namespace __gnu_cxx _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

  template<typename _UIntType, size_t __m,
	   size_t __pos1, size_t __sl1, size_t __sl2,
	   size_t __sr1, size_t __sr2,
	   uint32_t __msk1, uint32_t __msk2,
	   uint32_t __msk3, uint32_t __msk4,
	   uint32_t __parity1, uint32_t __parity2,
	   uint32_t __parity3, uint32_t __parity4>
    void simd_fast_mersenne_twister_engine<_UIntType, __m,
					   __pos1, __sl1, __sl2, __sr1, __sr2,
					   __msk1, __msk2, __msk3, __msk4,
					   __parity1, __parity2, __parity3,
					   __parity4>::
    seed(_UIntType __seed)
    {
      _M_state32[0] = static_cast<uint32_t>(__seed);
      for (size_t __i = 1; __i < _M_nstate32; ++__i)
	_M_state32[__i] = (1812433253UL
			   * (_M_state32[__i - 1] ^ (_M_state32[__i - 1] >> 30))
			   + __i);
      _M_pos = state_size;
      _M_period_certification();
    }


  namespace {

    inline uint32_t _Func1(uint32_t __x)
    {
      return (__x ^ (__x >> 27)) * UINT32_C(1664525);
    }

    inline uint32_t _Func2(uint32_t __x)
    {
      return (__x ^ (__x >> 27)) * UINT32_C(1566083941);
    }

  }


  template<typename _UIntType, size_t __m,
	   size_t __pos1, size_t __sl1, size_t __sl2,
	   size_t __sr1, size_t __sr2,
	   uint32_t __msk1, uint32_t __msk2,
	   uint32_t __msk3, uint32_t __msk4,
	   uint32_t __parity1, uint32_t __parity2,
	   uint32_t __parity3, uint32_t __parity4>
    template<typename _Sseq>
      auto
      simd_fast_mersenne_twister_engine<_UIntType, __m,
					__pos1, __sl1, __sl2, __sr1, __sr2,
					__msk1, __msk2, __msk3, __msk4,
					__parity1, __parity2, __parity3,
					__parity4>::
      seed(_Sseq& __q)
      -> _If_seed_seq<_Sseq>
      {
	size_t __lag;

	if (_M_nstate32 >= 623)
	  __lag = 11;
	else if (_M_nstate32 >= 68)
	  __lag = 7;
	else if (_M_nstate32 >= 39)
	  __lag = 5;
	else
	  __lag = 3;
	const size_t __mid = (_M_nstate32 - __lag) / 2;

	std::fill(_M_state32, _M_state32 + _M_nstate32, UINT32_C(0x8b8b8b8b));
	uint32_t __arr[_M_nstate32];
	__q.generate(__arr + 0, __arr + _M_nstate32);

	uint32_t __r = _Func1(_M_state32[0] ^ _M_state32[__mid]
			      ^ _M_state32[_M_nstate32  - 1]);
	_M_state32[__mid] += __r;
	__r += _M_nstate32;
	_M_state32[__mid + __lag] += __r;
	_M_state32[0] = __r;

	for (size_t __i = 1, __j = 0; __j < _M_nstate32; ++__j)
	  {
	    __r = _Func1(_M_state32[__i]
			 ^ _M_state32[(__i + __mid) % _M_nstate32]
			 ^ _M_state32[(__i + _M_nstate32 - 1) % _M_nstate32]);
	    _M_state32[(__i + __mid) % _M_nstate32] += __r;
	    __r += __arr[__j] + __i;
	    _M_state32[(__i + __mid + __lag) % _M_nstate32] += __r;
	    _M_state32[__i] = __r;
	    __i = (__i + 1) % _M_nstate32;
	  }
	for (size_t __j = 0; __j < _M_nstate32