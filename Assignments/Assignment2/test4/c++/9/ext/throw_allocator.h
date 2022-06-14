// -*- C++ -*-

// Copyright (C) 2005-2019 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License as published by the Free Software
// Foundation; either version 3, or (at your option) any later
// version.

// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

// Copyright (C) 2004 Ami Tavory and Vladimir Dreizin, IBM-HRL.

// Permission to use, copy, modify, sell, and distribute this software
// is hereby granted without fee, provided that the above copyright
// notice appears in all copies, and that both that copyright notice
// and this permission notice appear in supporting documentation. None
// of the above authors, nor IBM Haifa Research Laboratories, make any
// representation about the suitability of this software for any
// purpose. It is provided "as is" without express or implied
// warranty.

/** @file ext/throw_allocator.h
 *  This file is a GNU extension to the Standard C++ Library.
 *
 *  Contains two exception-generating types (throw_value, throw_allocator)
 *  intended to be used as value and allocator types while testing
 *  exception safety in templatized containers and algorithms. The
 *  allocator has additional log and debug features. The exception
 *  generated is of type forced_exception_error.
 */

#ifndef _THROW_ALLOCATOR_H
#define _THROW_ALLOCATOR_H 1

#include <cmath>
#include <ctime>
#include <map>
#include <string>
#include <ostream>
#include <stdexcept>
#include <utility>
#include <bits/functexcept.h>
#include <bits/move.h>
#if __cplusplus >= 201103L
# include <functional>
# include <random>
#else
# include <tr1/functional>
# include <tr1/random>
#endif

namespace __gnu_cxx _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  /**
   *  @brief Thown by exception safety machinery.
   *  @ingroup exceptions
   */
  struct forced_error : public std::exception
  { };

  // Substitute for forced_error object when -fno-exceptions.
  inline void
  __throw_forced_error()
  { _GLIBCXX_THROW_OR_ABORT(forced_error()); }

  /**
   *  @brief Base class for checking address and label information
   *  about allocations. Create a std::map between the allocated
   *  address (void*) and a datum for annotations, which are a pair of
   *  numbers corresponding to label and allocated size.
   */
  struct annotate_base
  {
  private:
    typedef std::pair<size_t, size_t>		data_type;
    typedef std::map<void*, data_type>		map_alloc_type;
    typedef map_alloc_type::value_type		entry_type;
    typedef map_alloc_type::const_iterator	const_iterator;
    typedef map_alloc_type::const_reference	const_reference;
#if __cplusplus >= 201103L
    typedef std::map<void*, size_t>		map_construct_type;
#endif

  public:
    annotate_base()
    {
      label();
      map_alloc();
    }

    static void
    set_label(size_t l)
    { label() = l; }

    static size_t
    get_label()
    { return label(); }

    void
    insert(void* p, size_t size)
    {
      entry_type entry = make_entry(p, size);
      if (!p)
	{
	  std::string error("annotate_base::insert null insert!\n");
	  log_to_string(error, entry);
	  std::__throw_logic_error(error.c_str());
	}

      std::pair<map_alloc_type::iterator, bool> inserted
	= map_alloc().insert(entry);
      if (!inserted.second)
	{
	  std::string error("annotate_base::insert double insert!\n");