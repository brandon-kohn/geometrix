//
//! Copyright © 2013
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_TEST_HPP
#define GEOMETRIX_TEST_HPP

#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/preprocessor.hpp>

#define GEOMETRIX_CHECK_SEQUENCE_CLOSE_3D(v, r, t)                          \
    BOOST_CHECK( geometrix::equals_within_absolute_tolerance(geometrix::get<0>(v), BOOST_PP_TUPLE_ELEM(3, 0, r), t )); \
    BOOST_CHECK( geometrix::equals_within_absolute_tolerance(geometrix::get<1>(v), BOOST_PP_TUPLE_ELEM(3, 1, r), t )); \
    BOOST_CHECK( geometrix::equals_within_absolute_tolerance(geometrix::get<2>(v), BOOST_PP_TUPLE_ELEM(3, 2, r), t )); \
    /***/

#define GEOMETRIX_CHECK_SEQUENCE_CLOSE_2D(v, r, t)                          \
    BOOST_CHECK( geometrix::equals_within_absolute_tolerance(geometrix::get<0>(v), BOOST_PP_TUPLE_ELEM(3, 0, r), t )); \
    BOOST_CHECK( geometrix::equals_within_absolute_tolerance(geometrix::get<1>(v), BOOST_PP_TUPLE_ELEM(3, 1, r), t )); \
    /***/

#endif
