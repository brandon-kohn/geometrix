///////////////////////////////////////////////////////////////////////////////
// intersection_tests.cpp
//
//  Copyright 2013 Brandon Kohn. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "./2d_kernel_fixture.hpp"

#include <geometrix/algorithm/intersection/convex_polygon_polygon_intersection.hpp>

#include <geometrix/utility/utilities.hpp>
#include <geometrix/utility/scope_timer.ipp>

#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/algorithm_ext/erase.hpp>

#include <exception>
#include <iostream>

//! Google tests
TEST_F(geometry_kernel_2d_fixture, convex_rectangle_polygon_intersection_simple_test_not_intersecting)
{
    using namespace geometrix;

    auto box = rectangle2{ { point2 { 0.0, 0.0 }, point2 { 1.0, 0.0 }, point2 { 1.0, 1.0 }, point2 { 0.0, 1.0 } } };
    
    auto trig = polygon2
    {
        {1.1, 0.5}
      , {2.1, 0.5}
      , {2.1, 2.0}
    };

    auto visitor = [](const point2& p) {};
    auto result = convex_polygon_polygon_intersection(box, trig, visitor, cmp);

    EXPECT_EQ(polygon_intersection_type::none, result);
}

TEST_F(geometry_kernel_2d_fixture, convex_polygon_polygon_intersection_simple_test_not_intersecting)
{
    using namespace geometrix;

    auto box = polygon2
    {
        {0.0, 0.0}
      , {1.0, 0.0}
      , {1.0, 1.0}
      , {0.0, 1.0}
    };
    
    auto trig = polygon2
    {
        {1.1, 0.5}
      , {2.1, 0.5}
      , {2.1, 2.0}
    };

    auto visitor = [](const point2& p) {};
    auto result = convex_polygon_polygon_intersection(box, trig, visitor, cmp);

    EXPECT_EQ(polygon_intersection_type::none, result);
}

TEST_F(geometry_kernel_2d_fixture, convex_polygon_polygon_intersection_simple_test_intersects_vertex)
{
    using namespace geometrix;

    auto box = polygon2
    {
        {0.0, 0.0}
      , {1.0, 0.0}
      , {1.0, 1.0}
      , {0.0, 1.0}
    };
    
    auto trig = polygon2
    {
        {1.0, 0.5}
      , {2.0, 0.5}
      , {2.0, 2.0}
    };

    std::vector<point2> points;
	auto                visitor = [&]( const point2& p )
	{
		points.emplace_back( p );
	};
    auto result = convex_polygon_polygon_intersection(box, trig, visitor, cmp);

    EXPECT_EQ(polygon_intersection_type::overlapping, result);
    EXPECT_EQ(1, points.size());
}

TEST_F(geometry_kernel_2d_fixture, convex_polygon_polygon_intersection_simple_test_intersects_border_vertex_inside)
{
    using namespace geometrix;

    auto box = polygon2
    {
        {0.0, 0.0}
      , {1.0, 0.0}
      , {1.0, 1.0}
      , {0.0, 1.0}
    };
    
    auto trig = polygon2
    {
        {0.0, 0.5}
      , {1.0, 0.5}
      , {0.0, 1.0}
    };

    std::vector<point2> points;
	auto                visitor = [&]( const point2& p )
	{
		points.emplace_back( p );
	};
    auto result = convex_polygon_polygon_intersection(box, trig, visitor, cmp);

    EXPECT_EQ(polygon_intersection_type::overlapping, result);
    EXPECT_EQ(3, points.size());
}

TEST_F(geometry_kernel_2d_fixture, convex_polygon_polygon_intersection_simple_test_intersects_border_twice_inside)
{
    using namespace geometrix;

    auto box = polygon2
    {
        {0.0, 0.0}
      , {1.0, 0.0}
      , {1.0, 1.0}
      , {0.0, 1.0}
    };
    
    auto trig = polygon2
    {
        {0.5, 0.5}
      , {1.0, 0.5}
      , {0.5, 1.0}
    };

    std::vector<point2> points;
	auto                visitor = [&]( const point2& p )
	{
		points.emplace_back( p );
	};
    auto result = convex_polygon_polygon_intersection(box, trig, visitor, cmp);

    EXPECT_EQ(polygon_intersection_type::overlapping, result);
    EXPECT_EQ(3, points.size());
}

TEST_F(geometry_kernel_2d_fixture, convex_polygon_polygon_intersection_simple_test_intersects_face)
{
    using namespace geometrix;

    auto box = polygon2
    {
        {0.0, 0.0}
      , {1.0, 0.0}
      , {1.0, 1.0}
      , {0.0, 1.0}
    };
    
    auto trig = polygon2
    {
        {1.0, 0.5}
      , {2.0, 1.0}
      , {1.0, 1.0}
    };

    std::vector<point2> points;
	auto                visitor = [&]( const point2& p )
	{
		points.emplace_back( p );
	};
    auto result = convex_polygon_polygon_intersection(box, trig, visitor, cmp);

    EXPECT_EQ(polygon_intersection_type::face, result);
    EXPECT_EQ(2, points.size());
}

TEST_F(geometry_kernel_2d_fixture, convex_polygon_polygon_intersection_simple_test_intersects_trig_not_face)
{
    using namespace geometrix;

    auto box = polygon2
    {
        {0.0, 1.0}
      , {0.0, 0.0}
      , {1.0, 0.0}
      , {1.0, 1.0}
    };
    
    auto trig = polygon2
    {
        {1.0, 1.0}
      , {0.0, 1.0}
      , {0.0, 0.5}
    };

    std::vector<point2> points;
	auto                visitor = [&]( const point2& p )
	{
		points.emplace_back( p );
	};
    auto result = convex_polygon_polygon_intersection(box, trig, visitor, cmp);

    EXPECT_EQ(polygon_intersection_type::overlapping, result);
    EXPECT_EQ(3, points.size());
}

TEST_F(geometry_kernel_2d_fixture, convex_polygon_polygon_intersection_simple_test_intersects_surface)
{
    using namespace geometrix;

    auto box = polygon2
    {
        {0.0, 0.0}
      , {1.0, 0.0}
      , {1.0, 1.0}
      , {0.0, 1.0}
    };
    
    auto box2 = polygon2
    {
        {0.5, 0.5}
      , {1.5, 0.5}
      , {1.5, 1.5}
      , {0.5, 1.5}
    };

    std::vector<point2> points;
	auto                visitor = [&]( const point2& p )
	{
		points.emplace_back( p );
	};
    auto result = convex_polygon_polygon_intersection(box, box2, visitor, cmp);

    EXPECT_EQ(polygon_intersection_type::overlapping, result);
    EXPECT_EQ(4, points.size());
}

TEST_F(geometry_kernel_2d_fixture, convex_polygon_polygon_intersection_simple_test_intersects_wholly_contained_surface)
{
    using namespace geometrix;

    auto box = polygon2
    {
        {0.0, 0.0}
      , {1.0, 0.0}
      , {1.0, 1.0}
      , {0.0, 1.0}
    };
    
    auto box2 = polygon2
    {
        {0.25, 0.25}
      , {0.75, 0.25}
      , {0.75, 0.75}
      , {0.25, 0.75}
    };

    std::vector<point2> points;
	auto                visitor = [&]( const point2& p )
	{
		points.emplace_back( p );
	};
    auto result = convex_polygon_polygon_intersection(box, box2, visitor, cmp);

    EXPECT_EQ(polygon_intersection_type::overlapping, result);
    EXPECT_EQ(4, points.size());
}

TEST_F(geometry_kernel_2d_fixture, convex_polygon_polygon_intersection_simple_test_intersects_wholly_contained_surface_diamond)
{
    using namespace geometrix;

    auto box = polygon2
    {
        {0.0, 0.0}
      , {1.0, 0.0}
      , {1.0, 1.0}
      , {0.0, 1.0}
    };
    
    auto box2 = polygon2
    {
        {0.5, 0.0}
      , {1.0, 0.5}
      , {0.5, 1.0}
      , {0.0, 0.5}
    };

    std::vector<point2> points;
	auto                visitor = [&]( const point2& p )
	{
		points.emplace_back( p );
	};
    auto result = convex_polygon_polygon_intersection(box, box2, visitor, cmp);

    EXPECT_EQ(polygon_intersection_type::overlapping, result);
    EXPECT_EQ(4, points.size());
}

TEST_F(geometry_kernel_2d_fixture, convex_polygon_polygon_intersection_case1)
{
    using namespace geometrix;

    auto box = polygon2
    {
        {6.927962e+05, 5.338380e+06}
      , {6.927330e+05, 5.338284e+06}
      , {6.927355e+05, 5.338282e+06}
      , {6.927987e+05, 5.338378e+06}
    };
    
    auto box2 = polygon2
    {
        {6.927634e+05, 5.338334e+06}
      , {6.927617e+05, 5.338331e+06}
      , {6.927634e+05, 5.338330e+06}
      , {6.927651e+05, 5.338333e+06}
    };

    std::vector<point2> points;
    auto visitor = [&](const point2& p) 
    {
        points.emplace_back(p);
    };
    auto result = convex_polygon_polygon_intersection(box, box2, visitor, cmp);

    EXPECT_EQ(polygon_intersection_type::overlapping, result);
    EXPECT_EQ(3, points.size());
}

#include <geometrix/algorithm/distance/point_segment_closest_point.hpp>
TEST_F(geometry_kernel_2d_fixture, convex_polygon_polygon_intersection_case2)
{
    using namespace geometrix;

	auto box = polygon2{ { 692796.15455329709, 5338379.8294809163 }, { 692733.03790395218, 5338283.837264738 }, { 692735.5445882522, 5338282.1890736623 }, { 692798.66123759712, 5338378.1812898405 } };
	auto box2 = polygon2{ { 692763.40410046978, 5338333.6606555684 }, { 692761.71484111156, 5338331.0915117282 }, { 692763.38596398244, 5338329.9927176852 }, { 692765.07522334065, 5338332.5618615253 } };

    auto p0 = point_segment_closest_point( box2[3], box[0], box[1] );
    auto p1 = point_segment_closest_point( box2[2], box[0], box[1] );
	auto seg = segment2{ p0, p1 };

    std::vector<point2> points;
    auto visitor = [&](const point2& p) 
    {
        points.emplace_back(p);
    };
    auto result = convex_polygon_polygon_intersection(box, box2, visitor, cmp);

    EXPECT_EQ(polygon_intersection_type::vertex, result);
    EXPECT_EQ(1, points.size());
	EXPECT_TRUE( numeric_sequence_equals_2d( p1, box2[2], cmp ) );
}
