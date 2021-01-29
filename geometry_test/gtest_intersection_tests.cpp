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
    auto visitor = [&](const point2& p) 
    {
        if (points.empty() || (!numeric_sequence_equals_2d(p, points.back(), cmp) && !numeric_sequence_equals_2d(p, points.front(), cmp)))
            points.emplace_back(p);
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
    auto visitor = [&](const point2& p) 
    {
        if (points.empty() || (!numeric_sequence_equals_2d(p, points.back(), cmp) && !numeric_sequence_equals_2d(p, points.front(), cmp)))
            points.emplace_back(p);
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
    auto visitor = [&](const point2& p) 
    {
        if (points.empty() || (!numeric_sequence_equals_2d(p, points.back(), cmp) && !numeric_sequence_equals_2d(p, points.front(), cmp)))
            points.emplace_back(p);
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
    auto visitor = [&](const point2& p) 
    {
        if (points.empty() || (!numeric_sequence_equals_2d(p, points.back(), cmp) && !numeric_sequence_equals_2d(p, points.front(), cmp)))
            points.emplace_back(p);
    };
    auto result = convex_polygon_polygon_intersection(box, trig, visitor, cmp);

    EXPECT_EQ(polygon_intersection_type::face, result);
    EXPECT_EQ(2, points.size());
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
    auto visitor = [&](const point2& p) 
    {
        if (points.empty() || (!numeric_sequence_equals_2d(p, points.back(), cmp) && !numeric_sequence_equals_2d(p, points.front(), cmp)))
            points.emplace_back(p);
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
    auto visitor = [&](const point2& p) 
    {
        if (points.empty() || (!numeric_sequence_equals_2d(p, points.back(), cmp) && !numeric_sequence_equals_2d(p, points.front(), cmp)))
            points.emplace_back(p);
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
    auto visitor = [&](const point2& p) 
    {
        if (points.empty() || (!numeric_sequence_equals_2d(p, points.back(), cmp) && !numeric_sequence_equals_2d(p, points.front(), cmp)))
            points.emplace_back(p);
    };
    auto result = convex_polygon_polygon_intersection(box, box2, visitor, cmp);

    EXPECT_EQ(polygon_intersection_type::overlapping, result);
    EXPECT_EQ(4, points.size());
}

