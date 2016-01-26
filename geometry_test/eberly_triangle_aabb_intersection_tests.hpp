//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_EBERLY_TRIANGLE_AABB_INTERSECTION_TESTS_HPP
#define GEOMETRIX_EBERLY_TRIANGLE_AABB_INTERSECTION_TESTS_HPP

#include <boost/test/included/unit_test.hpp>
#include <geometrix/numeric/number_comparison_policy.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/algorithm/eberly_triangle_aabb_intersection.hpp>

BOOST_AUTO_TEST_CASE( TestTriangleAABBIntersection )
{
	using namespace geometrix;
    typedef point<double, 2> point2;
    absolute_tolerance_comparison_policy<double> cmp(1e-10);
	
	axis_aligned_bounding_box<point2> box( point2( 0., 0. ), point2( 10., 10. ) );

	//! Overlapping
	BOOST_CHECK( eberly_triangle_aabb_intersection_2d( point2( 0, 0 ), point2( 1.0, 1.0 ), point2( 0., 1.0 ), box, cmp ) );

	//! Sharing a vertex.
	BOOST_CHECK( eberly_triangle_aabb_intersection_2d( point2( 0, 0 ), point2( -1.0, -1.0 ), point2( 0., -1.0 ), box, cmp ) );

	//! disjoint
	BOOST_CHECK( !eberly_triangle_aabb_intersection_2d( point2( -1, -1 ), point2( -4.0, -4.0 ), point2( -2., 0.0 ), box, cmp ) );
	BOOST_CHECK( !eberly_triangle_aabb_intersection_2d( point2( 0, 10 ), point2( 10, 0 ), point2( 10, 10 ), axis_aligned_bounding_box<point2>( point2( 0., 0. ), point2( 1., 1. ) ), cmp ) );

	//! Box wholly inside triangle.
	BOOST_CHECK( eberly_triangle_aabb_intersection_2d( point2( 0, 0 ), point2( 100.0, 100.0 ), point2(80., 1.0 ), axis_aligned_bounding_box<point2>( point2( 50., 25. ), point2( 60., 35. ) ), cmp ) );

	//! Triangle wholly inside box
	BOOST_CHECK( eberly_triangle_aabb_intersection_2d( point2( 1, 1 ), point2( 2, 2 ), point2( 1.5, 2 ), axis_aligned_bounding_box<point2>( point2( 0., 0. ), point2( 10., 10. ) ), cmp ) );
}

#endif //GEOMETRIX_EBERLY_TRIANGLE_AABB_INTERSECTION_TESTS_HPP
