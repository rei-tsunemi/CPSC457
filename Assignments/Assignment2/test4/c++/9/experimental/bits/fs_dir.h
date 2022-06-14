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

/** @file experimental/bits/fs_dir.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{experimental/filesystem}
 */

#ifndef _GLIBCXX_EXPERIMENTAL_FS_DIR_H
#define _GLIBCXX_EXPERIMENTAL_FS_DIR_H 1

#if __cplusplus < 201103L
# include <bits/c++0x_warning.h>
#else
# include <typeinfo>
# include <ext/concurrence.h>
# include <bits/unique_ptr.h>
# include <bits/shared_ptr.h>

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

namespace experimental
{
namespace filesystem
{
inline namespace v1
{
  /**
   * @ingroup filesystem-ts
   * @{
   */

  class file_status
  {
  public:
    // constructors
    explicit
    file_status(file_type __ft = file_type::none,
	        perms __prms = perms::unknown) noexcept
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

  class directory_entry
  {
  public:
    // constructors and destructor
    directory_entry() noexcept = default;
    directory_entry(const directory_entry&) = default;
    directory_entry(directory_entry&&) noexcept = default;
    explicit directory_entry(const filesystem::path& __p) : _M_path(__p) { }
    ~directory_entry() = default;

    // modifiers
    directory_entry& operator=(const directory_entry&) = default;
    directory_entry& operator=(directory_entry&&) noexcept = default;

    void assign(const filesystem::path& __p) { _M_path = __p; }

    void
    replace_filename(const filesystem::path& __p)
    { _M_path = _M_path.parent_path() / __p; }

    // observers
    const filesystem::path&  path() const noexcept { return _M_path; }
    operator const filesystem::path&() const noexcept { return _M_path; }

    file_status
    status() const
    { return filesystem::status(_M_path); }

    file_status
    status(error_code& __ec) const noexcept
    { return filesystem::status(_M_path, __ec); }

    file_status
    symlink_status() const
    { return filesystem::symlink_status(_M_path); }

    file_status
    symlink_status(error_code& __ec) const noexcept
    { return filesystem::symlink_status(_M_path, __ec); }

    bool
    operator< (const directory_entry& __rhs) const noexcept
    { return _M_path < __rhs._M_path; }

    bool
    operator==(const directory_entry& __rhs) const noexcept
    { return _M_path == __rhs._M_path; }

    bool
    operator!=(const dir