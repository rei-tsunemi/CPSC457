/* Threads compatibility routines for libgcc2 and libobjc.  */
/* Compile this one with gcc.  */
/* Copyright (C) 1997-2019 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

Under Section 7 of GPL version 3, you are granted additional
permissions described in the GCC Runtime Library Exception, version
3.1, as published by the Free Software Foundation.

You should have received a copy of the GNU General Public License and
a copy of the GCC Runtime Library Exception along with this program;
see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
<http://www.gnu.org/licenses/>.  */

#ifndef _GLIBCXX_GCC_GTHR_POSIX_H
#define _GLIBCXX_GCC_GTHR_POSIX_H

/* POSIX threads specific definitions.
   Easy, since the interface is just one-to-one mapping.  */

#define __GTHREADS 1
#define __GTHREADS_CXX0X 1

#include <pthread.h>

#if ((defined(_LIBOBJC) || defined(_LIBOBJC_WEAK)) \
     || !defined(_GTHREAD_USE_MUTEX_TIMEDLOCK))
# include <unistd.h>
# if defined(_POSIX_TIMEOUTS) && _POSIX_TIMEOUTS >= 0
#  define _GTHREAD_USE_MUTEX_TIMEDLOCK 1
# else
#  define _GTHREAD_USE_MUTEX_TIMEDLOCK 0
# endif
#endif

typedef pthread_t __gthread_t;
typedef pthread_key_t __gthread_key_t;
typedef pthread_once_t __gthread_once_t;
typedef pthread_mutex_t __gthread_mutex_t;
typedef pthread_mutex_t __gthread_recursive_mutex_t;
typedef pthread_cond_t __gthread_cond_t;
typedef struct timespec __gthread_time_t;

/* POSIX like conditional variables are supported.  Please look at comments
   in gthr.h for details. */
#define __GTHREAD_HAS_COND	1

#define __GTHREAD_MUTEX_INIT PTHREAD_MUTEX_INITIALIZER
#define __GTHREAD_MUTEX_INIT_FUNCTION __gthread_mutex_init_function
#define __GTHREAD_ONCE_INIT PTHREAD_ONCE_INIT
#if defined(PTHREAD_RECURSIVE_MUTEX_INITIALIZER)
#define __GTHREAD_RECURSIVE_MUTEX_INIT PTHREAD_RECURSIVE_MUTEX_INITIALIZER
#elif defined(PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP)
#define __GTHREAD_RECURSIVE_MUTEX_INIT PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP
#else
#define __GTHREAD_RECURSIVE_MUTEX_INIT_FUNCTION __gthread_recursive_mutex_init_function
#endif
#define __GTHREAD_COND_INIT PTHREAD_COND_INITIALIZER
#define __GTHREAD_TIME_INIT {0,0}

#ifdef _GTHREAD_USE_MUTEX_INIT_FUNC
# undef __GTHREAD_MUTEX_INIT
#endif
#ifdef _GTHREAD_USE_RECURSIVE_MUTEX_INIT_FUNC
# undef __GTHREAD_RECURSIVE_MUTEX_INIT
# undef __GTHREAD_RECURSIVE_MUTEX_INIT_FUNCTION
# define __GTHREAD_RECURSIVE_MUTEX_INIT_FUNCTION __gthread_recursive_mutex_init_function
#endif
#ifdef _GTHREAD_USE_COND_INIT_FUNC
# undef __GTHREAD_COND_INIT
# define __GTHREAD_COND_INIT_FUNCTION __gthread_cond_init_function
#endif

#if __GXX_WEAK__ && _GLIBCXX_GTHREAD_USE_WEAK
# ifndef __gthrw_pragma
#  define __gthrw_pragma(pragma)
# endif
# define __gthrw2(name,name2,type) \
  static __typeof(type) name \
    __attribute__ ((__weakref__(#name2), __copy__ (type))); \
  __gthrw_pragma(weak type)
# define __gthrw_(name) __gthrw_ ## name
#else
# define __gthrw2(name,name2,type)
# define __gthrw_(name) name
#endif

/* Typically, __gthrw_foo is a weak reference to symbol foo.  */
#define __gthrw(name) __gthrw2(__gthrw_ ## name,name,name)

__gthrw(pthread_once)
__gthrw(pthread_getspecific)
__gthrw(pthread_setspecific)

__gthrw(pthread_create)
__gthrw(pthread_join)
__gthrw(pthread_equal)
__gthrw(pthread_self)
__gthrw(pthread_detach)
#ifndef __BIONIC__
__gthrw(pthread_cancel)
#endif
__gthrw(sched_yield)

__gthrw(pthread_mutex_lock)
__gthrw(pthread_mutex_trylock)
#if _GTHREAD_USE_MUTEX_TIMEDLOCK
__gthrw(pthread_mutex_timedlock)
#endif
__gthrw(pthread_mutex_unlock)
__gthrw(pthread_mutex_init)
__gthrw(pthread_mutex_destroy)

__gthrw(pthread_cond_init)
__gthrw(pthr