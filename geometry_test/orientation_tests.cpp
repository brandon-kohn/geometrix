///////////////////////////////////////////////////////////////////////////////
// orientation_tests.cpp
//
//  Copyright 2021 Brandon Kohn. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "./2d_kernel_fixture.hpp"
#include "./3d_kernel_fixture.hpp"
#include "./3d_kernel_units_fixture.hpp"

#include <geometrix/algorithm/orientation.hpp>
#include <geometrix/algorithm/distance/point_line_distance.hpp>

#include <geometrix/utility/utilities.hpp>
#include <geometrix/utility/scope_timer.ipp>

#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/algorithm_ext/erase.hpp>

#include <exception>
#include <iostream>

//! Google tests
TEST_F(geometry_kernel_2d_fixture, point_line_orientation_point_is_right_and_far_returns_right)
{
    using namespace geometrix;

    auto p1 = point2 { 0., 0. };
    auto p2 = point2 { 4., 4. };
    auto l = make_line(p1, p2);
    auto seg = make_segment(p1, p2);
    auto p = point2 { 1.0, 0.0 };
    auto r = point_line_distance(p, l) - 0.1;
    auto c = circle2 { p, r };
    auto o = circle_line_orientation(c, l, cmp);
    
    EXPECT_EQ(o, oriented_right);
}

TEST_F(geometry_kernel_2d_fixture, point_line_orientation_point_is_left_and_far_returns_left)
{
    using namespace geometrix;

    auto p1 = point2 { 0., 0. };
    auto p2 = point2 { 4., 4. };
    auto l = make_line(p1, p2);
    auto seg = make_segment(p1, p2);
    auto p = point2 { 1.0, 1.5 };
    auto r = point_line_distance(p, l) - 0.1;
    auto c = circle2 { p, r };
    auto o = circle_line_orientation(c, l, cmp);
    
    EXPECT_EQ(o, oriented_left);
}

TEST_F(geometry_kernel_2d_fixture, point_line_orientation_point_is_right_and_near_returns_collinear)
{
    using namespace geometrix;

    auto p1 = point2 { 0., 0. };
    auto p2 = point2 { 4., 4. };
    auto l = make_line(p1, p2);
    auto seg = make_segment(p1, p2);
    auto p = point2 { 1.0, 0.0 };
    auto r = point_line_distance(p, l) + 0.1;
    auto c = circle2 { p, r };
    auto o = circle_line_orientation(c, l, cmp);

    EXPECT_EQ(o, oriented_collinear);
}

#include <geometrix/algorithm/orientation.hpp>

TEST_F(geometry_kernel_2d_fixture, circle_polyline_orientation_corner_test_point_right)
{
    using namespace geometrix;
    auto pline = polyline2 { { 0.0, 0.0 }, { 1.0, 0.0 }, { 1.0, 1.0 }, { 0.0, 1.0 } };
    point2 p = pline[2] + 1.0 * normalize(vector2 { 1.0, 1.0 });
    auto r = 0.5;
    auto c = circle2 { p, r };
    auto result = circle_polyline_orientation(c, pline, cmp);
    EXPECT_EQ(result, oriented_right);
}

TEST_F(geometry_kernel_2d_fixture, circle_polyline_orientation_corner_test_point_overlaps_collinear)
{
    using namespace geometrix;
    auto pline = polyline2 { { 0.0, 0.0 }, { 1.0, 0.0 }, { 1.0, 1.0 }, { 0.0, 1.0 } };
    point2 p = pline[2] + 1.0 * normalize(vector2 { 1.0, 1.0 });
    auto r = 1.5;
    auto c = circle2 { p, r };
    auto result = circle_polyline_orientation(c, pline, cmp);
    EXPECT_EQ(result, oriented_collinear);
}

TEST_F(geometry_kernel_2d_fixture, point_segment_orientation_point_is_right)
{
    using namespace geometrix;
    auto seg = segment2{ { 0.0, 0.0 }, { 4.0, 4.0 } };
    auto p = point2{ 1.0, 0.0 };
    auto result = point_segment_orientation(p, seg, cmp);
    auto result2 = point_segment_orientation(p, seg.get_start(), seg.get_end(), cmp);
    EXPECT_EQ(result, oriented_right);
    EXPECT_EQ(result2, oriented_right);
}

TEST_F(geometry_kernel_2d_fixture, point_segment_orientation_point_is_left)
{
    using namespace geometrix;
    auto seg = segment2{ { 0.0, 0.0 }, { 4.0, 4.0 } };
    auto p = point2{ 1.0, 2.0 };
    auto result = point_segment_orientation(p, seg, cmp);
    auto result2 = point_segment_orientation(p, seg.get_start(), seg.get_end(), cmp);
    EXPECT_EQ(result, oriented_left);
    EXPECT_EQ(result2, oriented_left);
}

TEST_F(geometry_kernel_2d_fixture, point_segment_orientation_point_is_collinear)
{
    using namespace geometrix;
    auto seg = segment2{ { 0.0, 0.0 }, { 4.0, 4.0 } };
    auto p = point2{ 1.0, 1.0 };
    auto result = point_segment_orientation(p, seg, cmp);
    auto result2 = point_segment_orientation(p, seg.get_start(), seg.get_end(), cmp);
    EXPECT_EQ(result, oriented_collinear);
    EXPECT_EQ(result2, oriented_collinear);
}

TEST_F(geometry_kernel_3d_fixture, collinear_test_3d_points_true)
{
    using namespace geometrix;
	auto a = point3{ 0.0, 0.0, 0.0 };
	auto b = point3{ 1.0, 1.0, 1.0 };
	auto c = point3{ 2.0, 2.0, 2.0 };

    auto o = is_collinear( c, a, b, cmp );
	EXPECT_TRUE( o );
}

TEST_F(geometry_kernel_3d_fixture, collinear_test_3d_points_false)
{
    using namespace geometrix;
	auto a = point3{ 0.0, 0.0, 0.0 };
	auto b = point3{ 1.0, 1.0, 1.0 };
	auto c = point3{ 2.0, 2.0, 0.0 };

    auto o = is_collinear( c, a, b, cmp );
	EXPECT_FALSE( o );
}

TEST_F(geometry_kernel_3d_units_fixture, collinear_test_3d_points_false)
{
    using namespace geometrix;
	auto a = point3{ 0.0 * boost::units::si::meters, 0.0 * boost::units::si::meters, 0.0 * boost::units::si::meters };
	auto b = point3{ 1.0 * boost::units::si::meters, 1.0 * boost::units::si::meters, 1.0 * boost::units::si::meters };
	auto c = point3{ 2.0 * boost::units::si::meters, 2.0 * boost::units::si::meters, 0.0 * boost::units::si::meters };

    auto o = is_collinear( c, a, b, cmp );
	EXPECT_FALSE( o );
}
