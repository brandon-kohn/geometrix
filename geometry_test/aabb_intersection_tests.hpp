//
//! Copyright © 2017
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_AABB_INTERSECTION_TESTS_HPP
#define GEOMETRIX_AABB_INTERSECTION_TESTS_HPP

#include <boost/test/included/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include <geometrix/algorithm/intersection/segment_aabb_intersection.hpp>
#include <geometrix/algorithm/intersection/ray_aabb_intersection.hpp>
#include <geometrix/algorithm/intersection/polyline_aabb_intersection.hpp>
#include <geometrix/algorithm/intersection/polygon_aabb_intersection.hpp>
#include <geometrix/primitive/axis_aligned_bounding_box.hpp>

#include "2d_kernel_units_fixture.hpp"

BOOST_FIXTURE_TEST_CASE(make_aabb_from_segment, geometry_kernel_2d_units_fixture)
{
	using namespace geometrix;
    using namespace boost::units::si;
    segment2 seg{ 0.5 * meters, 0.5 * meters, 1.5 * meters, 1.5 * meters };

	auto result = make_aabb<point2>(seg);

	BOOST_CHECK(numeric_sequence_equals(seg.get_start(), result.get_lower_bound(), cmp));
	BOOST_CHECK(numeric_sequence_equals(seg.get_end(), result.get_upper_bound(), cmp));
}
BOOST_FIXTURE_TEST_CASE(AABB_Segment_Intersection_Test_2D_Intersects, geometry_kernel_2d_units_fixture)
{
    using namespace boost::units::si;
    aabb2 box({ 1.0 * meters, 1.0 * meters }, { 2.0 * meters, 2.0 * meters });
    segment2 seg{ 0.5 * meters, 0.5 * meters, 1.5 * meters, 1.5 * meters };

    auto result = segment_aabb_intersection(seg.get_start(), seg.get_end(), box);

    BOOST_CHECK(result == true);
}

BOOST_FIXTURE_TEST_CASE(AABB_Segment_Intersection_Test_2D_Does_NOT_Intersect, geometry_kernel_2d_units_fixture)
{
    using namespace boost::units::si;
    aabb2 box({ 1.0 * meters, 1.0 * meters }, { 2.0 * meters, 2.0 * meters });
    segment2 seg{ 0.5 * meters, 0.5 * meters, -1.5 * meters, -1.5 * meters };

    auto result = segment_aabb_intersection(seg.get_start(), seg.get_end(), box);

    BOOST_CHECK(result == false);
}

BOOST_FIXTURE_TEST_CASE(AABB_Segment_Intersection_Test_2D_WhollyInsideBox, geometry_kernel_2d_units_fixture)
{
    using namespace boost::units::si;
    aabb2 box({ 1.0 * meters, 1.0 * meters }, { 2.0 * meters, 2.0 * meters });
    segment2 seg{ 1.5 * meters, 1.5 * meters, 1.6 * meters, 1.6 * meters };

    auto result = segment_aabb_intersection(seg.get_start(), seg.get_end(), box);

    BOOST_CHECK(result == true);
}

BOOST_FIXTURE_TEST_CASE(AABB_Segment_Intersection_Test_2D_IsAlongBorder, geometry_kernel_2d_units_fixture)
{
    using namespace boost::units::si;
    aabb2 box({ 1.0 * meters, 1.0 * meters }, { 2.0 * meters, 2.0 * meters });
    segment2 seg{ 1.0 * meters, 0.0 * meters, 1.0 * meters, 3.0 * meters };

    auto result = segment_aabb_intersection(seg.get_start(), seg.get_end(), box);

    BOOST_CHECK(result == true);
}

BOOST_FIXTURE_TEST_CASE(AABB_Polyline_Intersection_Test_2D_SyntaxCheck, geometry_kernel_2d_units_fixture)
{
    using namespace boost::units::si;
    aabb2 box({ 1.0 * meters, 1.0 * meters }, { 2.0 * meters, 2.0 * meters });
    polyline2 pline{ {1.0 * meters, 0.0 * meters}, {1.0 * meters, 3.0 * meters} };

    auto result = polyline_aabb_intersection(pline, box);

    BOOST_CHECK(result == true);
}

BOOST_FIXTURE_TEST_CASE(AABB_Polyline_Intersection_Test_2D_PolylineInsideBox, geometry_kernel_2d_units_fixture)
{
    using namespace boost::units::si;
    aabb2 box({ 1.0 * meters, 1.0 * meters }, { 2.0 * meters, 2.0 * meters });
    polyline2 pline{ { 1.1 * meters, 1.1 * meters }, { 1.9 * meters, 1.1 * meters }, { 1.9 * meters, 1.9 * meters },{ 1.1 * meters, 1.9 * meters } };

    auto result = polyline_aabb_intersection(pline, box);

    BOOST_CHECK(result == true);
}

BOOST_FIXTURE_TEST_CASE(AABB_Polyline_Intersection_Test_2D_PolylineOutsideBox, geometry_kernel_2d_units_fixture)
{
    using namespace boost::units::si;
    aabb2 box({ 1.0 * meters, 1.0 * meters }, { 2.0 * meters, 2.0 * meters });
    polyline2 pline{ { 0.9 * meters, 0.9 * meters },{ 2.1 * meters, 0.9 * meters },{ 2.1 * meters, 2.1 * meters },{ 0.9 * meters, 2.1 * meters } };

    auto result = polyline_aabb_intersection(pline, box);

    BOOST_CHECK(result == false);
}

BOOST_FIXTURE_TEST_CASE(AABB_Polygon_Intersection_Test_2D_SyntaxCheck, geometry_kernel_2d_units_fixture)
{
    using namespace boost::units::si;
    aabb2 box({ 1.0 * meters, 1.0 * meters }, { 2.0 * meters, 2.0 * meters });
    polygon2 pgon{ { 1.0 * meters, 0.0 * meters },{ 1.0 * meters, 3.0 * meters }, {0.0 * meters, 0.0 * meters} };

    auto result = polygon_aabb_intersection(pgon, box);

    BOOST_CHECK(result == true);
}

BOOST_FIXTURE_TEST_CASE(AABB_Polygon_Intersection_Test_2D_PolygonInsideBox, geometry_kernel_2d_units_fixture)
{
    using namespace boost::units::si;
    aabb2 box({ 1.0 * meters, 1.0 * meters }, { 2.0 * meters, 2.0 * meters });
    polygon2 pgon{ { 1.1 * meters, 1.1 * meters },{ 1.9 * meters, 1.1 * meters },{ 1.9 * meters, 1.9 * meters },{ 1.1 * meters, 1.9 * meters } };

    auto result = polygon_aabb_intersection(pgon, box);

    BOOST_CHECK(result == true);
}

BOOST_FIXTURE_TEST_CASE(AABB_Polygon_Intersection_Test_2D_PolygonOutsideBoxOverlapping, geometry_kernel_2d_units_fixture)
{
    using namespace boost::units::si;
    aabb2 box({ 1.0 * meters, 1.0 * meters }, { 2.0 * meters, 2.0 * meters });
    polygon2 pgon{ { 0.9 * meters, 0.9 * meters },{ 2.1 * meters, 0.9 * meters },{ 2.1 * meters, 2.1 * meters },{ 0.9 * meters, 2.1 * meters } };

    auto result = polygon_aabb_intersection(pgon, box);

    BOOST_CHECK(result == true);
}

BOOST_FIXTURE_TEST_CASE(AABB_Polygon_BoundingBox_Intersection_Test_2D_PolygonOutsideBoxOverlapping, geometry_kernel_2d_units_fixture)
{
    using namespace geometrix;
    using namespace boost::units::si;
    aabb2 box({ 1.0 * meters, 1.0 * meters }, { 2.0 * meters, 2.0 * meters });
    polygon2 pgon{ { 0.9 * meters, 0.9 * meters },{ 2.1 * meters, 0.9 * meters },{ 2.1 * meters, 2.1 * meters },{ 0.9 * meters, 2.1 * meters } };

    auto result = box.intersects(make_aabb<point2>(pgon));

    BOOST_CHECK(result == true);
}

BOOST_FIXTURE_TEST_CASE(AABB_Polygon_BoundingBox_Intersection_Test_2D_PolygonInsideBoxOverlapping, geometry_kernel_2d_units_fixture)
{
    using namespace geometrix;
    using namespace boost::units::si;
    aabb2 box({ 1.0 * meters, 1.0 * meters }, { 2.0 * meters, 2.0 * meters });
    polygon2 pgon{ { 1.1 * meters, 1.1 * meters },{ 1.9 * meters, 1.1 * meters },{ 1.9 * meters, 1.9 * meters },{ 1.1 * meters, 1.9 * meters } };

    auto result = box.intersects(make_aabb<point2>(pgon));

    BOOST_CHECK(result == true);
}

BOOST_FIXTURE_TEST_CASE(AABB_Polygon_BoundingBox_Contains_Test_2D_PolygonInsideBoxOverlapping, geometry_kernel_2d_units_fixture)
{
    using namespace geometrix;
    using namespace boost::units::si;
    aabb2 box({ 1.0 * meters, 1.0 * meters }, { 2.0 * meters, 2.0 * meters });
    polygon2 pgon{ { 1.1 * meters, 1.1 * meters },{ 1.9 * meters, 1.1 * meters },{ 1.9 * meters, 1.9 * meters },{ 1.1 * meters, 1.9 * meters } };

    auto result = box.contains(make_aabb<point2>(pgon));

    BOOST_CHECK(result == true);
}

BOOST_FIXTURE_TEST_CASE(AABB_Ray_Intersection_Test_2D_Intersects, geometry_kernel_2d_units_fixture)
{
	using namespace geometrix;
	using namespace boost::units::si;
	aabb2 box({ 1.0 * meters, 1.0 * meters }, { 2.0 * meters, 2.0 * meters });
	segment2 seg{ 0.5 * meters, 0.5 * meters, 1.5 * meters, 1.5 * meters };
	point2 oray = seg.get_start();
	dimensionless2 vray = normalize(seg.get_end() - seg.get_start());
	auto t0 = 0.0 * meters;
	auto t1 = 0.0 * meters;
	auto q0 = point2{};
	auto q1 = point2{};
	auto result = ray_aabb_intersection(oray, vray, box, t0, q0, t1, q1, cmp);

	BOOST_CHECK(result == true);
	BOOST_CHECK(numeric_sequence_equals(q0, box.get_lower_bound(), cmp));
	BOOST_CHECK(numeric_sequence_equals(q1, box.get_upper_bound(), cmp));
}

BOOST_FIXTURE_TEST_CASE(AABB_Ray_Intersection_Test_2D_IntersectsCorner, geometry_kernel_2d_units_fixture)
{
	using namespace geometrix;
	using namespace boost::units::si;
	aabb2 box({ 2.0 * meters, 1.0 * meters }, { 3.0 * meters, 2.0 * meters });
	segment2 seg{ 0.5 * meters, 0.5 * meters, 1.5 * meters, 1.5 * meters };
	point2 oray = seg.get_start();
	dimensionless2 vray = normalize(seg.get_end() - seg.get_start());
	auto t0 = 0.0 * meters;
	auto t1 = 0.0 * meters;
	auto q0 = point2{};
	auto q1 = point2{};
	auto result = ray_aabb_intersection(oray, vray, box, t0, q0, t1, q1, cmp);

	BOOST_CHECK(result == true);
	BOOST_CHECK(numeric_sequence_equals(q0, box[3], cmp));
	BOOST_CHECK(numeric_sequence_equals(q1, q0, cmp));
}

BOOST_FIXTURE_TEST_CASE(AABB_Ray_Intersection_Test_2D_IntersectsSide, geometry_kernel_2d_units_fixture)
{
	using namespace geometrix;
	using namespace boost::units::si;
	aabb2 box({ 2.0 * meters, 1.5 * meters }, { 3.0 * meters, 2.5 * meters });
	segment2 seg{ 0.5 * meters, 0.5 * meters, 1.5 * meters, 1.5 * meters };
	point2 oray = seg.get_start();
	dimensionless2 vray = normalize(seg.get_end() - seg.get_start());
	auto t0 = 0.0 * meters;
	auto t1 = 0.0 * meters;
	auto q0 = point2{};
	auto q1 = point2{};
	auto result = ray_aabb_intersection(oray, vray, box, t0, q0, t1, q1, cmp);

	BOOST_CHECK(result == true);
	BOOST_CHECK(!result.is_origin_inside());
	BOOST_CHECK(numeric_sequence_equals(q0, point2{ 2.0 * meters, 2.0 * meters }, cmp));
	BOOST_CHECK(numeric_sequence_equals(q1, point2{ 2.5 * meters, 2.5 * meters }, cmp));
}

BOOST_FIXTURE_TEST_CASE(AABB_Ray_Intersection_Test_2D_IntersectsFromInside, geometry_kernel_2d_units_fixture)
{
	using namespace geometrix;
	using namespace boost::units::si;
	aabb2 box({ 1.0 * meters, 1.0 * meters }, { 2.0 * meters, 2.0 * meters });
	segment2 seg{ 1.5 * meters, 1.5 * meters, 2.5 * meters, 2.5 * meters };
	point2 oray = seg.get_start();
	dimensionless2 vray = normalize(seg.get_end() - seg.get_start());
	auto t0 = 0.0 * meters;
	auto t1 = 0.0 * meters;
	auto q0 = point2{};
	auto q1 = point2{};
	auto result = ray_aabb_intersection(oray, vray, box, t0, q0, t1, q1, cmp);

	BOOST_CHECK(result == true);
	BOOST_CHECK(result.is_origin_inside());
	BOOST_CHECK(numeric_sequence_equals(q0, oray, cmp));
	BOOST_CHECK(numeric_sequence_equals(q1, box.get_upper_bound(), cmp));
}

BOOST_FIXTURE_TEST_CASE(AABB_Ray_Intersection_Test_2D_IntersectsFromInsideOnlyOnePoint, geometry_kernel_2d_units_fixture)
{
	using namespace geometrix;
	using namespace boost::units::si;
	aabb2 box({ 1.0 * meters, 1.0 * meters }, { 2.0 * meters, 2.0 * meters });
	segment2 seg{ 1.5 * meters, 1.5 * meters, 2.5 * meters, 2.5 * meters };
	point2 oray = seg.get_start();
	dimensionless2 vray = normalize(seg.get_end() - seg.get_start());
	auto t0 = 0.0 * meters;
	auto q0 = point2{};
	auto result = ray_aabb_intersection(oray, vray, box, t0, q0, cmp);

	BOOST_CHECK(result == true);
	BOOST_CHECK(result.is_origin_inside());
	BOOST_CHECK(numeric_sequence_equals(q0, box.get_upper_bound(), cmp));
}

BOOST_FIXTURE_TEST_CASE(AABB_Ray_Intersection_Test_2D_RAY_UP_IntersectsFromInsideOnlyOnePoint, geometry_kernel_2d_units_fixture)
{
	using namespace geometrix;
	using namespace boost::units::si;
	aabb2 box({ 1.0 * meters, 1.0 * meters }, { 2.0 * meters, 2.0 * meters });
	segment2 seg{ 1.5 * meters, 1.5 * meters, 1.5 * meters, 2.5 * meters };
	point2 oray = seg.get_start();
	dimensionless2 vray = normalize(seg.get_end() - seg.get_start());
	auto t0 = 0.0 * meters;
	auto q0 = point2{};
	auto result = ray_aabb_intersection(oray, vray, box, t0, q0, cmp);

	BOOST_CHECK(result == true);
	BOOST_CHECK(result.is_origin_inside());
    BOOST_CHECK(numeric_sequence_equals(q0, point2 { 1.5 * meters, 2.0 * meters }, cmp));
}

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
        aabb2 bb(point2 { 1, 1 }, point2 { 2, 2 });
        vector2 d = normalize(vector2 { 1, 1 });
        point2 a { 0, 0 };
        bool result = ray_aabb_intersection(a, d, bb, t, xPoint, cmp);
        BOOST_CHECK(result);
        BOOST_CHECK(numeric_sequence_equals(xPoint, point2 { 1, 1 }, cmp));
        BOOST_CHECK(cmp.equals(t, sqrt(2.0)));
    }
}

BOOST_AUTO_TEST_CASE(TestRayAABBIntersection_RayMovingAway_NoIntersection)
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
        aabb2 bb(point2 { 1, 1 }, point2 { 2, 2 });
        vector2 d = normalize(vector2 { 1, 1 });
        point2 a { 3, 3 };
        bool result = ray_aabb_intersection(a, d, bb, t, xPoint, cmp);
        BOOST_CHECK(!result);
    }
}

BOOST_AUTO_TEST_CASE(TestRayAABBIntersection_RayMovingAway2_NoIntersection)
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
        auto bb = aabb2 { point2 { -2324.3546986196538, -2841.7932822329558 }, point2 { -2320.3534799742397, -2839.2931625920946 } };
        auto d = vector2 { -0.0064465485759405998, 0.021591448562243244 };
        auto a = point2 { -2321.5064261645903, -2834.2385472663891 };
        bool result = ray_aabb_intersection(a, d, bb, t, xPoint, cmp);
        BOOST_CHECK(!result);
    }
}

#endif//! GEOMETRIX_AABB_INTERSECTION_TESTS_HPP
