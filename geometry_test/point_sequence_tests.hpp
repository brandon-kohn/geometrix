//
//! Copyright © 2008
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _GENERATIVE_GEOMETRY_POINT_SEQUENCE_TESTS_HPP
#define _GENERATIVE_GEOMETRY_POINT_SEQUENCE_TESTS_HPP
#pragma once

#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include "../geometry/point.hpp"
#include "../geometry/utilities.hpp"
#include "../geometry/point_sequence.hpp"
#include "../geometry/point_sequence_utilities.hpp"

BOOST_AUTO_TEST_CASE( TestPointSequences )
{
	using namespace generative::numeric::geometry;

	typedef point_double_2d point_2d;
	typedef point_double_3d point_3d;
	
    std::vector< point_2d > polygon;
    
    polygon.push_back( point_2d( 0., 0. ) ); 
    polygon.push_back( point_2d( 10., 0. ) ); 
    polygon.push_back( point_2d( 10., 10. ) ); 
    polygon.push_back( point_2d( 0., 10. ) ); 
    polygon.push_back( point_2d( 0., 0. ) ); 

    double area = get_area( polygon, fraction_tolerance_comparison_policy<double>(1e-10) );
    BOOST_CHECK_CLOSE( area, 100.0, 1e-10 );

    point_2d centroid = get_centroid( polygon, fraction_tolerance_comparison_policy<double>(1e-10) );
    BOOST_CHECK( numeric_sequence_equals( centroid, point_2d( 5., 5. ), fraction_tolerance_comparison_policy<double>(1e-10) ) );

    BOOST_CHECK( point_in_polygon( centroid, polygon ) );
    BOOST_CHECK( !point_in_polygon( point_2d( -5, -5 ), polygon ) );

}

#endif //_GENERATIVE_GEOMETRY_POINT_SEQUENCE_TESTS_HPP
