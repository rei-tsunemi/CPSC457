// Safe sequence/iterator base implementation  -*- C++ -*-

// Copyright (C) 2003-2019 Free Software Foundation, Inc.
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

/** @file debug/safe_base.h
 *  This file is a GNU debug extension to the Standard C++ Library.
 */

#ifndef _GLIBCXX_DEBUG_SAFE_BASE_H
#define _GLIBCXX_DEBUG_SAFE_BASE_H 1

#include <ext/concurrence.h>

namespace __gnu_debug
{
  class _Safe_sequence_base;

  /** \brief Basic functionality for a @a safe iterator.
   *
   *  The %_Safe_iterator_base base class implements the functionality
   *  of a safe iterator that is not specific to a particular iterator
   *  type. It contains a pointer back to the sequence it references
   *  along with iterator version information and pointers to form a
   *  doubly-linked list of iterators referenced by the container.
   *
   *  This class must not perform any operations that can throw an
   *  exception, or the exception guarantees of derived iterators will
   *  be broken.
   */
  class _Safe_iterator_base
  {
    friend class _Safe_sequence_base;

  public:
    /** The sequence this iterator references; may be NULL to indicate
	a singular iterator. */
    _Safe_sequence_base*	_M_sequence;

    /** The version number of this iterator. The sentinel value 0 is
     *  used to indicate an invalidated iterator (i.e., one that is
     *  singular because of an operation on the container). This
     *  version number must equal the version number in the sequence
     *  referenced by _M_sequence for the iterator to be
     *  non-singular.
     */
    unsigned int		_M_version;

    /** Pointer to the previous iterator in the sequence's list of
	iterators. Only valid when _M_sequence != NULL. */
    _Safe_iterator_base*	_M_prior;

    /** Pointer to the next iterator in the sequence's list of
	iterators. Only valid when _M_sequence != NULL. */
    _Safe_iterator_base*	_M_next;

  protected:
    /** Initializes the iterator and makes it singular. */
    _Safe_iterator_base()
    : _M_sequence(0), _M_version(0), _M_prior(0), _M_next(0)
    { }

    /** Initialize the iterator to reference the sequence pointed to
     *  by @p __seq. @p __constant is true when we are initializing a
     *  constant iterator, and false if it is a mutable iterator. Note
     *  that @p __seq may be NULL, in which case the iterator will be
     *  singular. Otherwise, the iterator will reference @p __seq and
     *  be nonsingular.
     */
    _Safe_iterator_base(const _Safe_sequence_base* __seq, bool __constant)
    : _M_sequence(0), _M_version(0), _M_prior(0), _M_next(0)
    { this->_M_attach(const_cast<_Safe_sequence_base*>(__seq), __constant); }

    /** Initializes the iterator to reference the same sequence that
	@p __x does. @p __constant is true if this is a constant
	iterator, and false if it is mutable. */
    _Safe_iterator_base(const _Safe_iterator_base& __x, bool __constant)
    : _M_sequence(0), _M_version(0), _M_prior(0), _M_next(0)
    { this->_M_attach(__x._M_sequence, __constant); }

    ~_Safe_iterator_base() { this->_M_detach(); }

    /** For use in _Safe_iterator. */
    __gnu_cxx::__mutex&
    _M_get_mutex(