//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_ASSERT_HPP
#define GEOMETRIX_ASSERT_HPP

//! Fix for used typedef in boost (which is fixed in versions beyond 1.58 I think.)
#include <boost/static_assert.hpp>

#if (defined(__apple_build_version__) && (__apple_build_version__ >= 7000000))
#  ifdef BOOST_STATIC_ASSERT_UNUSED_ATTRIBUTE
#  undef BOOST_STATIC_ASSERT_UNUSED_ATTRIBUTE
#  endif
#  define BOOST_STATIC_ASSERT_UNUSED_ATTRIBUTE __attribute__( (unused) )
#else
#  ifdef BOOST_STATIC_ASSERT_UNUSED_ATTRIBUTE
#  undef BOOST_STATIC_ASSERT_UNUSED_ATTRIBUTE
#  endif
#  define BOOST_STATIC_ASSERT_UNUSED_ATTRIBUTE
#endif

#define GEOMETRIX_STATIC_ASSERT BOOST_STATIC_ASSERT

#endif//! GEOMETRIX_ASSERT_HPP
