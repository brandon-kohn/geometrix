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
#include <geometrix/algorithm/bentley_ottmann_segment_intersection.hpp>
#include <geometrix/algorithm/intersection/segment_triangle_intersection.hpp>
#include <geometrix/algorithm/is_segment_in_range.hpp>

#include <iostream>

typedef geometrix::point_double_2d point2;
typedef geometrix::vector_double_2d vector2;
typedef geometrix::segment_double_2d segment2;

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
    intersection_type s = segment_segment_intersection( segment_t(p1, p2), segment_t(p3, p4), iPoint, absolute_tolerance_comparison_policy<double>(1e-10) );
    BOOST_CHECK( s == e_non_crossing );

    p1 = construct<point_double_3d>( 0, 0, 0 );
    v1 = construct<vector_double_3d>( 1, 1, 1 );
    p2 <<= p1 + v1;

    p3 = construct<point_double_3d>( 0.5, 0.5, 0.5 );
    p4 <<= p3 + v1;
    s = segment_segment_intersection( segment_t(p1, p2), segment_t(p3, p4), iPoint, absolute_tolerance_comparison_policy<double>(1e-10) );
    BOOST_CHECK( s == e_overlapping );

    p1 = construct<point_double_3d>( 0, 0, 0 );
    set<1>( p1, 1.0 );
    v1 = construct<vector_double_3d>( 1, 1, 1 );
    p2 <<= p1 + v1;

    p3 = construct<point_double_3d>( 0.5, 0.5, 0.5 );
    p4 <<= p3 + v1;
    s = segment_segment_intersection( segment_t(p1, p2), segment_t(p3, p4), iPoint, absolute_tolerance_comparison_policy<double>(1e-10) );
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

struct IntersectionVisitor
{
	template <typename Point, typename SegmentIterator>
	void operator()( const Point& event, SegmentIterator first, SegmentIterator last )
	{
		using namespace geometrix;
		std::cout << "Intersection: " << get<0>( event ) << "," << get<1>( event ) << std::endl;
	}
};

BOOST_AUTO_TEST_CASE( TestBentleyOttmannSegmentIntersection )
{
	using namespace geometrix;

	typedef point_double_2d point_2d;

	point_2d p1( 0., 0. );
	point_2d p2( 1., 1. );
	point_2d p3( 1., 0. );
	point_2d p4( 0., 1. );
	point_2d p5( 1., 2. );
	point_2d p6( 0.5, -0.1 );
	point_2d p7( 1.1, 0.8 );

	segment<point_2d> seg1( p1, p2 );
	segment<point_2d> seg2( p3, p4 );
	segment<point_2d> seg3( p3, p2 );
	segment<point_2d> seg4( p2, p4 );
	segment<point_2d> seg5( p6, p7 );
	segment<point_2d> seg6( p1, p3 );

	std::vector< segment<point_2d> > segments;
	segments.push_back( seg1 );
	segments.push_back( seg2 );
	segments.push_back( seg3 );
	segments.push_back( seg4 );
	segments.push_back( seg5 );
	segments.push_back( seg6 );

	{
		point_2d xPoints[2];
		intersection_type iType = segment_segment_intersection( seg5, seg6, xPoints, absolute_tolerance_comparison_policy<double>( 1e-10 ) );
		std::cout << iType << " at point: " << get<0>( xPoints[0] ) << ", " << get<1>( xPoints[0] ) << std::endl;
	}

	IntersectionVisitor v;
	bentley_ottmann_segment_intersection( segments, v, absolute_tolerance_comparison_policy<double>( 1e-10 ) );

}

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

void clip_geometry( std::vector<segment2>& segments, const segment2& clip )
{
	using namespace geometrix;	
	std::vector<segment2> toAdd;
	for( std::size_t i = 0; i < segments.size(); ++i ){
		segment2& seg = segments[i];
		if( segment_segment_intersection( clip, seg, (point2*)nullptr, absolute_tolerance_comparison_policy<double>(1e-10) ) == e_overlapping ){
			std::set<segment2, segment_interval_compare< absolute_tolerance_comparison_policy<double> > > toSplit{seg};
			collinear_segment_difference( toSplit, clip, absolute_tolerance_comparison_policy<double>( 1e-10 ) );
			if( !toSplit.empty() )
			{
				auto it = toSplit.begin();
				seg = *it++;
				while( it != toSplit.end() ){
					toAdd.push_back( *it );
					++it;
				}
			}
		}
	}

	for( const auto& seg : toAdd ){
		segments.push_back( seg );
	}
}

BOOST_AUTO_TEST_CASE( TestClipGeometry )
{
	using namespace geometrix;
	{
		std::vector<segment2> mObstacleSegments = {
			segment2{-32.572933099989314, 40.070068866480142, -121.46199416666059, 49.868863000534475}
			, segment2{-121.46199416666059, 49.868863000534475, -141.05958243331406, -40.420025799423456}
			, segment2{-141.05958243331406, -40.420025799423456, 23.945111633336637, -49.868863000068814}
			, segment2{23.945111633336637, -49.868863000068814, 22.020348500052933, 20.997416000813246}
			, segment2{22.020348500052933, 20.997416000813246, -32.572933099989314, 40.070068866480142}
		};

		segment2 door{23.410480472017341, -30.184715696196655, 23.225857199040536, -23.387222463608985};
		clip_geometry( mObstacleSegments, door );
	}

	{
		std::vector<segment2> mObstacleSegments = {
			segment2{34.130978416302241, -23.782644675578922, 23.311946932983119, -23.391059800516814}
			, segment2{23.311946932983119, -23.391059800516814, 23.481280933017842, -30.168653150554746}
			, segment2{23.481280933017842, -30.168653150554746, 39.060427054238971, -30.126825041137636}
			, segment2{39.060427054238971, -30.126825041137636, 56.585636959178373, -30.82817380130291}
			, segment2{56.585636959178373, -30.82817380130291, 56.480685156828258, -22.938003155402839}
			, segment2{56.480685156828258, -22.938003155402839, 34.130978416302241, -23.782644675578922}
		};

		segment2 door{23.311946932983119, -23.391059800516814, 23.481280933017842, -30.168653150554746};
		clip_geometry( mObstacleSegments, door );
	}

	{
		std::vector<segment2> mObstacleSegments = {
			segment2{34.130978416302241, -23.782644675578922, 23.311946932983119, -23.391059800516814}
			, segment2{0, 9.8813129168249309e-324, 0, 0}
			, segment2{23.481280933017842, -30.168653150554746, 39.060427054238971, -30.126825041137636}
			, segment2{39.060427054238971, -30.126825041137636, 56.585636959178373, -30.82817380130291}
			, segment2{56.585636959178373, -30.82817380130291, 56.480685156828258, -22.938003155402839}
			, segment2{56.480685156828258, -22.938003155402839, 34.130978416302241, -23.782644675578922}
		};
		segment2 door{56.584947315827691, -30.776327109775664, 56.482534144831277, -23.077008208573666};
		clip_geometry( mObstacleSegments, door );
	}

	{
		std::vector<segment2> mObstacleSegments = {
			segment2{112.53809236671077, 14.973665433935821, 55.84506916673854, 23.547610300593078}
		  , segment2{55.84506916673854, 23.547610300593078, 56.894939966732636, -48.01858256617561}
		  , segment2{56.894939966732636, -48.01858256617561, 91.015740966715384, -49.593388766050339}
		  , segment2{91.015740966715384, -49.593388766050339, 141.05958243337227, -11.798039966262877}
		  , segment2{141.05958243337227, -11.798039966262877, 112.53809236671077, 14.973665433935821}
		};
		
		segment2 door{56.529054251296813, -23.077372963791056, 56.642000533756573, -30.776544551519073};
		clip_geometry( mObstacleSegments, door );
	}
}

#endif //GEOMETRIX_SEGMENT_INTERSECTION_TESTS_HPP
