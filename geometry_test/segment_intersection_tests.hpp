//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_SEGMENT_INTERSECTION_TESTS_HPP
#define GEOMETRIX_SEGMENT_INTERSECTION_TESTS_HPP

#include <boost/test/included/unit_test.hpp>
#include <geometrix/numeric/number_comparison_policy.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/utility/utilities.hpp>
#include <geometrix/algebra/algebra.hpp>
#include <geometrix/algorithm/euclidean_distance.hpp>
#include <geometrix/arithmetic/vector.hpp>
#include <geometrix/algorithm/segment_intersection.hpp>
#include <geometrix/primitive/segment.hpp>
#include <geometrix/primitive/line.hpp>
//#include <geometrix/algorithm/bentley_ottmann_segment_intersection.hpp>
#include <geometrix/algorithm/intersection/segment_triangle_intersection.hpp>
#include <geometrix/algorithm/is_segment_in_range.hpp>

#include <iostream>

BOOST_AUTO_TEST_CASE( TestSegmentIntersection3d )
{
    using namespace geometrix;
    
    point_double_3d p1(1,0,0);
    vector_double_3d v1(2,3,1);
    point_double_3d p2 = construct<point_double_3d>( p1 + v1 );

    point_double_3d p3(0,5,5);
    vector_double_3d v2(5,1,-3);
    point_double_3d p4 = construct<point_double_3d>( p3 + v2 );
    
    point_double_3d iPoint[2];
    typedef segment<point_double_3d> segment_t;
    intersection_type s = calculate_intersection( segment_t(p1, p2), segment_t(p3, p4), iPoint, fraction_tolerance_comparison_policy<double>(1e-10) );
    BOOST_CHECK( s == e_non_crossing );

    p1 = construct<point_double_3d>( 0, 0, 0 );
    v1 = construct<vector_double_3d>( 1, 1, 1 );
    p2 <<= p1 + v1;

    p3 = construct<point_double_3d>( 0.5, 0.5, 0.5 );
    p4 <<= p3 + v1;
    s = calculate_intersection( segment_t(p1, p2), segment_t(p3, p4), iPoint, fraction_tolerance_comparison_policy<double>(1e-10) );
    BOOST_CHECK( s == e_overlapping );

    p1 = construct<point_double_3d>( 0, 0, 0 );
    set<1>( p1, 1.0 );
    v1 = construct<vector_double_3d>( 1, 1, 1 );
    p2 <<= p1 + v1;

    p3 = construct<point_double_3d>( 0.5, 0.5, 0.5 );
    p4 <<= p3 + v1;
    s = calculate_intersection( segment_t(p1, p2), segment_t(p3, p4), iPoint, fraction_tolerance_comparison_policy<double>(1e-10) );
    BOOST_CHECK( s == e_non_crossing );
}

BOOST_AUTO_TEST_CASE( TestSegmentTriangleIntersection )
{
	using namespace geometrix;

	typedef point_double_2d point2;

	BOOST_CHECK( segment_triangle_intersect( point2( 0.0, 0.5 ), point2( 0.0, 1.0 ), point2( 0.0, 0.0 ), point2( 1.0, 0.0 ), point2( 0.5, 1.0 )) == false );
	BOOST_CHECK( segment_triangle_intersect( point2( 10.0, 0.5 ), point2( 10.0, 1.0 ), point2( 0.0, 0.0 ), point2( 1.0, 0.0 ), point2( 0.5, 1.0 ) ) == false );
	BOOST_CHECK( segment_triangle_intersect( point2( 0.5, 0.5 ), point2( 0.5, 0.6 ), point2( 0.0, 0.0 ), point2( 1.0, 0.0 ), point2( 0.5, 1.0 ) ) == true );
	BOOST_CHECK( segment_triangle_intersect( point2( 0.5, 0.5 ), point2( 0.5, 1.0 ), point2( 0.0, 0.0 ), point2( 1.0, 0.0 ), point2( 0.5, 1.0 )) == true );
	BOOST_CHECK( segment_triangle_intersect( point2( 0.5, 0.5 ), point2( 1.0, 0.5 ), point2( 0.0, 0.0 ), point2( 1.0, 0.0 ), point2( 0.5, 1.0 ) ) == true );
	BOOST_CHECK( segment_triangle_intersect( point2( 0.0, 0.0 ), point2( 1.0, 0.0 ), point2( 0.0, 0.0 ), point2( 1.0, 0.0 ), point2( 0.5, 1.0 ) ) == true );
	BOOST_CHECK( segment_triangle_intersect( point2( 1.0, 0.0 ), point2( 0.5, 1.0 ), point2( 0.0, 0.0 ), point2( 1.0, 0.0 ), point2( 0.5, 1.0 ) ) == true );
	BOOST_CHECK( segment_triangle_intersect( point2( 0.5, 1.0 ), point2( 0.0, 0.0 ), point2( 0.0, 0.0 ), point2( 1.0, 0.0 ), point2( 0.5, 1.0 ) ) == true );

	point2 xPoints[2];
	absolute_tolerance_comparison_policy<double> cmp( 1e-10 );
	BOOST_CHECK( segment_triangle_intersect( point2( 0.0, 0.5 ), point2( 0.0, 1.0 ), point2( 0.0, 0.0 ), point2( 1.0, 0.0 ), point2( 0.5, 1.0 ), xPoints, cmp ) == 0 );
	BOOST_CHECK( segment_triangle_intersect( point2( 10.0, 0.5 ), point2( 10.0, 1.0 ), point2( 0.0, 0.0 ), point2( 1.0, 0.0 ), point2( 0.5, 1.0 ), xPoints, cmp ) == 0 );
	BOOST_CHECK( segment_triangle_intersect( point2( 0.5, 0.5 ), point2( 0.5, 0.6 ), point2( 0.0, 0.0 ), point2( 1.0, 0.0 ), point2( 0.5, 1.0 ), xPoints, cmp ) == 2 );
	BOOST_CHECK( segment_triangle_intersect( point2( 0.5, 0.5 ), point2( 0.5, 1.0 ), point2( 0.0, 0.0 ), point2( 1.0, 0.0 ), point2( 0.5, 1.0 ), xPoints, cmp ) == 2 );
	BOOST_CHECK( segment_triangle_intersect( point2( 0.5, 0.5 ), point2( 1.0, 0.5 ), point2( 0.0, 0.0 ), point2( 1.0, 0.0 ), point2( 0.5, 1.0 ), xPoints, cmp ) == 2 );
	BOOST_CHECK( segment_triangle_intersect( point2( 0.0, 0.0 ), point2( 1.0, 0.0 ), point2( 0.0, 0.0 ), point2( 1.0, 0.0 ), point2( 0.5, 1.0 ), xPoints, cmp ) == 2 );
	BOOST_CHECK( segment_triangle_intersect( point2( 1.0, 0.0 ), point2( 0.5, 1.0 ), point2( 0.0, 0.0 ), point2( 1.0, 0.0 ), point2( 0.5, 1.0 ), xPoints, cmp ) == 2 );
	BOOST_CHECK( segment_triangle_intersect( point2( 0.5, 1.0 ), point2( 0.0, 0.0 ), point2( 0.0, 0.0 ), point2( 1.0, 0.0 ), point2( 0.5, 1.0 ), xPoints, cmp ) == 2 );
	BOOST_CHECK( segment_triangle_intersect( point2( -0.5, -1.0 ), point2( 0.0, 0.0 ), point2( 0.0, 0.0 ), point2( 1.0, 0.0 ), point2( 0.5, 1.0 ), xPoints, cmp ) == 1 );

}

// struct IntersectionVisitor
// {
// 	template <typename Point, typename SegmentIterator>
// 	void operator()( const Point& event, SegmentIterator first, SegmentIterator last )
// 	{
// 		using namespace geometrix;
// 		std::cout << "Intersection: " << get<0>( event ) << "," << get<1>( event ) << std::endl;
// 	}
// };

// BOOST_AUTO_TEST_CASE( TestBentleyOttmannSegmentIntersection )
// {
// 	using namespace geometrix;
// 
// 	typedef point_double_2d point_2d;
// 
// 	point_2d p1( 0., 0. );
// 	point_2d p2( 1., 1. );
// 	point_2d p3( 1., 0. );
// 	point_2d p4( 0., 1. );
// 	point_2d p5( 1., 2. );
// 	point_2d p6( 0.5, -0.1 );
// 	point_2d p7( 1.1, 0.8 );
// 
// 	segment<point_2d> seg1( p1, p2 );
// 	segment<point_2d> seg2( p3, p4 );
// 	segment<point_2d> seg3( p3, p2 );
// 	segment<point_2d> seg4( p2, p4 );
// 	segment<point_2d> seg5( p6, p7 );
// 	segment<point_2d> seg6( p1, p3 );
// 
// 	std::vector< segment<point_2d> > segments;
// 	segments.push_back( seg1 );
// 	segments.push_back( seg2 );
// 	segments.push_back( seg3 );
// 	segments.push_back( seg4 );
// 	segments.push_back( seg5 );
// 	segments.push_back( seg6 );
// 
// 	{
// 		point_2d xPoints[2];
// 		intersection_type iType = calculate_intersection( seg5, seg6, xPoints, fraction_tolerance_comparison_policy<double>( 1e-10 ) );
// 		std::cout << iType << " at point: " << get<0>( xPoints[0] ) << ", " << get<1>( xPoints[0] ) << std::endl;
// 	}
// 
// 	IntersectionVisitor v;
// 	//bentley_ottmann_segment_intersection( segments, v, fraction_tolerance_comparison_policy<double>( 1e-10 ) );
// 
// }

BOOST_AUTO_TEST_CASE( TestIsSegmentInRange )
{
	using namespace geometrix;
	typedef point_double_2d point2;
	typedef vector_double_2d vector2;
	typedef segment_double_2d segment2;

	point2 orig( 0, 0 );

	//! Range vectors are the standard 1st quadrant (+x, +y  plane).
	{
		vector2 lo{1, 0}, hi{0, 1};
		BOOST_CHECK( is_segment_in_range_2d( segment2( 1., 1., 2., 2. ), lo, hi, orig ) );
		
		BOOST_CHECK( !is_segment_in_range_2d( segment2( -1., -1., -2., -2. ), lo, hi, orig ) );

		BOOST_CHECK( !is_segment_in_range_2d( segment2( -2., -2., -1., -1. ), lo, hi, orig ) );
		
		BOOST_CHECK( !is_segment_in_range_2d( segment2( 1., -1., 2., -2. ), lo, hi, orig ) );

		//! Both A and B are outside the range.
		BOOST_CHECK( is_segment_in_range_2d( segment2( -0.5, 0.75, 0.75, -0.5 ), lo, hi, orig ) );

		//! A and B reversed and outside the range.
		BOOST_CHECK( is_segment_in_range_2d( segment2( 0.75, -0.5, -0.5, 0.75 ), lo, hi, orig ) );

		//! A and B both lay on the range vectors.
		BOOST_CHECK( is_segment_in_range_2d( segment2( 0.5, 0., 0., 0.5 ), lo, hi, orig ) );

		//! reversed
		BOOST_CHECK( is_segment_in_range_2d( segment2( 0., 0.5, 0.5, 0. ), lo, hi, orig ) );

		//! A and B both lay to the left of hi (outside the range) in quadrant 2.
		BOOST_CHECK( !is_segment_in_range_2d( segment2( -1., 1., -2., 2. ), lo, hi, orig ) );

		//! A and B both lay to the right of lo (outside the range) in quadrant 4.
		BOOST_CHECK( !is_segment_in_range_2d( segment2( 1., -1., 2., -2. ), lo, hi, orig ) );

		//! Segment is wholly collinear and overlapping the lo range.
		BOOST_CHECK( is_segment_in_range_2d( segment2( 0., 0., 1., 0. ), lo, hi, orig ) );

		//! Segment is wholly collinear and overlapping the hi range.
		BOOST_CHECK( is_segment_in_range_2d( segment2( 0., 0., 0., 1. ), lo, hi, orig ) );

		//! Segment starts at origin and falls outside the range.
		//! Convention should be to include as the origin overlaps.
		BOOST_CHECK( is_segment_in_range_2d( segment2( 0., 0., -1, -1 ), lo, hi, orig ) );
	}

	//! Range vectors span the 1st and 2nd quadrants.
	{
		vector2 lo{1, 0}, hi{-1, 0};
		
		BOOST_CHECK( is_segment_in_range_2d( segment2( 1., 1., 2., 2. ), lo, hi, orig ) );
		
		BOOST_CHECK( !is_segment_in_range_2d( segment2( -1., -1., -2., -2. ), lo, hi, orig ) );

		BOOST_CHECK( !is_segment_in_range_2d( segment2( 1., -1., 2., -2. ), lo, hi, orig ) );

		BOOST_CHECK( is_segment_in_range_2d( segment2( -0.5, 0.75, 0.75, -0.5 ), lo, hi, orig ) );

		//! A and B both lay on the range vectors.
		BOOST_CHECK( is_segment_in_range_2d( segment2( 0.5, 0., -0.5, 0. ), lo, hi, orig ) );

		//! reversed
		BOOST_CHECK( is_segment_in_range_2d( segment2( -0.5, 0., 0.5, 0. ), lo, hi, orig ) );

		//! Segment starts at origin and falls outside the range.
		//! Convention should be to include as the origin overlaps.
		BOOST_CHECK( is_segment_in_range_2d( segment2( 0., 0., 0., -1 ), lo, hi, orig ) );
	}
}

BOOST_AUTO_TEST_CASE( TestIsSegmentInRangeXPoints )
{
	using namespace geometrix;
	typedef point_double_2d point2;
	typedef vector_double_2d vector2;
	typedef segment_double_2d segment2;

	point2 orig( 0, 0 );
	absolute_tolerance_comparison_policy<double> cmp( 1e-10 );
	point2 xPoints[2];

	//! Range vectors are the standard 1st quadrant (+x, +y  plane).
	{
		vector2 lo{1, 0}, hi{0, 1};
		BOOST_CHECK( is_segment_in_range_2d( segment2( 1., 1., 2., 2. ), lo, hi, orig, xPoints, cmp ) );

		BOOST_CHECK( !is_segment_in_range_2d( segment2( -1., -1., -2., -2. ), lo, hi, orig, xPoints, cmp ) );

		BOOST_CHECK( !is_segment_in_range_2d( segment2( -2., -2., -1., -1. ), lo, hi, orig, xPoints, cmp ) );

		BOOST_CHECK( !is_segment_in_range_2d( segment2( 1., -1., 2., -2. ), lo, hi, orig, xPoints, cmp ) );

		//! Both A and B are outside the range.
		BOOST_CHECK( is_segment_in_range_2d( segment2( -0.5, 0.75, 0.75, -0.5 ), lo, hi, orig, xPoints, cmp ) );

		//! A and B reversed and outside the range.
		BOOST_CHECK( is_segment_in_range_2d( segment2( 0.75, -0.5, -0.5, 0.75 ), lo, hi, orig, xPoints, cmp ) );

		//! A and B both lay on the range vectors.
		BOOST_CHECK( is_segment_in_range_2d( segment2( 0.5, 0., 0., 0.5 ), lo, hi, orig, xPoints, cmp ) );

		//! reversed
		BOOST_CHECK( is_segment_in_range_2d( segment2( 0., 0.5, 0.5, 0. ), lo, hi, orig, xPoints, cmp ) );

		//! A and B both lay to the left of hi (outside the range) in quadrant 2.
		BOOST_CHECK( !is_segment_in_range_2d( segment2( -1., 1., -2., 2. ), lo, hi, orig, xPoints, cmp ) );

		//! A and B both lay to the right of lo (outside the range) in quadrant 4.
		BOOST_CHECK( !is_segment_in_range_2d( segment2( 1., -1., 2., -2. ), lo, hi, orig, xPoints, cmp ) );

		//! Segment is wholly collinear and overlapping the lo range.
		BOOST_CHECK( is_segment_in_range_2d( segment2( 0., 0., 1., 0. ), lo, hi, orig, xPoints, cmp ) );

		//! Segment is wholly collinear and overlapping the hi range.
		BOOST_CHECK( is_segment_in_range_2d( segment2( 0., 0., 0., 1. ), lo, hi, orig, xPoints, cmp ) );

		//! Segment starts at origin and falls outside the range.
		//! Convention should be to include as the origin overlaps.
		BOOST_CHECK( is_segment_in_range_2d( segment2( 0., 0., -1, -1 ), lo, hi, orig, xPoints, cmp ) );
	}

	//! Range vectors span the 1st and 2nd quadrants.
	{
		vector2 lo{1, 0}, hi{-1, 0};

		BOOST_CHECK( is_segment_in_range_2d( segment2( 1., 1., 2., 2. ), lo, hi, orig, xPoints, cmp ) );

		BOOST_CHECK( !is_segment_in_range_2d( segment2( -1., -1., -2., -2. ), lo, hi, orig, xPoints, cmp ) );

		BOOST_CHECK( !is_segment_in_range_2d( segment2( 1., -1., 2., -2. ), lo, hi, orig, xPoints, cmp ) );

		BOOST_CHECK( is_segment_in_range_2d( segment2( -0.5, 0.75, 0.75, -0.5 ), lo, hi, orig, xPoints, cmp ) );

		//! A and B both lay on the range vectors.
		BOOST_CHECK( is_segment_in_range_2d( segment2( 0.5, 0., -0.5, 0. ), lo, hi, orig, xPoints, cmp ) );

		//! reversed
		BOOST_CHECK( is_segment_in_range_2d( segment2( -0.5, 0., 0.5, 0. ), lo, hi, orig, xPoints, cmp ) );

		//! Segment starts at origin and falls outside the range.
		//! Convention should be to include as the origin overlaps.
		BOOST_CHECK( is_segment_in_range_2d( segment2( 0., 0., 0., -1 ), lo, hi, orig, xPoints, cmp ) );
	}

	//! Case where one segment end lays on vector which should find two xpoints.
	{
		segment2 segment{2.9258824939351098, -3.1790399695953453, 4.5546968504549721, -4.5007328175926116};
		point2 origin{7.7816492997812539, 3.2786150708659449};
		vector2 lo{-4.8557668058461445, -6.4576550404612902}, hi{-1.6251837264018896, -3.6551071609521757};
		BOOST_CHECK( is_segment_in_range_2d( segment, lo, hi, origin, xPoints, cmp ) );

		BOOST_CHECK( numeric_sequence_equals_2d( xPoints[0], point2{2.9258824939351094, -3.1790399695953449}, cmp ) );
		BOOST_CHECK( numeric_sequence_equals_2d( xPoints[1], point2{4.3842027843380089, -4.3623863038166473}, cmp ) );
	}
}

#endif //GEOMETRIX_SEGMENT_INTERSECTION_TESTS_HPP
