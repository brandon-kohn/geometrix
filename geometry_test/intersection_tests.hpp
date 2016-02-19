//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_INTERSECTION_TESTS_HPP
#define GEOMETRIX_INTERSECTION_TESTS_HPP

#include <boost/test/included/unit_test.hpp>
#include <geometrix/numeric/number_comparison_policy.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/utility/utilities.hpp>
#include <geometrix/arithmetic/vector.hpp>
#include <geometrix/algorithm/segment_intersection.hpp>
#include <geometrix/algorithm/line_intersection.hpp>
#include <geometrix/algorithm/intersection/moving_sphere_plane_intersection.hpp>
#include <geometrix/algorithm/intersection/moving_sphere_segment_intersection.hpp>
#include <geometrix/primitive/sphere.hpp>
#include <geometrix/primitive/plane.hpp>
#include <geometrix/primitive/line.hpp>
#include <geometrix/primitive/segment.hpp>
#include <geometrix/algebra/algebra.hpp>

#include <iostream>

BOOST_AUTO_TEST_CASE( TestIntersections )
{
    using namespace geometrix;

    typedef point_double_2d point_2d;
    
    point_2d p1( 0., 0. );
    point_2d p2( 1., 1. );
    point_2d p3( 1., 0. );
    point_2d p4( 0., 1. );

    segment<point_2d> seg1( p1, p2 );
    segment<point_2d> seg2( p3, p4 );

    {
        point_2d xPoints[2];
        intersection_type iType = calculate_intersection( seg1, seg2, xPoints, fraction_tolerance_comparison_policy<double>(1e-10) );
        BOOST_CHECK( iType == e_crossing );
        std::cout << iType << " at point: " << xPoints[0].get<0>() << ", " << xPoints[0].get<1>() << std::endl;
    }

    {
        point_2d xPoints[2];    
        intersection_type iType = calculate_intersection( seg2, seg1, xPoints, fraction_tolerance_comparison_policy<double>(1e-10) );
        BOOST_CHECK( iType == e_crossing );
        std::cout << iType << " at point: " << xPoints[0].get<0>() << ", " << xPoints[0].get<1>() << std::endl;
    }

    {
        point_2d p;
        segment<point_2d> seg( p1, p3 );

        intersection_type iType = line_segment_intersect( point_2d( 0.5, -50. ), point_2d( 0.5, 50. ), seg, p, fraction_tolerance_comparison_policy<double>(1e-10) );
        BOOST_CHECK( iType == e_crossing );
        std::cout << iType << " at point: " << p.get<0>() << ", " << p.get<1>() << std::endl;

        iType = line_segment_intersect( point_2d( 0.5, -50. ), point_2d( 0.5, -40. ), seg, p, fraction_tolerance_comparison_policy<double>(1e-10) );
        BOOST_CHECK( iType == e_crossing );
        std::cout << iType << " at point: " << p.get<0>() << ", " << p.get<1>() << std::endl;
    }
}

BOOST_AUTO_TEST_CASE( TestMovingCircleLineIntersection )
{
	using namespace geometrix;

	typedef point_double_2d point2;
	typedef vector_double_2d vector2;
	typedef circle_double_2d circle2;
	typedef line<point2, vector2> line2;
	typedef segment_double_2d segment2;
	double t;
	point2 q;
	absolute_tolerance_comparison_policy<double> cmp( 1e-10 );

	//! General case intersecting.
	{
		circle2 circle{point2{1.0, 1.0}, 1.0};
		line2 line{point2{-1, -1}, vector2{0, 1}};
		vector2 velocity{-3, 0};
		
		BOOST_CHECK( moving_sphere_plane_intersection( circle, velocity, line, t, q, cmp ) );
		BOOST_CHECK( numeric_sequence_equals( q, point2{-1, 1}, cmp ) );
	}

	//! Intersect from starting position inside segment.
	{
		circle2 circle{point2{1.0, 1.0}, 0.5};
		segment2 seg{point2{1, -2}, vector2{1, 2}};		
		vector2 velocity{-3, 0};
		BOOST_CHECK( intersect_moving_sphere_segment( circle, velocity, seg, t, q, cmp ) );
		BOOST_CHECK( numeric_sequence_equals( q, point2{1, 1}, cmp ) );
	}

	//! initial: intersecting line, final not intersecting segment and moving away.
	{
		circle2 circle{point2{1.0, -3.0}, 0.5};
		segment2 seg{point2{1, -2}, vector2{1, 2}};
		vector2 velocity{-3, 0};
		BOOST_CHECK( !intersect_moving_sphere_segment( circle, velocity, seg, t, q, cmp ) );
	}

	//! initial: intersecting line not segment, moving toward lower endpoint
	{
		circle2 circle{point2{1.0, -3.0}, 0.5};
		segment2 seg{point2{1, -2}, vector2{1, 2}};
		vector2 velocity{0, 2};
		bool result = intersect_moving_sphere_segment( circle, velocity, seg, t, q, cmp );
		BOOST_CHECK( result );
		BOOST_CHECK( numeric_sequence_equals( q, point2{1, -2}, cmp ) );
		BOOST_CHECK( cmp.equals( t, 0.25 ) );
	}

	//! initial: intersecting line not segment, moving toward upper endpoint
	{
		circle2 circle{point2{1.0, 3.0}, 0.5};
		segment2 seg{point2{1, -2}, vector2{1, 2}};
		vector2 velocity{0, -2};
		bool result = intersect_moving_sphere_segment( circle, velocity, seg, t, q, cmp );
		BOOST_CHECK( result );
		BOOST_CHECK( numeric_sequence_equals( q, point2{1, 2}, cmp ) );
		BOOST_CHECK( cmp.equals( t, 0.25 ) );
	}

	//! initial: not intersecting line nor segment, moving toward upper endpoint
	{
		circle2 circle{point2{2.0, 3.0}, 0.5};
		segment2 seg{point2{1, -2}, vector2{1, 2}};
		vector2 velocity{-2, -2};
		bool result = intersect_moving_sphere_segment( circle, velocity, seg, t, q, cmp );
		BOOST_CHECK( result );
		BOOST_CHECK( numeric_sequence_equals( q, point2{1, 2}, cmp ) );
		BOOST_CHECK( cmp.equals( t, 0.32322330470336319 ) );
	}
}

#endif //GEOMETRIX_INTERSECTION_TESTS_HPP
