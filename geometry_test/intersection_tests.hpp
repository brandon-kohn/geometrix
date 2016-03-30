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
#include <geometrix/algorithm/intersection/segment_capsule_intersection.hpp>
#include <geometrix/primitive/sphere.hpp>
#include <geometrix/primitive/plane.hpp>
#include <geometrix/primitive/line.hpp>
#include <geometrix/primitive/segment.hpp>
#include <geometrix/primitive/polygon.hpp>
#include <geometrix/primitive/polyline.hpp>
#include <geometrix/algebra/algebra.hpp>
#include <geometrix/utility/ignore_unused_warnings.hpp>

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
        intersection_type iType = segment_segment_intersection( seg1, seg2, xPoints, fraction_tolerance_comparison_policy<double>(1e-10) );
        BOOST_CHECK( iType == e_crossing );
        std::cout << iType << " at point: " << xPoints[0].get<0>() << ", " << xPoints[0].get<1>() << std::endl;
    }

    {
        point_2d xPoints[2];    
        intersection_type iType = segment_segment_intersection( seg2, seg1, xPoints, fraction_tolerance_comparison_policy<double>(1e-10) );
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
		BOOST_CHECK( moving_sphere_segment_intersection( circle, velocity, seg, t, q, cmp ) );
		BOOST_CHECK( numeric_sequence_equals( q, point2{1, 1}, cmp ) );
	}

	//! initial: intersecting line, final not intersecting segment and moving away.
	{
		circle2 circle{point2{1.0, -3.0}, 0.5};
		segment2 seg{point2{1, -2}, vector2{1, 2}};
		vector2 velocity{-3, 0};
		BOOST_CHECK( !moving_sphere_segment_intersection( circle, velocity, seg, t, q, cmp ) );
	}

	//! initial: intersecting line not segment, moving toward lower endpoint
	{
		circle2 circle{point2{1.0, -3.0}, 0.5};
		segment2 seg{point2{1, -2}, vector2{1, 2}};
		vector2 velocity{0, 2};
		bool result = moving_sphere_segment_intersection( circle, velocity, seg, t, q, cmp );
		BOOST_CHECK( result );
		BOOST_CHECK( numeric_sequence_equals( q, point2{1, -2}, cmp ) );
		BOOST_CHECK( cmp.equals( t, 0.25 ) );
	}

	//! initial: intersecting line not segment, moving toward upper endpoint
	{
		circle2 circle{point2{1.0, 3.0}, 0.5};
		segment2 seg{point2{1, -2}, vector2{1, 2}};
		vector2 velocity{0, -2};
		bool result = moving_sphere_segment_intersection( circle, velocity, seg, t, q, cmp );
		BOOST_CHECK( result );
		BOOST_CHECK( numeric_sequence_equals( q, point2{1, 2}, cmp ) );
		BOOST_CHECK( cmp.equals( t, 0.25 ) );
	}

	//! initial: not intersecting line nor segment, moving toward upper endpoint
	{
		circle2 circle{point2{2.0, 3.0}, 0.5};
		segment2 seg{point2{1, -2}, vector2{1, 2}};
		vector2 velocity{-2, -2};
		bool result = moving_sphere_segment_intersection( circle, velocity, seg, t, q, cmp );
		BOOST_CHECK( result );
		BOOST_CHECK( numeric_sequence_equals( q, point2{1, 2}, cmp ) );
		BOOST_CHECK( cmp.equals( t, 0.32322330470336319 ) );
	}

	//! initial: not intersecting line nor segment, moving toward lower endpoint
	{
		circle2 circle{point2{2.0, -3.0}, 0.5};
		segment2 seg{point2{1, -2}, vector2{1, 2}};
		vector2 velocity{-2, 2};
		bool result = moving_sphere_segment_intersection( circle, velocity, seg, t, q, cmp );
		BOOST_CHECK( result );
		BOOST_CHECK( numeric_sequence_equals( q, point2{1, -2}, cmp ) );
		BOOST_CHECK( cmp.equals( t, 0.32322330470336319 ) );
	}

	//! Sample bug 1
	{
		segment2 seg{55.84506916673854, 23.547610300593078, 56.529054251296813, -23.077372963791056};
		
		double radius = 0.31111750477426175;
		point2 position = point2{56.752395087297181, -23.585190612055708};
		vector2 velocity = vector2{-0.03648659998106174, 2.7389785331583272};
		circle2 circle{position, radius};
		
		bool result = moving_sphere_segment_intersection( circle, velocity, seg, t, q, cmp );
		BOOST_CHECK( result );
		BOOST_CHECK( numeric_sequence_equals( q, point2{56.529054251296813, -23.077372963791053}, cmp ) );
		BOOST_CHECK( cmp.equals( t, 0.10490959954263361 ) );
	}

	//! Sample bug 2 - stationary velocity.
	{
		segment2 seg{55.84506916673854, 23.547610300593078, 56.529054251296813, -23.077372963791056};

		double radius = 0.31111750477426175;
		point2 position = point2{56.752395087297181, -23.585190612055708};
		vector2 velocity = vector2{0,0};
		circle2 circle{position, radius};

		auto result = moving_sphere_segment_intersection( circle, velocity, seg, t, q, cmp );
		BOOST_CHECK( !result );
		BOOST_CHECK( cmp.equals( t, 0 ) );
	}

	//! Sample bug 3 - hits middle initially but reports incorrectly.
	{
		segment2 seg{-5.1316132118228097, 2.5356948795595144, -5.2326372913244086, -1.6241013071471571};
		point2 position{-4.8906096250382545, -0.24123259784358375};
		vector2 velocity{-2.5483733948031464, -3.6937969925168712};
		double radius = 0.30835263973557986;
		circle2 circle{position, radius};
		auto result = moving_sphere_segment_intersection( circle, velocity, seg, t, q, cmp );
		BOOST_CHECK( result.is_intersecting() && result.is_on_line_at_start() );
		BOOST_CHECK( cmp.equals( t, 0 ) );
	}

	//! Sample bug 4
	{
		segment2 seg{2.6941811136190381, -2.9910271024389203, -1.0194636759712239, 2.654467593997424};
		point2 position{3.0585517353902505, -2.9145958000370644};
		vector2 velocity{-0.72444262735718412, -2.0398104426204622};
		double radius = 0.31777965955349585;
		circle2 circle{position, radius};
		auto result = moving_sphere_segment_intersection( circle, velocity, seg, t, q, cmp );
		BOOST_CHECK( !result );
	}

	//! result type test
	{
		moving_sphere_segment_intersection_result r( true, true, true, false, false );
		BOOST_CHECK( r.is_intersecting() );
		BOOST_CHECK( r.is_on_line_at_start() );
		BOOST_CHECK( r.is_endpoint() );
	}

	{
		moving_sphere_segment_intersection_result r( true, true, false, false, false );
		BOOST_CHECK( r.is_intersecting() );
		BOOST_CHECK( r.is_on_line_at_start() );
		BOOST_CHECK( !r.is_endpoint() );
	}

	{
		moving_sphere_segment_intersection_result r( true, false, true, false, false );
		BOOST_CHECK( r.is_intersecting() );
		BOOST_CHECK( !r.is_on_line_at_start() );
		BOOST_CHECK( r.is_endpoint() );
	}

	{
		moving_sphere_segment_intersection_result r( true, false, false, false, false );
		BOOST_CHECK( r.is_intersecting() );
		BOOST_CHECK( !r.is_on_line_at_start() );
		BOOST_CHECK( !r.is_endpoint() );
	}
}

#include <geometrix/algorithm/intersection/segment_polyline_intersection.hpp>
#include <geometrix/primitive/vector_point_sequence.hpp>
BOOST_AUTO_TEST_CASE(TestSegmentPolylineIntersections)
{
	using namespace geometrix;

	typedef point_double_2d point2;
	typedef segment_double_2d segment2;
	typedef polyline<point2> polyline2;
	absolute_tolerance_comparison_policy<double> cmp(1e-10);
	std::vector<std::tuple<intersection_type, std::size_t, std::size_t, point2, point2>> intersections;
	auto visitor = [&intersections](intersection_type iType, std::size_t i, std::size_t j, point2 xPoint0, point2 xPoint1) -> bool
	{
		intersections.emplace_back(iType, i, j, xPoint0, xPoint1);
		return false;//! keep going.
	};

	{
		polyline2 geometry;
		geometry.push_back(point2(0., 0.));
		geometry.push_back(point2(10., 0.));
		geometry.push_back(point2(15., 5.));
		geometry.push_back(point2(10., 10.));
		geometry.push_back(point2(0., 10.));
		geometry.push_back(point2(5., 5.));

		segment2 segment{-1.0, -1.0, -2.0, -2.0};

		BOOST_CHECK(segment_polyline_intersect(segment, geometry, visitor, cmp) == false);
		BOOST_CHECK(intersections.empty());

		segment2 segment2{ 10.0, 0.0, 15.0, 5.0 };		

		BOOST_CHECK(segment_polyline_intersect(segment2, geometry, visitor, cmp) == true);
		BOOST_CHECK(intersections.size() == 3);
	}
}

BOOST_AUTO_TEST_CASE(TestSegmentCapsuleIntersection)
{
	using namespace geometrix;

	typedef point_double_2d point2;
	typedef vector_double_2d vector2;
	typedef segment_double_2d segment2;

	ignore_unused_warning_of<vector2>();

	point2 a, b, c, d, xPoint;
	double r, t;
	absolute_tolerance_comparison_policy<double> cmp(1e-10);

	//! Endpoint a intersection from outside segment.
	{
		c = point2{1.0, 1.0};
		d = point2{ 4.0, 1.0 };
		r = 0.5;
		a = point2{ 0.0, 0.0 };
		b = point2{ 2.0, 3.0 };
		
		bool result = segment_capsule_intersection(a, b, c, d, r, t, xPoint, cmp);
		segment2 ab = { a, b };
		segment2 cd = { c, d };
		point2 q = a + t * (b - a);
		BOOST_CHECK(numeric_sequence_equals(q, xPoint, cmp));
		BOOST_CHECK(numeric_sequence_equals(q, point2{ 0.53846153846153821, 0.80769230769230727 }, cmp));
		auto qr = make_sphere(q, r);
		auto ar = make_sphere(a, r);
		auto br = make_sphere(b, r);
		auto cr = make_sphere(c, r);
		auto dr = make_sphere(d, r);
		BOOST_CHECK(result);
	}

	//! Intersection inside.
	{
		c = point2{ 1.0, 1.0 };
		d = point2{ 4.0, 1.0 };
		r = 0.5;
		a = point2{ 1.25, 0.0 };
		b = point2{ 1.25, 3.0 };

		bool result = segment_capsule_intersection(a, b, c, d, r, t, xPoint, cmp);
		segment2 ab = { a, b };
		segment2 cd = { c, d };
		point2 q = a + t * (b - a);
		BOOST_CHECK(numeric_sequence_equals(q, xPoint, cmp));
		BOOST_CHECK(numeric_sequence_equals(q, point2{ 1.2500000000000000, 0.50000000000000000 }, cmp));
		auto qr = make_sphere(q, r);
		auto ar = make_sphere(a, r);
		auto br = make_sphere(b, r);
		auto cr = make_sphere(c, r);
		auto dr = make_sphere(d, r);
		BOOST_CHECK(result);
	}

	//! Intersect b endpoint.
	{
		c = point2{ 1.0, 1.0 };
		d = point2{ 4.0, 1.0 };
		r = 0.5;
		a = point2{ 4.25, 0.0 };
		b = point2{ 4.25, 3.0 };

		bool result = segment_capsule_intersection(a, b, c, d, r, t, xPoint, cmp);
		segment2 ab = { a, b };
		segment2 cd = { c, d };
		point2 q = a + t * (b - a);
		BOOST_CHECK(numeric_sequence_equals(q, xPoint, cmp));
		BOOST_CHECK(numeric_sequence_equals(q, point2{ 4.2500000000000000, 0.56698729810778070 }, cmp));
		auto qr = make_sphere(q, r);
		auto ar = make_sphere(a, r);
		auto br = make_sphere(b, r);
		auto cr = make_sphere(c, r);
		auto dr = make_sphere(d, r);
		BOOST_CHECK(result);
	}
}

#include <geometrix/algorithm/intersection/ray_aabb_intersection.hpp>
BOOST_AUTO_TEST_CASE(TestRayAABBIntersection)
{
	using namespace geometrix;

	typedef point_double_2d point2;
	typedef vector_double_2d vector2;
	typedef segment_double_2d segment2;
	typedef axis_aligned_bounding_box<point2> aabb2;

	ignore_unused_warning_of<segment2>();

	point2 xPoint;
	double t;
	absolute_tolerance_comparison_policy<double> cmp(1e-10);

	{
		aabb2 bb(point2{ 1,1 }, point2{ 2,2 });
		vector2 d = normalize<vector2>(vector2{ 1,1 });
		point2 a{ 0,0 };
		bool result = ray_aabb_intersection(a, d, bb, t, xPoint, cmp);
		BOOST_CHECK(result);
		BOOST_CHECK(numeric_sequence_equals(xPoint, point2{ 1,1 }, cmp));
		BOOST_CHECK(cmp.equals(t, sqrt(2.0)));
	}

}

#include <geometrix/algorithm/intersection/moving_sphere_aabb_intersection.hpp>
BOOST_AUTO_TEST_CASE(TestMovingCircleAABBIntersection)
{
	using namespace geometrix;

	typedef point_double_2d point2;
	typedef vector_double_2d vector2;
	typedef circle_double_2d circle2;
	typedef segment_double_2d segment2;
	typedef axis_aligned_bounding_box<point2> aabb2;
	typedef polygon<point2> polygon2;

	ignore_unused_warning_of<segment2>();

	double t;
	point2 q;
	absolute_tolerance_comparison_policy<double> cmp(1e-10);

	//! General case intersecting.
	{
		circle2 circle{ point2{ 1.0, 1.0 }, 0.25 };
		aabb2 bb{ point2{ -1, -1 }, point2{ 0, 0 } };
		polygon2 box{ point2{-1,-1}, point2{0, -1}, point2{0, 0}, point2{-1, 0} };
		vector2 velocity{ -1, -1 };

		BOOST_CHECK(moving_sphere_aabb_intersection(circle, velocity, bb, t, q, cmp));
		circle2 qr(circle.get_center() + t * velocity, circle.get_radius());
		BOOST_CHECK(numeric_sequence_equals(q, point2{ 0, 0 }, cmp));
	}

	//! Bottom case middle intersecting.
	{
		circle2 circle{ point2{ -0.5, -2.0 }, 0.25 };
		aabb2 bb{ point2{ -1, -1 }, point2{ 0, 0 } };
		polygon2 box{ point2{ -1,-1 }, point2{ 0, -1 }, point2{ 0, 0 }, point2{ -1, 0 } };
		vector2 velocity{ 0, 1 };

		BOOST_CHECK(moving_sphere_aabb_intersection(circle, velocity, bb, t, q, cmp));
		circle2 qr(circle.get_center() + t * velocity, circle.get_radius());
		BOOST_CHECK(numeric_sequence_equals(q, point2{ -0.50000000000000000, -1.0000000000000000 }, cmp));
	}

	//! Side case middle intersecting.
	{
		circle2 circle{ point2{ -2.0, -0.5 }, 0.25 };
		aabb2 bb{ point2{ -1, -1 }, point2{ 0, 0 } };
		polygon2 box{ point2{ -1,-1 }, point2{ 0, -1 }, point2{ 0, 0 }, point2{ -1, 0 } };
		vector2 velocity{ 1, 0 };

		BOOST_CHECK(moving_sphere_aabb_intersection(circle, velocity, bb, t, q, cmp));
		circle2 qr(circle.get_center() + t * velocity, circle.get_radius());
		BOOST_CHECK(numeric_sequence_equals(q, point2{ -1.00000000000000000, -0.5000000000000000 }, cmp));
	}

	//! Side case angled intersecting.
	{
		circle2 circle{ point2{ -2.0, -1.0 }, 0.25 };
		aabb2 bb{ point2{ -1, -1 }, point2{ 0, 0 } };
		polygon2 box{ point2{ -1,-1 }, point2{ 0, -1 }, point2{ 0, 0 }, point2{ -1, 0 } };
		vector2 velocity{ 1, 1 };

		BOOST_CHECK(moving_sphere_aabb_intersection(circle, velocity, bb, t, q, cmp));
		circle2 qr(circle.get_center() + t * velocity, circle.get_radius());
		BOOST_CHECK(numeric_sequence_equals(q, point2{ -1.0000000000000000, -0.25000000000000000 }, cmp));
	}

	//! Side case overlapping intersecting.
	{
		circle2 circle{ point2{ -1.25, -1.75 }, 0.25 };
		aabb2 bb{ point2{ -1, -1 }, point2{ 0, 0 } };
		polygon2 box{ point2{ -1,-1 }, point2{ 0, -1 }, point2{ 0, 0 }, point2{ -1, 0 } };
		vector2 velocity{ 0, 1 };

		BOOST_CHECK(moving_sphere_aabb_intersection(circle, velocity, bb, t, q, cmp));
		circle2 qr(circle.get_center() + t * velocity, circle.get_radius());
		BOOST_CHECK(numeric_sequence_equals(q, point2{ -1, -1 }, cmp));
	}

	//! Passing corner at 45.
	{
		double radius = 0.25;
		double sqrt2 = sqrt(2.0);
		vector2 velocity{ 1, -1 };
		circle2 circle{ point2{ radius/sqrt2, radius / sqrt2 } - velocity, radius };
		aabb2 bb{ point2{ -1, -1 }, point2{ 0, 0 } };
		polygon2 box{ point2{ -1,-1 }, point2{ 0, -1 }, point2{ 0, 0 }, point2{ -1, 0 } };
		
		BOOST_CHECK(!moving_sphere_aabb_intersection(circle, velocity, bb, t, q, cmp));
		circle2 qr(circle.get_center() + t * velocity, circle.get_radius());
		//BOOST_CHECK(numeric_sequence_equals(q, point2{ 0, 0 }, cmp));
	}

	//! Passing corner at 45 shave.
	{
		circle2 circle{ point2{ -1., 1.25 }, 0.25 };
		aabb2 bb{ point2{ -1, -1 }, point2{ 0, 0 } };
		polygon2 box{ point2{ -1,-1 }, point2{ 0, -1 }, point2{ 0, 0 }, point2{ -1, 0 } };
		vector2 velocity{ 1, -1 };

		BOOST_CHECK(moving_sphere_aabb_intersection(circle, velocity, bb, t, q, cmp));
		circle2 qr(circle.get_center() + t * velocity, circle.get_radius());
		BOOST_CHECK(numeric_sequence_equals(q, point2{ 0, 0 }, cmp));
	}
}

#include <geometrix/algorithm/intersection/moving_sphere_obb_intersection.hpp>

template <typename Point, typename Vector, typename NumberComparisonPolicy>
bool test_obb_collision(const Point& p, double radius, const Vector& velocity, const Point& ocenter, const Vector& odirection, double& t, Point& q, const NumberComparisonPolicy& cmp)
{
	using namespace geometrix;
	typedef obb_double_2d obb2;
	typedef aabb_double_2d aabb2;
	typedef point_double_2d point2;
	typedef vector_double_2d vector2;
	typedef polygon<point2> polygon2;
	typedef segment<point2> segment2;
	typedef circle_double_2d circle2;

	obb2 obb(ocenter, odirection, left_normal(odirection), 0.5, 0.5);
	point2 ll = obb.get_right_backward_point();
	point2 ur = obb.get_left_forward_point();
	point2 lb = obb.get_left_backward_point();
	point2 rf = obb.get_right_forward_point();
	polygon2 s{ ll, rf, ur, lb };

	vector2 xAxis{ 1,0 };
	point2 ll2 = rotate_point(ll, obb.get_u(), xAxis, obb.get_center());
	point2 ur2 = rotate_point(ur, obb.get_u(), xAxis, obb.get_center());
	point2 lb2 = rotate_point(lb, obb.get_u(), xAxis, obb.get_center());
	point2 rf2 = rotate_point(rf, obb.get_u(), xAxis, obb.get_center());
	polygon2 s2{ ll2, rf2, ur2, lb2 };
	aabb2 aabb{ ll2, ur2 };
	point2 rp = rotate_point(p, obb.get_u(), xAxis, obb.get_center());
	circle2 rcircle{ rp, radius };
	vector2 rvelocity = rotate_vector(velocity, obb.get_u(), xAxis);
	segment2 step{ p, p + velocity };
	segment2 rstep{ rp, rp + rvelocity };
	moving_sphere_aabb_intersection(rcircle, rvelocity, aabb, t, q, cmp);
	circle2 cqr(rp + t * rvelocity, radius);

	circle2 circle{ p, radius };	
	bool result = moving_sphere_obb_intersection(circle, velocity, obb, t, q, cmp);
	circle2 qr(p + t * velocity, radius);
	return result;
}

BOOST_AUTO_TEST_CASE(TestRotateOBB)
{
	using namespace geometrix;
	typedef obb_double_2d obb2;
	typedef aabb_double_2d aabb2;
	typedef point_double_2d point2;
	typedef vector_double_2d vector2;
	typedef polygon<point2> polygon2;
	typedef segment<point2> segment2;
	typedef circle_double_2d circle2;
	absolute_tolerance_comparison_policy<double> cmp(1e-10);
	double t = 0;
	point2 q;

	//! Approaching face 3-0 from 45.
	{
		point2 p{ 0,0 };
		double radius = 0.25;
		point2 ocenter{ 1,1 };
		vector2 odirection = normalize<vector2>(vector2{ 1,1 });
		vector2 velocity{ 1,1 };		
		bool result = test_obb_collision(p, radius, velocity, ocenter, odirection, t, q, cmp);
		BOOST_CHECK(result);		
		BOOST_CHECK(numeric_sequence_equals(q, point2{ 0.64644660940672627, 0.64644660940672627 }, cmp));
	}

	//! Approaching face 0-1 from Below at 135.
	{
		point2 p{ 1, 0 };
		double radius = 0.25;
		point2 ocenter{ 1,1 };
		vector2 odirection = normalize<vector2>(vector2{ 1,1 });
		vector2 velocity{ -1, 1 };
		bool result = test_obb_collision(p, radius, velocity, ocenter, odirection, t, q, cmp);
		BOOST_CHECK(result);
		BOOST_CHECK(numeric_sequence_equals(q, point2{ 1.0000000000000000, 0.29289321881345259 }, cmp));
	}

	//! Approaching face 0-1 from Below at 90.
	{
		point2 p{ 1.8, -1 };
		double radius = 0.25;
		point2 ocenter{ 1,1 };
		vector2 odirection = normalize<vector2>(vector2{ 1,1 });
		vector2 velocity{ 0, 1 };
		bool result = test_obb_collision(p, radius, velocity, ocenter, odirection, t, q, cmp);
		BOOST_CHECK(result);
		BOOST_CHECK(numeric_sequence_equals(q, point2{ 1.6232233047033631, 0.91611652351681572 }, cmp));
	}

	//! Approaching face 1-2 from Side at pi.
	{
		point2 p{ 3, 1 };
		double radius = 0.25;
		point2 ocenter{ 1,1 };
		vector2 odirection = normalize<vector2>(vector2{ 1,1 });
		vector2 velocity{ -1, 0 };
		bool result = test_obb_collision(p, radius, velocity, ocenter, odirection, t, q, cmp);
		BOOST_CHECK(result);
		BOOST_CHECK(numeric_sequence_equals(q, point2{ 1.7071067811865475, 1.0000000000000000 }, cmp));
	}

	//! Approaching face 1-2 from Side at 225.
	{
		point2 p{ 2.0, 2 };
		double radius = 0.25;
		point2 ocenter{ 1,1 };
		vector2 odirection = normalize<vector2>(vector2{ 1,1 });
		vector2 velocity{ -1, -1 };
		bool result = test_obb_collision(p, radius, velocity, ocenter, odirection, t, q, cmp);
		BOOST_CHECK(result);
		BOOST_CHECK(numeric_sequence_equals(q, point2{ 1.3535533905932737, 1.3535533905932737 }, cmp));
	}

	//! Approaching face 1-2 from Top at 270.
	{
		point2 p{ 1.0, 3 };
		double radius = 0.25;
		point2 ocenter{ 1,1 };
		vector2 odirection = normalize<vector2>(vector2{ 1,1 });
		vector2 velocity{ 0, -1 };
		bool result = test_obb_collision(p, radius, velocity, ocenter, odirection, t, q, cmp);
		BOOST_CHECK(result);
		BOOST_CHECK(numeric_sequence_equals(q, point2{ 1.0000000000000000, 1.7071067811865475 }, cmp));
	}

	//! Approaching face 2-3 from Side at 315.
	{
		point2 p{ 0.0, 3 };
		double radius = 0.25;
		point2 ocenter{ 1,1 };
		vector2 odirection = normalize<vector2>(vector2{ 1,1 });
		vector2 velocity{ 1, -1 };
		bool result = test_obb_collision(p, radius, velocity, ocenter, odirection, t, q, cmp);
		BOOST_CHECK(result);
		BOOST_CHECK(numeric_sequence_equals(q, point2{ 0.99999999999999967, 1.7071067811865459 }, cmp));
	}

	//! Approaching face 2-3 from Side at 315.
	{
		point2 p{ -1, 3 };
		double radius = 0.25;
		point2 ocenter{ 1,1 };
		vector2 odirection = normalize<vector2>(vector2{ 1,1 });
		vector2 velocity{ 1, -1 };
		bool result = test_obb_collision(p, radius, velocity, ocenter, odirection, t, q, cmp);
		BOOST_CHECK(result);
		BOOST_CHECK(numeric_sequence_equals(q, point2{ 0.64644660940672616, 1.3535533905932737 }, cmp));
	}
}

#include <geometrix/algorithm/intersection/ray_segment_intersection.hpp>
BOOST_AUTO_TEST_CASE(TestRaySegmentIntersection)
{
	using namespace geometrix;
	typedef point_double_2d point2;
	typedef vector_double_2d vector2;
	typedef segment<point2> segment2;
	absolute_tolerance_comparison_policy<double> cmp(1e-10);
	double t = 0;
	point2 q[2];

	{
		point2 center{ 1, 1 };
		vector2 dir = normalize<vector2>({ 1, 1 });
		segment2 segment{ 2, 0, 0, 2 };
		intersection_type result = ray_segment_intersection(center, dir, segment, t, q, cmp);
		BOOST_CHECK(result == e_crossing);
		BOOST_CHECK(cmp.equals(t, 0));
		BOOST_CHECK(numeric_sequence_equals(q[0], point2{ 1,1 }, cmp));
	}

	{
		point2 center{ 1, 1 };
		vector2 dir = normalize<vector2>({ 1, 1 });
		segment2 segment{ 3, 0, 0, 3 };
		intersection_type result = ray_segment_intersection(center, dir, segment, t, q, cmp);
		BOOST_CHECK(result == e_crossing);
		BOOST_CHECK(cmp.equals(t, 0.70710678118654757));
		BOOST_CHECK(numeric_sequence_equals(q[0], point2{ 1.5,1.5 }, cmp));
	}

	{
		point2 center{ 1, 1 };
		vector2 dir = normalize<vector2>({ 1, 1 });
		segment2 segment{ 3, 3, 4, 4 };
		intersection_type result = ray_segment_intersection(center, dir, segment, t, q, cmp);
		BOOST_CHECK(result == e_overlapping);
		BOOST_CHECK(cmp.equals(t, 2.8284271247461898));
		BOOST_CHECK(numeric_sequence_equals(q[0], point2{ 3, 3 }, cmp));
		BOOST_CHECK(numeric_sequence_equals(q[1], point2{ 4, 4 }, cmp));
	}

	{
		point2 center{ 1, 1 };
		vector2 dir = normalize<vector2>({ 1, 1 });
		segment2 segment{ 0, 0, 4, 4 };
		intersection_type result = ray_segment_intersection(center, dir, segment, t, q, cmp);
		BOOST_CHECK(result == e_overlapping);
		BOOST_CHECK(cmp.equals(t, 0));
		BOOST_CHECK(numeric_sequence_equals(q[0], point2{ 1, 1 }, cmp));
		BOOST_CHECK(numeric_sequence_equals(q[1], point2{ 4, 4 }, cmp));
	}

	{
		point2 center{ 1, 1 };
		vector2 dir = normalize<vector2>({ 1, 1 });
		segment2 segment{ 4, 4, 0, 0 };
		intersection_type result = ray_segment_intersection(center, dir, segment, t, q, cmp);
		BOOST_CHECK(result == e_overlapping);
		BOOST_CHECK(cmp.equals(t, 0));
		BOOST_CHECK(numeric_sequence_equals(q[0], point2{ 1, 1 }, cmp));
		BOOST_CHECK(numeric_sequence_equals(q[1], point2{ 4, 4 }, cmp));
	}

	{
		point2 center{ 1, 1 };
		vector2 dir = normalize<vector2>({ 1, 1 });
		segment2 segment{ 4, 4, 4, 10 };
		intersection_type result = ray_segment_intersection(center, dir, segment, t, q, cmp);
		BOOST_CHECK(result == e_endpoint);
		BOOST_CHECK(cmp.equals(t, 4.2426406871192857));
		BOOST_CHECK(numeric_sequence_equals(q[0], point2{ 4, 4 }, cmp));
	}

	{
		point2 center{ 1, 1 };
		vector2 dir = normalize<vector2>({ -1, -1 });
		segment2 segment{ 4, 4, 4, 10 };
		intersection_type result = ray_segment_intersection(center, dir, segment, t, q, cmp);
		BOOST_CHECK(result == e_non_crossing);
	}

	{		
		vector2 dir = left_normal(normalize<vector2>({ 1, 1 }));
		point2 center{ point2{4, 4} + dir };
		segment2 segment{ 4, 4, 4, 10 };
		intersection_type result = ray_segment_intersection(center, dir, segment, t, q, cmp);
		BOOST_CHECK(result == e_non_crossing);
	}
}

#include <boost/optional.hpp>
BOOST_AUTO_TEST_CASE(TestConvertToBoostOptional)
{
	using namespace geometrix;
	typedef vector_double_2d vector2;
	typedef point_double_2d point2;

	typedef boost::optional<vector2> ovector2;

	point2 p{ 0, 0 };
	vector2 v{ 10, 10 };
	ovector2 ovp = vector2(p + 10 * v);
}

#endif //GEOMETRIX_INTERSECTION_TESTS_HPP
