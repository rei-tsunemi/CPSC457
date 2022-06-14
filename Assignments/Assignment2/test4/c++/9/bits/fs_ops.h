// Filesystem operational functions -*- C++ -*-

// Copyright (C) 2014-2019 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your __option)
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

/** @file include/bits/fs_fwd.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{filesystem}
 */

#ifndef _GLIBCXX_FS_OPS_H
#define _GLIBCXX_FS_OPS_H 1

#if __cplusplus >= 201703L

#include <cstdint>

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

namespace filesystem
{
  /**
   * @ingroup filesystem
   * @{
   */

  path absolute(const path& __p);
  path absolute(const path& __p, error_code& __ec);

  path canonical(const path& __p);
  path canonical(const path& __p, error_code& __ec);

  inline void
  copy(const path& __from, const path& __to)
  { copy(__from, __to, copy_options::none); }

  inline void
  copy(const path& __from, const path& __to, error_code& __ec)
  { copy(__from, __to, copy_options::none, __ec); }

  void copy(const path& __from, const path& __to, copy_options __options);
  void copy(const path& __from, const path& __to, copy_options __options,
	    error_code& __ec);

  inline bool
  copy_file(const path& __from, const path& __to)
  { return copy_file(__from, __to, copy_options::none); }

  inline bool
  copy_file(const path& __from, const path& __to, error_code& __ec)
  { return copy_file(__from, __to, copy_options::none, __ec); }

  bool copy_file(const path& __from, const path& __to, copy_options __option);
  bool copy_file(const path& __from, const path& __to, copy_options __option,
		 error_code& __ec);

  void copy_symlink(const path& __existing_symlink, const path& __new_symlink);
  void copy_symlink(const path& __existing_symlink, const path& __new_symlink,
		    error_code& __ec) noexcept;

  bool create_directories(const path& __p);
  bool create_directories(const path& __p, error_code& __ec);

  bool create_directory(const path& __p);
  bool create_directory(const path& __p, error_code& __ec) noexcept;

  bool create_directory(const path& __p, const path& attributes);
  bool create_directory(const path& __p, const path& attributes,
			error_code& __ec) noexcept;

  void create_directory_symlink(const path& __to, const path& __new_symlink);
  void create_directory_symlink(const path& __to, const path& __new_symlink,
				error_code& __ec) noexcept;

  void create_hard_link(const path& __to, const path& __new_hard_link);
  void create_hard_link(const path& __to, const path& __new_hard_link,
			error_code& __ec) noexcept;

  void create_symlink(const path& __to, const path& __new_symlink);
  void create_symlink(const path& __to, const path& __new_symlink,
		      error_code& __ec) noexcept;

  path current_path();
  path current_path(error_code& __ec);
  void current_path(const path& __p);
  void current_path(const path& __p, error_code& __ec) noexcept;

  bool
  equivalent(const path& __p1, const path& __p2);

  bool
  equivalent(const path& __p1, const path& __p2, error_code& __ec) noexcept;

  inline bool
  exists(file_status __s) noexcept
  { return status_known(__s) && __s.type() != file_type::not_found; }

  inli