//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#include <geometrix/numeric/number_comparison_policy.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/utility/utilities.hpp>
#include <geometrix/primitive/vector_point_sequence.hpp>
#include <geometrix/primitive/polyline.hpp>
#include <geometrix/primitive/polygon.hpp>
#include <geometrix/primitive/point_sequence_utilities.hpp>
#include <geometrix/arithmetic/vector.hpp>
#include <geometrix/primitive/segment.hpp>
#include <geometrix/algorithm/bsp_tree_2d.hpp>

#include "./2d_kernel_fixture.hpp"

#include <iostream>

struct bsptree2d_fixture : geometry_kernel_2d_fixture
{
    typedef geometrix::bsp_tree_2d< segment2 > bsp2;
};

TEST_F(bsptree2d_fixture, TestBooleanBSP)
{
    using namespace geometrix;

    std::vector<segment2> edges;

    point2 p1( 0., 0. );
    point2 p2( 10., 0. );
    edges.push_back( segment2( p1, p2 ) );
    point2 p3( 10., 10. );
    edges.push_back( segment2( p2, p3 ) );
    point2 p4( 0., 10. );
    edges.push_back( segment2( p3, p4 ) );
    edges.push_back( segment2( p4, p1 ) );

    std::vector< point2 > box1;
    box1.push_back( p1 );
    box1.push_back( p2 );
    box1.push_back( p3 );
    box1.push_back( p4 );

    point2 p5( 50., 50. );
    point2 p6( 150., 50. );
    edges.push_back( segment2( p5, p6 ) );
    point2 p7( 150., 150. );
    edges.push_back( segment2( p6, p7 ) );
    point2 p8( 50., 150. );
    edges.push_back( segment2( p7, p8 ) );
    edges.push_back( segment2( p8, p5 ) );

    std::vector< point2 > box2;
    box2.push_back( p5 );
    box2.push_back( p6 );
    box2.push_back( p7 );
    box2.push_back( p8 );

    absolute_tolerance_comparison_policy<double> compare(1e-10);
    typedef bsp_tree_2d< segment2 > bsp_tree;
    bsp_tree tree(polygon_as_segment_range< segment2 >(box1), partition_policies::first_segment_selector_policy< segment2 >(), compare);
    tree.insert(polygon_as_segment_range< segment2 >(box2), partition_policies::first_segment_selector_policy< segment2 >(), compare);

    auto b1c = get_centroid(box1);
    EXPECT_TRUE( tree.locate_point( b1c, compare ) == e_inside );

    auto b2c = get_centroid(box2);
    EXPECT_TRUE( tree.locate_point( b2c, compare ) == e_inside );
    EXPECT_TRUE( tree.locate_point( point2( 20., 20. ), compare )  == e_outside );
    EXPECT_TRUE( tree.locate_point( p1, compare )  == e_boundary );
    EXPECT_TRUE( tree.locate_point( p6, compare )  == e_boundary );
}

TEST_F(bsptree2d_fixture, ClassifyPolyline)
{
    using namespace geometrix;

    {
        polyline2 geometry{ point2{0,0}, point2{1,0}, point2{1,1}, point2{0,1} };
        point2 pos{ 0.5,0.5 };
        bsp2 tree(polyline_as_segment_range<segment2>(geometry), partition_policies::first_segment_selector_policy<segment2>(), cmp);
        auto result = tree.locate_point(pos, cmp);
        EXPECT_EQ(e_inside, result);
        point2 pos2{ 1.5, 0.5 };
        auto result2 = tree.locate_point(pos2, cmp);
        EXPECT_EQ(e_outside, result2);
    }

    {
        polyline2 geometry{ point2{ 0,0 }, point2{ 1,0 }, point2{ 1,1 }, point2{ 0,1 } };
        point2 pos{ -1, 0 };
        bsp2 tree(polyline_as_segment_range<segment2>(geometry), partition_policies::first_segment_selector_policy<segment2>(), cmp);
        auto result = tree.locate_point(pos, cmp);
        EXPECT_NE(e_outside, result);
    }

    {
        polyline2 geometry{ point2{0, -5}, point2{ 0,0 }, point2{ 1,0 }, point2{ 1,1 }, point2{ 0,1 }, point2{ -5, -5 } };
        bsp2 tree(polyline_as_segment_range<segment2>(geometry), partition_policies::first_segment_selector_policy<segment2>(), cmp);

        point2 pos3{ -0.5, -0.1 };
        auto result3 = tree.locate_point(pos3, cmp);
        EXPECT_EQ(e_inside, result3);
    }
}
