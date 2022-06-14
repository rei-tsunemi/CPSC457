// Filesystem directory utilities -*- C++ -*-

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

/** @file include/bits/fs_dir.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{filesystem}
 */

#ifndef _GLIBCXX_FS_DIR_H
#define _GLIBCXX_FS_DIR_H 1

#if __cplusplus >= 201703L
# include <typeinfo>
# include <ext/concurrence.h>
# include <bits/unique_ptr.h>
# include <bits/shared_ptr.h>

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

namespace filesystem
{
  /**
   * @ingroup filesystem
   * @{
   */

  class file_status
  {
  public:
    // constructors and destructor
    file_status() noexcept : file_status(file_type::none) {}

    explicit
    file_status(file_type __ft, perms __prms = perms::unknown) noexcept
    : _M_type(__ft), _M_perms(__prms) { }

    file_status(const file_status&) noexcept = default;
    file_status(file_status&&) noexcept = default;
    ~file_status() = default;

    file_status& operator=(const file_status&) noexcept = default;
    file_status& operator=(file_status&&) noexcept = default;

    // observers
    file_type  type() const noexcept { return _M_type; }
    perms      permissions() const noexcept { return _M_perms; }

    // modifiers
    void       type(file_type __ft) noexcept { _M_type = __ft; }
    void       permissions(perms __prms) noexcept { _M_perms = __prms; }

  private:
    file_type	_M_type;
    perms	_M_perms;
  };

_GLIBCXX_BEGIN_NAMESPACE_CXX11

  struct _Dir;
  class directory_iterator;
  class recursive_directory_iterator;

  class directory_entry
  {
  public:
    // constructors and destructor
    directory_entry() noexcept = default;
    directory_entry(const directory_entry&) = default;
    directory_entry(directory_entry&&) noexcept = default;

    explicit
    directory_entry(const filesystem::path& __p)
    : _M_path(__p)
    { refresh(); }

    directory_entry(const filesystem::path& __p, error_code& __ec)
    : _M_path(__p)
    {
      refresh(__ec);
      if (__ec)
	_M_path.clear();
    }

    ~directory_entry() = default;

    // modifiers
    directory_entry& operator=(const directory_entry&) = default;
    directory_entry& operator=(directory_entry&&) noexcept = default;

    void
    assign(const filesystem::path& __p)
    {
      _M_path = __p;
      refresh();
    }

    void
    assign(const filesystem::path& __p, error_code& __ec)
    {
      _M_path = __p;
      refresh(__ec);
    }

    void
    replace_filename(const filesystem::path& __p)
    {
      _M_path.replace_filename(__p);
      refresh();
    }

    void
    replace_filename(const filesystem::path& __p, error_code& __ec)
    {
      _M_path.replace_filename(__p);
      refresh(__ec);
    }

    void
    refresh()
    { _M_type = symlink_status().type(); }

    void
    refresh(error_code& __ec) noexcept
    { _M_type = symlink_status(__ec).type(); }

    // observers
    const filesystem::path& path() const noexcept { return _M_path; }
    operator const filesystem::path& () const noexcept { return _M_path; }

  