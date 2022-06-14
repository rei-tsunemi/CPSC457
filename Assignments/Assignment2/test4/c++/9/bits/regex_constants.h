// class template regex -*- C++ -*-

// Copyright (C) 2010-2019 Free Software Foundation, Inc.
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
 *  @file bits/regex_constants.h
 *  @brief Constant definitions for the std regex library.
 *
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{regex}
 */

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

/**
 * @defgroup regex Regular Expressions
 *
 * A facility for performing regular expression pattern matching.
 * @{
 */

/**
 * @namespace std::regex_constants
 * @brief ISO C++-0x entities sub namespace for regex.
 */
namespace regex_constants
{
  /**
   * @name 5.1 Regular Expression Syntax Options
   */
  //@{
  enum __syntax_option
  {
    _S_icase,
    _S_nosubs,
    _S_optimize,
    _S_collate,
    _S_ECMAScript,
    _S_basic,
    _S_extended,
    _S_awk,
    _S_grep,
    _S_egrep,
    _S_polynomial,
    _S_syntax_last
  };

  /**
   * @brief This is a bitmask type indicating how to interpret the regex.
   *
   * The @c syntax_option_type is implementation defined but it is valid to
   * perform bitwise operations on these values and expect the right thing to
   * happen.
   *
   * A valid value of type syntax_option_type shall have exactly one of the
   * elements @c ECMAScript, @c basic, @c extended, @c awk, @c grep, @c egrep
   * %set.
   */
  enum syntax_option_type : unsigned int { };

  /**
   * Specifies that the matching of regular expressions against a character
   * sequence shall be performed without regard to case.
   */
  _GLIBCXX17_INLINE constexpr syntax_option_type icase =
    static_cast<syntax_option_type>(1 << _S_icase);

  /**
   * Specifies that when a regular expression is matched against a character
   * container sequence, no sub-expression matches are to be stored in the
   * supplied match_results structure.
   */
  _GLIBCXX17_INLINE constexpr syntax_option_type nosubs =
    static_cast<syntax_option_type>(1 << _S_nosubs);

  /**
   * Specifies that the regular expression engine should pay more attention to
   * the speed with which regular expressions are matched, and less to the
   * speed with which regular expression objects are constructed. Otherwise
   * it has no detectable effect on the program output.
   */
  _GLIBCXX17_INLINE constexpr syntax_option_type optimize =
    static_cast<syntax_option_type>(1 << _S_optimize);

  /**
   * Specifies that character ranges of the form [a-b] should be locale
   * sensitive.
   */
  _GLIBCXX17_INLINE constexpr syntax_option_type collate =
    static_cast<syntax_option_type>(1 << _S_collate);

  /**
   * Specifies that the grammar recognized by the regular expression engine is
   * that used by ECMAScript in ECMA-262 [Ecma International, ECMAScript
   * Language Specification, Standard Ecma-262, third edition, 1999], as
   * modified in section [28.13].  This grammar is similar to that defined
   * in the PERL scripting language but extended with elements found in the
   * POSIX regular expression grammar.
   */
  _GLIBCXX17_INLINE constexpr syntax_option_type ECMAScript =
    s