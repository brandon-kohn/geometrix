//
//! Copyright © 2008
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_PRODUCT_TESTS_HPP
#define _BOOST_GEOMETRY_PRODUCT_TESTS_HPP
#pragma once

#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include "../geometry/point.hpp"
#include "../geometry/vector.hpp"
#include "../geometry/utilities.hpp"
#include "../geometry/products.hpp"
#include "../geometry/indexed_sequence_operators.hpp"

BOOST_AUTO_TEST_CASE( TestProducts )
{
	using namespace boost::numeric::geometry;

	typedef point_double_2d point_2d;
	typedef point_double_3d point_3d;
	
	point_2d a( 1., 0. );
    point_2d b( 0., 1. );

    double dot = dot_product( a, b );
    BOOST_CHECK_CLOSE( dot, 0., 1e-10 );

    dot = dot_product( a, a );
    BOOST_CHECK_CLOSE( dot, 1., 1e-10 );

    double cross = cross_product( a, b );
    BOOST_CHECK( cross > 0. );

    cross = cross_product( b, a );
    BOOST_CHECK( cross < 0. );

    point_2d zero( 0., 0. );
    cross = cross_product( zero, a, b );
    BOOST_CHECK( cross > 0. );

    cross = cross_product( zero, b, a );
    BOOST_CHECK( cross < 0. );

    point_3d a3( 1., 0., 0. );
    point_3d b3( 0., 1., 0. );
    cross = cross_product( a3, b3 );
    BOOST_CHECK( cross > 0. );

    cross = cross_product( b3, a3 );
    BOOST_CHECK( cross < 0. );

    point_3d zero3( 0., 0., 0. );
    cross = cross_product( zero3, a3, b3 );
    BOOST_CHECK( cross > 0. );

    cross = cross_product( zero3, b3, a3 );
    BOOST_CHECK( cross < 0. );

    typedef vector_double_2d vector_2;
    typedef vector_double_3d vector_3;
    fraction_tolerance_comparison_policy<double> compare(1e-10);

    vector_2 v1( 0.0, 1.0 );
    vector_2 v2( 1.0, 0.0 );
    dot = v1*v2;
    BOOST_CHECK( compare.equals( dot, 0.0 ) );

    vector_2 v3( 2.0, 2.0 );    
    vector_2 v4( 1.0, 1.0 );
    v3 = normalize( v3 );
    v4 = normalize( v4 );
    dot = v3*v4;
    BOOST_CHECK( compare.equals( dot, 1.0 ) );
    
}

#endif //_BOOST_GEOMETRY_PRODUCT_TESTS_HPP



