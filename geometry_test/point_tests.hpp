//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_POINT_TESTS_HPP
#define _BOOST_GEOMETRY_POINT_TESTS_HPP
#pragma once

#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include "../geometry/point.hpp"
#include "../geometry/utilities.hpp"

BOOST_AUTO_TEST_CASE( TestPointOperators )
{
	using namespace boost::numeric::geometry;

	typedef point_double_2d CPoint2D;
	typedef point_double_3d CPoint3D;

    typedef cartesian_access_traits< CPoint2D > cartesian_access_2d;
    typedef cartesian_access_traits< CPoint3D > cartesian_access_3d;

    boost::function_requires< CartesianCoordinateAccessorConcept< cartesian_access_2d > >();
    boost::function_requires< CartesianCoordinateAccessorConcept< cartesian_access_3d > >();
	
	CPoint2D p( 1, 2 );

    BOOST_CHECK( point_traits< CPoint2D >::dimension_type::value == 2 );
    BOOST_CHECK( point_traits< CPoint3D >::dimension_type::value == 3 );

	//! Shouldn't compile as the point is 2D.
	//p(Z);

	//! Shouldn't compile
	//typedef point_template<double,4> point_4d;
	//point_4d p4;

	typedef boost::numeric::geometry::point_double_2d CPoint2D;
	CPoint2D a( 1., 1. );
	CPoint2D b( 1., 1. );
    
	//! Check addition
	{
		CPoint2D c = a + b;
        BOOST_CHECK_CLOSE( cartesian_access_2d::get_x( c ), 2., 1e-10 );
        BOOST_CHECK_CLOSE( cartesian_access_2d::get_y( c ), 2., 1e-10 );
	}

	//! Check subtraction
	{
		CPoint2D c = a - b;
        BOOST_CHECK_CLOSE( cartesian_access_2d::get_x( c ), 0., 1e-10 );
        BOOST_CHECK_CLOSE( cartesian_access_2d::get_y( c ), 0., 1e-10 );
	}

	//! Check scalar multiplication
	{
		CPoint2D c = a * 4;
        BOOST_CHECK_CLOSE( cartesian_access_2d::get_x( c ), 4., 1e-10 );
        BOOST_CHECK_CLOSE( cartesian_access_2d::get_y( c ), 4., 1e-10 );
	}

	//! Check scalar division
	{
		CPoint2D c = a / 4;
        BOOST_CHECK_CLOSE( cartesian_access_2d::get_x( c ), .25, 1e-10 );
        BOOST_CHECK_CLOSE( cartesian_access_2d::get_y( c ), .25, 1e-10 );
	}

    //Test polar access traits    
    typedef polar_access_traits< CPoint2D > polar_access_2d;    
    boost::function_requires< PolarCoordinateAccessorConcept< polar_access_2d > >();
    
    double r = polar_access_2d::get_radius( a );
    double t = polar_access_2d::get_theta( a );

    typedef boost::numeric::geometry::point_double_3d CPoint3D;
    typedef polar_access_traits< CPoint3D > polar_access_3d;
    boost::function_requires< PolarCoordinateAccessorConcept< polar_access_3d > >();
    CPoint3D d( 1., 1., 1. );
    double r2 = polar_access_3d::get_radius( d );
    double t2 = polar_access_3d::get_theta( d );
    double phi = polar_access_3d::get_phi( d );
}

#endif //_BOOST_GEOMETRY_POINT_TESTS_HPP



