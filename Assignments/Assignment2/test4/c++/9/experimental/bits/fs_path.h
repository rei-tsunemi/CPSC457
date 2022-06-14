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

/** @file experimental/bits/fs_path.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{experimental/filesystem}
 */

#ifndef _GLIBCXX_EXPERIMENTAL_FS_PATH_H
#define _GLIBCXX_EXPERIMENTAL_FS_PATH_H 1

#if __cplusplus < 201103L
# include <bits/c++0x_warning.h>
#else

#include <utility>
#include <type_traits>
#include <vector>
#include <locale>
#include <iosfwd>
#include <codecvt>
#include <system_error>
#include <bits/stl_algobase.h>
#include <bits/quoted_string.h>
#include <bits/locale_conv.h>
#if __cplusplus == 201402L
# include <experimental/string_view>
#endif

#if defined(_WIN32) && !defined(__CYGWIN__)
# define _GLIBCXX_FILESYSTEM_IS_WINDOWS 1
# include <algorithm>
#endif

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

namespace experimental
{
namespace filesystem
{
inline namespace v1
{
_GLIBCXX_BEGIN_NAMESPACE_CXX11

#if __cplusplus == 201402L
  using std::experimental::basic_string_view;
#elif __cplusplus > 201402L
  using std::basic_string_view;
#endif

  /**
   * @ingroup filesystem-ts
   * @{
   */

  /// A filesystem path.
  class path
  {
    template<typename _CharT,
	     typename _Ch = typename remove_const<_CharT>::type>
      using __is_encoded_char
	= __or_<is_same<_Ch, char>,
		is_same<_Ch, wchar_t>,
#ifdef _GLIBCXX_USE_CHAR8_T
		is_same<_Ch, char8_t>,
#endif
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

#if __cplusplus >= 201402L
    template<typename _CharT, typename _Traits>
      static __is_encoded_char<_CharT>
      __is_path_src(const basic_string_view<_CharT, _Traits>&, int);
#endif

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

    template<typename _Tp1, typename _Tp2 = void,
	     typename _Tp1_nocv = typename remove_cv<_Tp1>::type,
	     typename _Tp1_noptr = typename remove_pointer<_Tp1>::type>
      using _Path = typename
	std::enable_if<__and_<__not_<is_same<_Tp1_nocv, path>>,
			      __not_<is_void<_Tp1_noptr>>,
			      __constructible_from<_Tp1, _Tp2>>::value,
		 