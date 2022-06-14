// Class filesystem::path -*- C++ -*-

// Copyright (C) 2014-2019 Free Software Foundation, Inc.
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

/** @file include/bits/fs_path.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{filesystem}
 */

#ifndef _GLIBCXX_FS_PATH_H
#define _GLIBCXX_FS_PATH_H 1

#if __cplusplus >= 201703L

#include <utility>
#include <type_traits>
#include <locale>
#include <iosfwd>
#include <iomanip>
#include <codecvt>
#include <string_view>
#include <system_error>
#include <bits/stl_algobase.h>
#include <bits/locale_conv.h>
#include <ext/concurrence.h>
#include <bits/shared_ptr.h>
#include <bits/unique_ptr.h>

#if defined(_WIN32) && !defined(__CYGWIN__)
# define _GLIBCXX_FILESYSTEM_IS_WINDOWS 1
# include <algorithm>
#endif

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

namespace filesystem
{
_GLIBCXX_BEGIN_NAMESPACE_CXX11

  /**
   * @ingroup filesystem
   * @{
   */

  /// A filesystem path.
  class path
  {
    template<typename _CharT, typename _Ch = remove_const_t<_CharT>>
      using __is_encoded_char
	= __or_<is_same<_Ch, char>,
#ifdef _GLIBCXX_USE_CHAR8_T
		is_same<_Ch, char8_t>,
#endif
		is_same<_Ch, wchar_t>,
		is_same<_Ch, char16_t>,
		is_same<_Ch, char32_t>>;

    template<typename _Iter,
	     typename _Iter_traits = std::iterator_traits<_Iter>>
      using __is_path_iter_src
	= __and_<__is_encoded_char<typename _Iter_traits::value_type>,
		 std::is_base_of<std::input_iterator_tag,
				 typename _Iter_traits::iterator_category>>;

    template<typename _Iter>
      static __is_path_iter_src<_Iter>
      __is_path_src(_Iter, int);

    template<typename _CharT, typename _Traits, typename _Alloc>
      static __is_encoded_char<_CharT>
      __is_path_src(const basic_string<_CharT, _Traits, _Alloc>&, int);

    template<typename _CharT, typename _Traits>
      static __is_encoded_char<_CharT>
      __is_path_src(const basic_string_view<_CharT, _Traits>&, int);

    template<typename _Unknown>
      static std::false_type
      __is_path_src(const _Unknown&, ...);

    template<typename _Tp1, typename _Tp2>
      struct __constructible_from;

    template<typename _Iter>
      struct __constructible_from<_Iter, _Iter>
      : __is_path_iter_src<_Iter>
      { };

    template<typename _Source>
      struct __constructible_from<_Source, void>
      : decltype(__is_path_src(std::declval<_Source>(), 0))
      { };

    template<typename _Tp1, typename _Tp2 = void>
      using _Path = typename
	std::enable_if<__and_<__not_<is_same<remove_cv_t<_Tp1>, path>>,
			      __not_<is_void<remove_pointer_t<_Tp1>>>,
			      __constructible_from<_Tp1, _Tp2>>::value,
		       path>::type;

    template<typename _Source>
      static _Source
      _S_range_begin(_Source __begin) { return __begin; }

    struct __null_terminated { };

    template<typename _Source>
      static __null_terminated
      _S_range_end(_Source) { return {}; }

    template<typename _CharT, typename _Traits, typename _Alloc>
      static const _CharT*
      _S_range_begin(const basic_string<_CharT, _Traits, _