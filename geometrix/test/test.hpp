//
//! Copyright © 2013
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_TEST_TEST_HPP
#define GEOMETRIX_TEST_TEST_HPP

#include <boost/preprocessor/tuple.hpp>

#define GEOMETRIX_CHECK_SEQUENCE_CLOSE_3D(v, r, t)                                                                     \
    BOOST_CHECK( geometrix::equals_within_absolute_tolerance(geometrix::get<0>(v), BOOST_PP_TUPLE_ELEM(3, 0, r), t )); \
    BOOST_CHECK( geometrix::equals_within_absolute_tolerance(geometrix::get<1>(v), BOOST_PP_TUPLE_ELEM(3, 1, r), t )); \
    BOOST_CHECK( geometrix::equals_within_absolute_tolerance(geometrix::get<2>(v), BOOST_PP_TUPLE_ELEM(3, 2, r), t )); \
/***/

#define GEOMETRIX_CHECK_SEQUENCE_CLOSE_2D(v, r, t)                                                                     \
    BOOST_CHECK( geometrix::equals_within_absolute_tolerance(geometrix::get<0>(v), BOOST_PP_TUPLE_ELEM(3, 0, r), t )); \
    BOOST_CHECK( geometrix::equals_within_absolute_tolerance(geometrix::get<1>(v), BOOST_PP_TUPLE_ELEM(3, 1, r), t )); \
/***/

//! Usage:
//! This file should only be included in implementation files. Using it in headers may
//! result in undefined behaviour if care is not made that conditional compiles occur consistently
//! between translation units which include the header.
//! 
//! \code
//! #define SOME_TEST 1 (enable SOME_TEST)
//! ...
//! #if GEOMETRIX_TEST_ENABLED(SOME_TEST)
//! ...//! Do test code for case.
//! #endif
//! \endcode

//! \def Macro to test if a conditional compilation unit (debug test) should be compiled.
#define GEOMETRIX_TEST_ENABLED(Flag) \
    GEOMETRIX_TESTS_ENABLED && Flag  \
/***/

//! By default debug tests are enabled. Add GEOMETRIX_DEBUG_TESTS_DISABLED to the 
//! defines on the project to disable all such tests within the project.
#ifndef GEOMETRIX_TESTS_ENABLED
    #define GEOMETRIX_TESTS_ENABLED 0
#else
    #undef GEOMETRIX_TESTS_ENABLED
    #define GEOMETRIX_TESTS_ENABLED 1
#endif

#endif//! GEOMETRIX_TEST_TEST_HPP
