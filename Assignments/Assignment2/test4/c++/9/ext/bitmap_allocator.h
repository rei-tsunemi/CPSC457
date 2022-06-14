// Bitmap Allocator. -*- C++ -*-

// Copyright (C) 2004-2019 Free Software Foundation, Inc.
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

/** @file ext/bitmap_allocator.h
 *  This file is a GNU extension to the Standard C++ Library.
 */

#ifndef _BITMAP_ALLOCATOR_H
#define _BITMAP_ALLOCATOR_H 1

#include <utility> // For std::pair.
#include <bits/functexcept.h> // For __throw_bad_alloc().
#include <functional> // For greater_equal, and less_equal.
#include <new> // For operator new.
#include <debug/debug.h> // _GLIBCXX_DEBUG_ASSERT
#include <ext/concurrence.h>
#include <bits/move.h>

/** @brief The constant in the expression below is the alignment
 * required in bytes.
 */
#define _BALLOC_ALIGN_BYTES 8

namespace __gnu_cxx _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  using std::size_t;
  using std::ptrdiff_t;

  namespace __detail
  {
    /** @class  __mini_vector bitmap_allocator.h bitmap_allocator.h
     *
     *  @brief  __mini_vector<> is a stripped down version of the
     *  full-fledged std::vector<>.
     *
     *  It is to be used only for built-in types or PODs. Notable
     *  differences are:
     * 
     *  1. Not all accessor functions are present.
     *  2. Used ONLY for PODs.
     *  3. No Allocator template argument. Uses ::operator new() to get
     *  memory, and ::operator delete() to free it.
     *  Caveat: The dtor does NOT free the memory allocated, so this a
     *  memory-leaking vector!
     */
    template<typename _Tp>
      class __mini_vector
      {
	__mini_vector(const __mini_vector&);
	__mini_vector& operator=(const __mini_vector&);

      public:
	typedef _Tp value_type;
	typedef _Tp* pointer;
	typedef _Tp& reference;
	typedef const _Tp& const_reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef pointer iterator;

      private:
	pointer _M_start;
	pointer _M_finish;
	pointer _M_end_of_storage;

	size_type
	_M_space_left() const throw()
	{ return _M_end_of_storage - _M_finish; }

	_GLIBCXX_NODISCARD pointer
	allocate(size_type __n)
	{ return static_cast<pointer>(::operator new(__n * sizeof(_Tp))); }

	void
	deallocate(pointer __p, size_type)
	{ ::operator delete(__p); }

      public:
	// Members used: size(), push_back(), pop_back(),
	// insert(iterator, const_reference), erase(iterator),
	// begin(), end(), back(), operator[].

	__mini_vector()
        : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }

	size_type
	size() const throw()
	{ return _M_finish - _M_start; }

	iterator
	begin() const throw()
	{ return this->_M_start; }

	iterator
	end() const throw()
	{ return this->_M_finish; }

	reference
	back() const throw()
	{ return *(this->end() - 1); }

	reference
	operator[](const size_type __pos) const throw()
	{ return this->_M_start[__pos]; }

	void
	insert(iterator __pos, const_reference __x);

	void
	push_back(const_reference __x)
	{
	  if (this->_M_space_left())
	    {
	      *this->end() = __x;
	      ++this->_M_finish;
	    }
	  else
	    this->insert(this->end(), __x);
	}

	void
	pop_back() throw()
	{ --this->_M_finish; }

	void
	erase(iterator __pos) throw();

	void
	clear() throw()
	{ this->_M_fini