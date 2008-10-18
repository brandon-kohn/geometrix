//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_NUMERIC_SEQUENCE_TESTS_HPP
#define _BOOST_GEOMETRY_NUMERIC_SEQUENCE_TESTS_HPP
#pragma once

#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include "../geometry/numeric_sequence.hpp"
#include "../geometry/point.hpp"
#include "../geometry/vector.hpp"

BOOST_AUTO_TEST_CASE( TestNumericSequence )
{
    using namespace boost::numeric::geometry;
    using namespace boost::numeric::geometry::detail;

    typedef numeric_sequence< double, 3 > double_3;
    typedef point< double, 3 > point_3;
    typedef vector< double, 3 > vector_3;

    //double_3 sequence(
    typedef boost::array<double,3> array3;
    array3 ar = { 0., 1., 2. };

    point_3 a( 1., 2., 0. );
	vector_3 b( 1., 2., 0. );
    
	//! Check addition
	{
		point_3 c = a + b;
        BOOST_CHECK_CLOSE( cartesian_access_traits<point_3>::get<0>( c ), 2., 1e-10 );
        BOOST_CHECK_CLOSE( cartesian_access_traits<point_3>::get<1>( c ), 4., 1e-10 );
	}

	//! Check subtraction
	{
		point_3 c = a - b;
        BOOST_CHECK_CLOSE( cartesian_access_traits<point_3>::get<0>( c ), 0., 1e-10 );
        BOOST_CHECK_CLOSE( cartesian_access_traits<point_3>::get<1>( c ), 0., 1e-10 );
	}

	//! Check scalar multiplication
	{
		vector_3 c = b * 4;
        BOOST_CHECK_CLOSE( cartesian_access_traits<vector_3>::get<0>( c ), 4., 1e-10 );
        BOOST_CHECK_CLOSE( cartesian_access_traits<vector_3>::get<1>( c ), 8., 1e-10 );
	}

	//! Check scalar division
	{
		vector_3 c = b / 4;
        BOOST_CHECK_CLOSE( cartesian_access_traits<vector_3>::get<0>( c ), .25, 1e-10 );
        BOOST_CHECK_CLOSE( cartesian_access_traits<vector_3>::get<1>( c ), .5, 1e-10 );
	}

    //Test polar access traits    
    typedef polar_access_traits< point_3 > polar_access_2d;    
    boost::function_requires< PolarCoordinateAccessorConcept< polar_access_2d > >();
    
    double r = polar_access_2d::get<0>( a );
    double t = polar_access_2d::get<1>( a );

    typedef boost::numeric::geometry::point_double_3d CPoint3D;
    typedef polar_access_traits< CPoint3D > polar_access_3d;
    boost::function_requires< PolarCoordinateAccessorConcept< polar_access_3d > >();
    point_3 d( 1., 1., 1. );
    double r2 = polar_access_3d::get<0>( d );
    double t2 = polar_access_3d::get<1>( d );
    double phi = polar_access_3d::get<2>( d );

}

#endif //_BOOST_GEOMETRY_NUMERIC_SEQUENCE_TESTS_HPP
