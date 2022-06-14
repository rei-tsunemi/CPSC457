// Class template uniform_int_distribution -*- C++ -*-

// Copyright (C) 2009-2019 Free Software Foundation, Inc.
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
 * @file bits/uniform_int_dist.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{random}
 */

#ifndef _GLIBCXX_BITS_UNIFORM_INT_DIST_H
#define _GLIBCXX_BITS_UNIFORM_INT_DIST_H

#include <type_traits>
#include <limits>

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  namespace __detail
  {
    /* Determine whether number is a power of 2.  */
    template<typename _Tp>
      inline bool
      _Power_of_2(_Tp __x)
      {
	return ((__x - 1) & __x) == 0;
      }
  }

  /**
   * @brief Uniform discrete distribution for random numbers.
   * A discrete random distribution on the range @f$[min, max]@f$ with equal
   * probability throughout the range.
   */
  template<typename _IntType = int>
    class uniform_int_distribution
    {
      static_assert(std::is_integral<_IntType>::value,
		    "template argument must be an integral type");

    public:
      /** The type of the range of the distribution. */
      typedef _IntType result_type;
      /** Parameter type. */
      struct param_type
      {
	typedef uniform_int_distribution<_IntType> distribution_type;

	param_type() : param_type(0) { }

	explicit
	param_type(_IntType __a,
		   _IntType __b = numeric_limits<_IntType>::max())
	: _M_a(__a), _M_b(__b)
	{
	  __glibcxx_assert(_M_a <= _M_b);
	}

	result_type
	a() const
	{ return _M_a; }

	result_type
	b() const
	{ return _M_b; }

	friend bool
	operator==(const param_type& __p1, const param_type& __p2)
	{ return __p1._M_a == __p2._M_a && __p1._M_b == __p2._M_b; }

	friend bool
	operator!=(const param_type& __p1, const param_type& __p2)
	{ return !(__p1 == __p2); }

      private:
	_IntType _M_a;
	_IntType _M_b;
      };

    public:
      /**
       * @brief Constructs a uniform distribution object.
       */
      uniform_int_distribution() : uniform_int_distribution(0) { }

      /**
       * @brief Constructs a uniform distribution object.
       */
      explicit
      uniform_int_distribution(_IntType __a,
			       _IntType __b = numeric_limits<_IntType>::max())
      : _M_param(__a, __b)
      { }

      explicit
      uniform_int_distribution(const param_type& __p)
      : _M_param(__p)
      { }

      /**
       * @brief Resets the distribution state.
       *
       * Does nothing for the uniform integer distribution.
       */
      void
      reset() { }

      result_type
      a() const
      { return _M_param.a(); }

      result_type
      b() const
      { return _M_param.b(); }

      /**
       * @brief Returns the parameter set of the distribution.
       */
      param_type
      param() const
      { return _M_param; }

      /**
       * @brief Sets the parameter set of the distribution.
       * @param __param The new parameter set of the distribution.
       */
      void
      param(const param_type& __param)
      { _M_param = __param; }

      /**
       * @brief Returns the inclusive lower bound of the distribu