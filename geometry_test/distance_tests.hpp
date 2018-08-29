//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_DISTANCE_TESTS_HPP
#define GEOMETRIX_DISTANCE_TESTS_HPP

#include <boost/test/included/unit_test.hpp>
#include <geometrix/numeric/number_comparison_policy.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/primitive/segment.hpp>
#include <geometrix/algorithm/segment_intersection.hpp>
#include <geometrix/algorithm/euclidean_distance.hpp>
#include <geometrix/algorithm/distance/eberly_segment_segment_distance.hpp>
#include "2d_kernel_fixture.hpp"
#include <iostream>
#include <array>

BOOST_AUTO_TEST_CASE( TestDistance )
{
    using namespace geometrix;
    
    absolute_tolerance_comparison_policy<double> compare(1e-10);
    {	    
        typedef point_double_3d point_3d;    
        
        point_3d p1( 0., 0., 0. );
        point_3d p2( 1., 1., 1. );
        point_3d p3( 2., 2., 2. );

        double d1 = point_point_distance( p1, p2 );
        double d2 = point_point_distance( p1, p3 );
        double d3 = point_point_distance( p2, p3 );
        BOOST_CHECK( compare.equals( d1, std::sqrt( 3.0 ) ) );
        BOOST_CHECK( compare.equals( d2, std::sqrt( 12.0 ) ) );
        BOOST_CHECK( compare.equals( d3, std::sqrt( 3.0 ) ) );
    }

	{	    
        typedef point_double_3d point_3d;    
        
        const point_3d p1( 0., 0., 0. );
        const point_3d p2( 1., 1., 1. );
        const point_3d p3( 2., 2., 2. );

        double d1 = point_point_distance( p1, p2 );
        double d2 = point_point_distance( p1, p3 );
        double d3 = point_point_distance( p2, p3 );
        BOOST_CHECK( compare.equals( d1, std::sqrt( 3.0 ) ) );
        BOOST_CHECK( compare.equals( d2, std::sqrt( 12.0 ) ) );
        BOOST_CHECK( compare.equals( d3, std::sqrt( 3.0 ) ) );
    }

    {
        typedef point_int_3d point_3d;
        
        point_3d p1( 0, 0, 0 );
        point_3d p2( 1, 1, 1 );
        point_3d p3( 2, 2, 2 );

        double d1 = point_point_distance( p1, p2 );
        double d2 = point_point_distance( p1, p3 );
        double d3 = point_point_distance( p2, p3 );
        BOOST_CHECK( compare.equals( d1, std::sqrt( 3.0 ) ) );
        BOOST_CHECK( compare.equals( d2, std::sqrt( 12.0 ) ) );
        BOOST_CHECK( compare.equals( d3, std::sqrt( 3.0 ) ) );
    }

	typedef point_double_2d point2;
	typedef segment<point2> segment2;
	{		
		segment2 s1( point2( 1.0, 0.0 ), point2( 0.0, 0.0 ) );
		segment2 s2( point2( 0.0, 1.0 ), point2( 0.0, 2.0 ) );
		BOOST_CHECK( compare.equals( segment_segment_distance_sqrd( s1, s2, compare), 1.0 ) );
		BOOST_CHECK( compare.equals( eberly_segment_segment_distance_sqrd( s1, s2 ), 1.0 ) );
	}
	{
		segment2 s1( point2( -1.0264718499965966, 9.6163341007195407e-7 ), point2( 0.91950808032415809, -1.0094441192690283e-6 ) );
		segment2 s2( point2( -1.0629447383806110, 9.2709540082141753e-7 ), point2( 1.0811583868227901, -1.0670017179567367e-6 ) );
		BOOST_CHECK( segment_segment_intersection( s1, s2, (point2*)nullptr, compare ) == e_crossing );//! It should be zero as they intersect.
		BOOST_CHECK( compare.equals( segment_segment_distance_sqrd( s1, s2, compare), 0.0 ) );
		//! TODO: This fails for the version which takes the sqrt of the above result due to roundoff errors.
	}
}

BOOST_AUTO_TEST_CASE( TestPointAABBDistance )
{
	using namespace geometrix;

	typedef point<double, 2> point2;
	absolute_tolerance_comparison_policy<double> cmp( 1e-10 );
	axis_aligned_bounding_box<point2> box( point2( 0., 0. ), point2( 10., 10. ) );

	{
		point2 p( 5, 5 );
		BOOST_CHECK( cmp.equals( point_aabb_distance( p, box ), 0.0 ) );
	}

	{
		point2 p( 15, 5 );
		BOOST_CHECK( cmp.equals( point_aabb_distance( p, box ), 5.0 ) );
	}

	{
		point2 p( 11, 11 );
		BOOST_CHECK( cmp.equals( point_aabb_distance( p, box ), sqrt(2.0) ) );
	}

	{
		point2 p( -1, -1 );
		BOOST_CHECK( cmp.equals( point_aabb_distance( p, box ), sqrt( 2.0 ) ) );
	}
}

BOOST_AUTO_TEST_CASE( TestPointAABBClosestPoint )
{
	using namespace geometrix;

	typedef point<double, 2> point2;
	absolute_tolerance_comparison_policy<double> cmp( 1e-10 );
	axis_aligned_bounding_box<point2> box( point2( 0., 0. ), point2( 10., 10. ) );

	{
		point2 p( 5, 5 );
		auto cp = point_aabb_closest_point(p, box);
		BOOST_CHECK(numeric_sequence_equals(p, cp, cmp));
	}

	{
		point2 p( 15, 5 );
		auto cp = point_aabb_closest_point(p, box);
		BOOST_CHECK(numeric_sequence_equals(cp, point2{10, 5}, cmp));
	}

	{
		point2 p( -5, 5 );
		auto cp = point_aabb_closest_point(p, box);
		BOOST_CHECK(numeric_sequence_equals(cp, point2{0, 5}, cmp));
	}

	{
		point2 p( 5, 15 );
		auto cp = point_aabb_closest_point(p, box);
		BOOST_CHECK(numeric_sequence_equals(cp, point2{5, 10}, cmp));
	}

	{
		point2 p( 5, -5 );
		auto cp = point_aabb_closest_point(p, box);
		BOOST_CHECK(numeric_sequence_equals(cp, point2{5, 0}, cmp));
	}
}

#include <geometrix/primitive/oriented_bounding_box.hpp>
#include <geometrix/primitive/rectangle.hpp>
#include <geometrix/algorithm/distance/point_obb_distance.hpp>
BOOST_AUTO_TEST_CASE(TestPointOBBDistance)
{
	using namespace geometrix;
	typedef point_double_2d point2;
	typedef obb_double_2d obb2;
	typedef vector_double_2d vector2;
	typedef rectangle<point2> rectangle2;
	absolute_tolerance_comparison_policy<double> cmp(1e-10);
	{
		point2 ocenter{ 1,1 };
		vector2 odirection = normalize<vector2>({ 1,1 });
		obb2 obb(ocenter, odirection, left_normal(odirection), 0.5, 0.5);
		rectangle2 r(point2{ 0,0 }, point2{ 1,0 }, point2{ 1,1 }, point2{ 0,1 });
		point2 p{ 0,0 };
		BOOST_CHECK_CLOSE(point_obb_distance(p, obb), 0.91421356237309492, 1e-10);

		point2 cp = point_obb_closest_point(p, obb);
		BOOST_CHECK(numeric_sequence_equals(cp, point2{ 0.64644660940672627, 0.64644660940672627 }, cmp));
	}
}

#include <geometrix/algorithm/distance/segment_obb_distance.hpp>
BOOST_AUTO_TEST_CASE(TestSegmentOBBDistance)
{
	using namespace geometrix;
	typedef point_double_2d point2;
	typedef obb_double_2d obb2;
	typedef vector_double_2d vector2;
	typedef segment<point2> segment2;
	absolute_tolerance_comparison_policy<double> cmp(1e-10);
	{
		point2 ocenter{ 1,1 };
		vector2 odirection = normalize<vector2>({ 1,1 });
		obb2 obb(ocenter, odirection, left_normal(odirection), 0.5, 0.5);
		segment2 seg{ 0, 0, 1, 0 };
		double result = segment_obb_distance(seg, obb, cmp);
		BOOST_CHECK_CLOSE(result, 0.29289321881345254, 1e-10);
	}
}

BOOST_FIXTURE_TEST_CASE(closest_point_segment_segment_ParallelSegs_ReturnsSquaredLengthBetweenTwoSegments, geometry_kernel_2d_fixture)
{
	using namespace geometrix;

	auto a = segment2{ 10., 5., 20., 5. };
	auto b = segment2{ 10., 0., 20., 0. };

	double s, t;
	point2 c1, c2;
	auto d2 = segment_segment_closest_point(a.get_start(), a.get_end(), b.get_start(), b.get_end(), s, t, c1, c2, cmp);

	BOOST_CHECK(cmp.equals(s, 0.));
	BOOST_CHECK(cmp.equals(t, 0.));
	BOOST_CHECK(cmp.equals(d2, 25.0));
}

BOOST_FIXTURE_TEST_CASE(closest_point_segment_segment_IntersectingSegs_ReturnsSquaredLengthBetweenTwoSegments, geometry_kernel_2d_fixture)
{
	using namespace geometrix;

	auto a = segment2{ 10., 5., 20., 5. };
	auto b = segment2{ 15., 0., 15., 10. };

	double s, t;
	point2 c1, c2;
	auto d2 = segment_segment_closest_point(a.get_start(), a.get_end(), b.get_start(), b.get_end(), s, t, c1, c2, cmp);

	BOOST_CHECK(cmp.equals(s, 0.5));
	BOOST_CHECK(cmp.equals(t, 0.5));
	BOOST_CHECK(cmp.equals(d2, 0.0));
}

BOOST_FIXTURE_TEST_CASE(polyline_distance_3line_test, geometry_kernel_2d_fixture)
{
	using namespace geometrix;
	auto pline1 = polyline2{ {0.0,0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0, 1.0} };
	auto pline2 = polyline2{ {3.0,3.0}, {4.0, 3.0}, {4.0, 4.0}, {3.0, 4.0} };
	auto result_check = polyline_polyline_distance_sqrd_brute(pline1, pline2, cmp);
	auto result = polyline_polyline_distance_sqrd(pline1, pline2, cmp);
	BOOST_CHECK(result == result_check);
}

BOOST_FIXTURE_TEST_CASE(polyline_distance_6line_test, geometry_kernel_2d_fixture)
{
	using namespace geometrix;
	auto pline1 = polyline2{ {0.0,0.0}, {0.3, 0.3}, {1.0, 0.0}, {1.0, 1.0}, {0.7, 0.7}, {0.0, 1.0}, {0.0, 0.3} };
	auto pline2 = polyline2{ {3.0,3.0}, {3.3, 3.3}, {4.0, 3.0}, {4.0, 4.0}, {3.7, 3.7}, {3.0, 4.0}, {3.0, 3.3} };
	auto result_check = polyline_polyline_distance_sqrd_brute(pline1, pline2, cmp);
	auto result = polyline_polyline_distance_sqrd(pline1, pline2, cmp);
	BOOST_CHECK(result == result_check);
}

template <typename PointSequence, typename Point, int Divisions = 100, typename std::enable_if<geometrix::is_polyline<PointSequence>::value, int>::type = 0>
inline PointSequence make_circle_as_sequence(Point& center, double r)
{
	using namespace geometrix;
	auto v = vector_double_2d{ r, 0.0 };
	auto s = constants::two_pi<double>() / Divisions, t = 0.;
	auto poly = PointSequence{};
	for (auto i = 0UL; i <= Divisions; ++i, t += s) 
	{
		poly.emplace_back(center + vector_double_2d{ r * cos(t), r * sin(t) });
	}

	return std::move(poly);
}

template <typename PointSequence, typename Point, int Divisions = 100, typename std::enable_if<geometrix::is_polygon<PointSequence>::value, int>::type = 0>
inline PointSequence make_circle_as_sequence(Point& center, double r)
{
	using namespace geometrix;
	auto v = vector_double_2d{ r, 0.0 };
	auto s = constants::two_pi<double>() / Divisions, t = 0.;
	auto poly = PointSequence{};
	for (auto i = 0UL; i < Divisions; ++i, t += s) 
	{
		poly.emplace_back(center + vector_double_2d{ r * cos(t), r * sin(t) });
	}

	return std::move(poly);
}

BOOST_FIXTURE_TEST_CASE(segment_polyline_closest_point_test, geometry_kernel_2d_fixture)
{
	using namespace geometrix;
	auto pline = make_circle_as_sequence<polyline2, point2, 16>(point2{ 0,0 }, 10.0);
	auto seg = segment2{ -10.0, 12.3, 10.0, 12.3 };
	double s, t;
	point2 c1, c2;
	std::size_t i;
	auto result = segment_polyline_closest_point(seg.get_start(), seg.get_end(), pline, i, s, t, c1, c2, cmp);
	BOOST_CHECK(cmp.equals(result, 2.3 * 2.3));
}

BOOST_FIXTURE_TEST_CASE(polyline_polyline_closest_point_test, geometry_kernel_2d_fixture)
{
	using namespace geometrix;
	auto pline = make_circle_as_sequence<polyline2, point2, 16>(point2{ 0,0 }, 10.0);
	auto seg = polyline2{ {-10.0, 12.3}, {10.0, 12.3} };
	double s, t;
	point2 c1, c2;
	std::size_t i, j;
	auto result = polyline_polyline_closest_point(seg, pline, i, j, s, t, c1, c2, cmp);
	BOOST_CHECK(cmp.equals(result, 2.3 * 2.3));
}

BOOST_FIXTURE_TEST_CASE(polyline_closest_point_circular_lines_test, geometry_kernel_2d_fixture)
{
	using namespace geometrix;
	auto pline1 = polyline2{};
	auto pline2 = polyline2{};

	auto p1 = point2{ 0., 0. };
	auto p2 = point2{ 0., 4. };
	auto v = vector2{ 1.0, 0.0 };

	auto s = constants::two_pi<double>() / 100.0, t = 0.;
	for (auto i = 0UL; i < 101; ++i, t += s) 
	{
		v = vector2{ cos(t), sin(t) };
		pline1.emplace_back(p1 + v);
		pline2.emplace_back(p2 + v);
	}

	double s1, s2, t2, t1;
	point2 c1, cc1, c2, cc2;
	std::size_t i, i1, j, j1;
	auto result_check = polyline_polyline_closest_point_brute(pline1, pline2, i1, j1, s1, t1, cc1, cc2, cmp);
	auto result = polyline_polyline_closest_point(pline1, pline2, i, j, s2, t2, c1, c2, cmp);
	BOOST_CHECK(result == result_check);
	BOOST_CHECK(numeric_sequence_equals(cc1, c1, cmp));
	BOOST_CHECK(numeric_sequence_equals(cc2, c2, cmp));
}

BOOST_FIXTURE_TEST_CASE(polyline_distance_circular_lines_test, geometry_kernel_2d_fixture)
{
	using namespace geometrix;
	auto pline1 = polyline2{};
	auto pline2 = polyline2{};

	auto p1 = point2{ 0., 0. };
	auto p2 = point2{ 4., 0. };
	auto v = vector2{ 1.0, 0.0 };

	auto s = constants::two_pi<double>() / 100.0, t = 0.;
	for (auto i = 0UL; i < 101; ++i, t += s) 
	{
		v = vector2{ cos(t), sin(t) };
		pline1.emplace_back(p1 + v);
		pline2.emplace_back(p2 + v);
	}

	auto result_check = polyline_polyline_distance_sqrd_brute(pline1, pline2, cmp);
	auto result = polyline_polyline_distance_sqrd(pline1, pline2, cmp);
	BOOST_CHECK(result == result_check);
}

BOOST_FIXTURE_TEST_CASE(polygon_polygon_closest_point_test, geometry_kernel_2d_fixture)
{
	using namespace geometrix;
	auto pgon = make_circle_as_sequence<polygon2, point2, 16>(point2{ 0,0 }, 10.0);
	auto seg = polygon2{ {-10.0, 12.3}, {10.0, 12.3} };
	double s, t;
	point2 c1, c2;
	std::size_t i, j;
	auto result = polygon_polygon_closest_point(seg, pgon, i, j, s, t, c1, c2, cmp);
	BOOST_CHECK(cmp.equals(result, 2.3 * 2.3));
}

BOOST_FIXTURE_TEST_CASE(polygon_closest_point_circular_lines_test, geometry_kernel_2d_fixture)
{
	using namespace geometrix;
	auto pgon1 = polygon2{};
	auto pgon2 = polygon2{};

	auto p1 = point2{ 0., 0. };
	auto p2 = point2{ 0., 4. };
	auto v = vector2{ 1.0, 0.0 };

	auto s = constants::two_pi<double>() / 100.0, t = 0.;
	for (auto i = 0UL; i < 100; ++i, t += s) 
	{
		v = vector2{ cos(t), sin(t) };
		pgon1.emplace_back(p1 + v);
		pgon2.emplace_back(p2 + v);
	}

	double s1, s2, t2, t1;
	point2 c1, cc1, c2, cc2;
	std::size_t i, i1, j, j1;
	auto result_check = polygon_polygon_closest_point_brute(pgon1, pgon2, i1, j1, s1, t1, cc1, cc2, cmp);
	auto result = polygon_polygon_closest_point(pgon1, pgon2, i, j, s2, t2, c1, c2, cmp);
	BOOST_CHECK(result == result_check);
	BOOST_CHECK(numeric_sequence_equals(cc1, c1, cmp));
	BOOST_CHECK(numeric_sequence_equals(cc2, c2, cmp));
}

BOOST_FIXTURE_TEST_CASE(polygon_distance_circular_lines_test, geometry_kernel_2d_fixture)
{
	using namespace geometrix;
	auto pgon1 = polygon2{};
	auto pgon2 = polygon2{};

	auto p1 = point2{ 0., 0. };
	auto p2 = point2{ 4., 0. };
	auto v = vector2{ 1.0, 0.0 };

	auto s = constants::two_pi<double>() / 100.0, t = 0.;
	for (auto i = 0UL; i < 100; ++i, t += s) 
	{
		v = vector2{ cos(t), sin(t) };
		pgon1.emplace_back(p1 + v);
		pgon2.emplace_back(p2 + v);
	}

	auto result_check = polygon_polygon_distance_sqrd_brute(pgon1, pgon2, cmp);
	auto result = polygon_polygon_distance_sqrd(pgon1, pgon2, cmp);
	BOOST_CHECK(result == result_check);
}

#endif //GEOMETRIX_DISTANCE_TESTS_HPP
