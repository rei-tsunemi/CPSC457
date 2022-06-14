// Locale support -*- C++ -*-

// Copyright (C) 2007-2019 Free Software Foundation, Inc.
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

/** @file bits/locale_facets_nonio.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{locale}
 */

//
// ISO C++ 14882: 22.1  Locales
//

#ifndef _LOCALE_FACETS_NONIO_H
#define _LOCALE_FACETS_NONIO_H 1

#pragma GCC system_header

#include <ctime>	// For struct tm

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  /**
   *  @brief  Time format ordering data.
   *  @ingroup locales
   *
   *  This class provides an enum representing different orderings of
   *  time: day, month, and year.
  */
  class time_base
  {
  public:
    enum dateorder { no_order, dmy, mdy, ymd, ydm };
  };

  template<typename _CharT>
    struct __timepunct_cache : public locale::facet
    {
      // List of all known timezones, with GMT first.
      static const _CharT*		_S_timezones[14];

      const _CharT*			_M_date_format;
      const _CharT*			_M_date_era_format;
      const _CharT*			_M_time_format;
      const _CharT*			_M_time_era_format;
      const _CharT*			_M_date_time_format;
      const _CharT*			_M_date_time_era_format;
      const _CharT*			_M_am;
      const _CharT*			_M_pm;
      const _CharT*			_M_am_pm_format;

      // Day names, starting with "C"'s Sunday.
      const _CharT*			_M_day1;
      const _CharT*			_M_day2;
      const _CharT*			_M_day3;
      const _CharT*			_M_day4;
      const _CharT*			_M_day5;
      const _CharT*			_M_day6;
      const _CharT*			_M_day7;

      // Abbreviated day names, starting with "C"'s Sun.
      const _CharT*			_M_aday1;
      const _CharT*			_M_aday2;
      const _CharT*			_M_aday3;
      const _CharT*			_M_aday4;
      const _CharT*			_M_aday5;
      const _CharT*			_M_aday6;
      const _CharT*			_M_aday7;

      // Month names, starting with "C"'s January.
      const _CharT*			_M_month01;
      const _CharT*			_M_month02;
      const _CharT*			_M_month03;
      const _CharT*			_M_month04;
      const _CharT*			_M_month05;
      const _CharT*			_M_month06;
      const _CharT*			_M_month07;
      const _CharT*			_M_month08;
      const _CharT*			_M_month09;
      const _CharT*			_M_month10;
      const _CharT*			_M_month11;
      const _CharT*			_M_month12;

      // Abbreviated month names, starting with "C"'s Jan.
      const _CharT*			_M_amonth01;
      const _CharT*			_M_amonth02;
      const _CharT*			_M_amonth03;
      const _CharT*			_M_amonth04;
      const _CharT*			_M_amonth05;
      const _CharT*			_M_amonth06;
      const _CharT*			_M_amonth07;
      const _CharT*			_M_amonth08;
      const _CharT*			_M_amonth09;
      const _CharT*			_M_amonth10;
      const _CharT*			_M_amonth11;
      const _CharT*			_M_amonth12;

      bool				_M_allocated;

      __timepunct_cache(size_t __refs = 0) : facet(__refs),
      _M_date_format(0), _M_date_era_format(0), _M_time_format(0),
      _M_time_era_format(0), _M_date_time_format(0),
      _M_date_time_era_format(0), _M_am(0), _M_pm(0),
      _M_am_pm_format(0), _M_day1(0), _M_day2(