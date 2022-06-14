// Streambuf iterators

// Copyright (C) 1997-2019 Free Software Foundation, Inc.
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

/** @file bits/streambuf_iterator.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{iterator}
 */

#ifndef _STREAMBUF_ITERATOR_H
#define _STREAMBUF_ITERATOR_H 1

#pragma GCC system_header

#include <streambuf>
#include <debug/debug.h>

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  /**
   * @addtogroup iterators
   * @{
   */

  // 24.5.3 Template class istreambuf_iterator
  /// Provides input iterator semantics for streambufs.
  template<typename _CharT, typename _Traits>
    class istreambuf_iterator
    : public iterator<input_iterator_tag, _CharT, typename _Traits::off_type,
		      _CharT*,
#if __cplusplus >= 201103L
    // LWG 445.
		      _CharT>
#else
		      _CharT&>
#endif
    {
    public:
      // Types:
      //@{
      /// Public typedefs
      typedef _CharT					char_type;
      typedef _Traits					traits_type;
      typedef typename _Traits::int_type		int_type;
      typedef basic_streambuf<_CharT, _Traits>		streambuf_type;
      typedef basic_istream<_CharT, _Traits>		istream_type;
      //@}

      template<typename _CharT2>
	friend typename __gnu_cxx::__enable_if<__is_char<_CharT2>::__value,
				    ostreambuf_iterator<_CharT2> >::__type
	copy(istreambuf_iterator<_CharT2>, istreambuf_iterator<_CharT2>,
	     ostreambuf_iterator<_CharT2>);

      template<bool _IsMove, typename _CharT2>
	friend typename __gnu_cxx::__enable_if<__is_char<_CharT2>::__value,
					       _CharT2*>::__type
	__copy_move_a2(istreambuf_iterator<_CharT2>,
		       istreambuf_iterator<_CharT2>, _CharT2*);

      template<typename _CharT2>
	friend typename __gnu_cxx::__enable_if<__is_char<_CharT2>::__value,
				    istreambuf_iterator<_CharT2> >::__type
	find(istreambuf_iterator<_CharT2>, istreambuf_iterator<_CharT2>,
	     const _CharT2&);

      template<typename _CharT2, typename _Distance>
	friend typename __gnu_cxx::__enable_if<__is_char<_CharT2>::__value,
					       void>::__type
	advance(istreambuf_iterator<_CharT2>&, _Distance);

    private:
      // 24.5.3 istreambuf_iterator
      // p 1
      // If the end of stream is reached (streambuf_type::sgetc()
      // returns traits_type::eof()), the iterator becomes equal to
      // the "end of stream" iterator value.
      // NB: This implementation assumes the "end of stream" value
      // is EOF, or -1.
      mutable streambuf_type*	_M_sbuf;
      int_type			_M_c;

    public:
      ///  Construct end of input stream iterator.
      _GLIBCXX_CONSTEXPR istreambuf_iterator() _GLIBCXX_USE_NOEXCEPT
      : _M_sbuf(0), _M_c(traits_type::eof()) { }

#if __cplusplus >= 201103L
      istreambuf_iterator(const istreambuf_iterator&) noexcept = default;

      ~istreambuf_iterator() = default;
#endif

      ///  Construct start of input stream iterator.
      istreambuf_iterator(istream_type& __s) _GLIBCXX_USE_NOEXCEPT
      : _M_sbuf(__s.rdbuf()), _M_c(traits_type::eof()) { }

      ///  Construct start of streambuf iterator.
    