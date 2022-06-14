// Stream iterators

// Copyright (C) 2001-2019 Free Software Foundation, Inc.
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

/** @file bits/stream_iterator.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{iterator}
 */

#ifndef _STREAM_ITERATOR_H
#define _STREAM_ITERATOR_H 1

#pragma GCC system_header

#include <debug/debug.h>

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  /**
   * @addtogroup iterators
   * @{
   */

  /// Provides input iterator semantics for streams.
  template<typename _Tp, typename _CharT = char,
           typename _Traits = char_traits<_CharT>, typename _Dist = ptrdiff_t>
    class istream_iterator
    : public iterator<input_iterator_tag, _Tp, _Dist, const _Tp*, const _Tp&>
    {
    public:
      typedef _CharT                         char_type;
      typedef _Traits                        traits_type;
      typedef basic_istream<_CharT, _Traits> istream_type;

    private:
      istream_type*	_M_stream;
      _Tp		_M_value;
      bool		_M_ok;

    public:
      ///  Construct end of input stream iterator.
      _GLIBCXX_CONSTEXPR istream_iterator()
      : _M_stream(0), _M_value(), _M_ok(false) {}

      ///  Construct start of input stream iterator.
      istream_iterator(istream_type& __s)
      : _M_stream(std::__addressof(__s))
      { _M_read(); }

      istream_iterator(const istream_iterator& __obj)
      : _M_stream(__obj._M_stream), _M_value(__obj._M_value),
        _M_ok(__obj._M_ok)
      { }

#if __cplusplus >= 201103L
      istream_iterator& operator=(const istream_iterator&) = default;
#endif

      const _Tp&
      operator*() const
      {
	__glibcxx_requires_cond(_M_ok,
				_M_message(__gnu_debug::__msg_deref_istream)
				._M_iterator(*this));
	return _M_value;
      }

      const _Tp*
      operator->() const { return std::__addressof((operator*())); }

      istream_iterator&
      operator++()
      {
	__glibcxx_requires_cond(_M_ok,
				_M_message(__gnu_debug::__msg_inc_istream)
				._M_iterator(*this));
	_M_read();
	return *this;
      }

      istream_iterator
      operator++(int)
      {
	__glibcxx_requires_cond(_M_ok,
				_M_message(__gnu_debug::__msg_inc_istream)
				._M_iterator(*this));
	istream_iterator __tmp = *this;
	_M_read();
	return __tmp;
      }

      bool
      _M_equal(const istream_iterator& __x) const
      { return (_M_ok == __x._M_ok) && (!_M_ok || _M_stream == __x._M_stream); }

    private:
      void
      _M_read()
      {
	_M_ok = (_M_stream && *_M_stream) ? true : false;
	if (_M_ok)
	  {
	    *_M_stream >> _M_value;
	    _M_ok = *_M_stream ? true : false;
	  }
      }
    };

  ///  Return true if x and y are both end or not end, or x and y are the same.
  template<typename _Tp, typename _CharT, typename _Traits, typename _Dist>
    inline bool
    operator==(const istream_iterator<_Tp, _CharT, _Traits, _Dist>& __x,
	       const istream_iterator<_Tp, _CharT, _Traits, _Dist>& __y)
    { return __x._M_equal(__y); }

  ///  Return false if x and y are both end or not end, or x and y are the same.
  template <class _Tp, c