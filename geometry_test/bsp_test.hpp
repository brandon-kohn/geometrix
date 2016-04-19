//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_BSP_TEST_HPP
#define GEOMETRIX_BSP_TEST_HPP

#include <boost/test/included/unit_test.hpp>

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

template <typename Polygon>
void print_polygon( const Polygon& polygon )
{
    using namespace geometrix;
    typedef typename point_sequence_traits< Polygon >::point_type point_type;
    typedef cartesian_access_traits< point_type > point_access;

    std::cout << "-----------------------" << std::endl;
    BOOST_FOREACH( const point_type& p, polygon )
    {
        std::cout << "( " << point_access::get<0>( p ) << "," << point_access::get<1>( p ) << " )" << std::endl;
    }
}

BOOST_AUTO_TEST_CASE( TestBooleanBSP )
{
    using namespace geometrix;

    typedef point_double_2d point_2d;
    typedef segment< point_2d > segment_2d;
    std::vector< segment_2d > edges;

    point_2d p1( 0., 0. );
    point_2d p2( 10., 0. );
    edges.push_back( segment_2d( p1, p2 ) );
    point_2d p3( 10., 10. );
    edges.push_back( segment_2d( p2, p3 ) );
    point_2d p4( 0., 10. );
    edges.push_back( segment_2d( p3, p4 ) );
    edges.push_back( segment_2d( p4, p1 ) );

    std::vector< point_2d > box1;
    box1.push_back( p1 );
    box1.push_back( p2 );
    box1.push_back( p3 );
    box1.push_back( p4 );

    point_2d p5( 50., 50. );
    point_2d p6( 150., 50. ); 
    edges.push_back( segment_2d( p5, p6 ) );
    point_2d p7( 150., 150. );
    edges.push_back( segment_2d( p6, p7 ) );
    point_2d p8( 50., 150. );
    edges.push_back( segment_2d( p7, p8 ) );
    edges.push_back( segment_2d( p8, p5 ) );

    std::vector< point_2d > box2;
    box2.push_back( p5 );
    box2.push_back( p6 );
    box2.push_back( p7 );
    box2.push_back( p8 );

    absolute_tolerance_comparison_policy<double> compare(1e-10); 
    typedef bsp_tree_2d< segment_2d > bsp_tree;
	bsp_tree tree(polygon_as_segment_range< segment_2d >(box1), partition_policies::first_segment_selector_policy< segment_2d >(), compare);
	tree.insert(polygon_as_segment_range< segment_2d >(box2), partition_policies::first_segment_selector_policy< segment_2d >(), compare);

	auto b1c = get_centroid(box1);
    BOOST_CHECK( tree.locate_point( b1c, compare ) == e_inside );

	auto b2c = get_centroid(box2);
    BOOST_CHECK( tree.locate_point( b2c, compare ) == e_inside );
    BOOST_CHECK( tree.locate_point( point_2d( 20., 20. ), compare )  == e_outside );
    BOOST_CHECK( tree.locate_point( p1, compare )  == e_boundary );
    BOOST_CHECK( tree.locate_point( p6, compare )  == e_boundary );
}

BOOST_FIXTURE_TEST_CASE(ClassifyPolyline, bsptree2d_fixture)
{
	using namespace geometrix;
	
	{
		polyline2 geometry{ point2{0,0}, point2{1,0}, point2{1,1}, point2{0,1} };
		point2 pos{ 0.5,0.5 };
		bsp2 tree(polyline_as_segment_range<segment2>(geometry), partition_policies::first_segment_selector_policy<segment2>(), cmp);
		auto result = tree.locate_point(pos, cmp);
		BOOST_CHECK_EQUAL(e_inside, result);
		point2 pos2{ 1.5, 0.5 };
		auto result2 = tree.locate_point(pos2, cmp);
		BOOST_CHECK_EQUAL(e_outside, result2);
	}

	{
		polyline2 geometry{ point2{ 0,0 }, point2{ 1,0 }, point2{ 1,1 }, point2{ 0,1 } };
		point2 pos{ -1, 0 };
		bsp2 tree(polyline_as_segment_range<segment2>(geometry), partition_policies::first_segment_selector_policy<segment2>(), cmp);
		auto result = tree.locate_point(pos, cmp);
		BOOST_CHECK_NE(e_outside, result);		
	}

	{
		auto geometry = polyline2{ point2{ 0,0 }, point2{ 1,0 }, point2{ 1,1 }, point2{ 0,1 }, point2{-5, -5} };
		point2 pos{ -1, 0 };
		bsp2 tree(polyline_as_segment_range<segment2>(geometry), partition_policies::first_segment_selector_policy<segment2>(), cmp);
		auto result = tree.locate_point(pos, cmp);
		BOOST_CHECK_EQUAL(e_outside, result);

		point2 pos2{ -0.5, 0 };
		auto result2 = tree.locate_point(pos2, cmp);
		BOOST_CHECK_NE(e_outside, result2);

		point2 pos3{ -0.5, -0.1 };
		auto result3 = tree.locate_point(pos3, cmp);
		BOOST_CHECK_EQUAL(e_outside, result3);
	}

	{
		polyline2 geometry{ point2{0, -5}, point2{ 0,0 }, point2{ 1,0 }, point2{ 1,1 }, point2{ 0,1 }, point2{ -5, -5 } };
		bsp2 tree(polyline_as_segment_range<segment2>(geometry), partition_policies::first_segment_selector_policy<segment2>(), cmp);

		point2 pos3{ -0.5, -0.1 };
		auto result3 = tree.locate_point(pos3, cmp);
		BOOST_CHECK_EQUAL(e_inside, result3);
	}
}

BOOST_FIXTURE_TEST_CASE(bsp_tree_classify_polyline, bsptree2d_fixture)
{
	using namespace geometrix;
	//! polyline with a shape like an __/\/\__
	polyline2 geometry{ point2{ 0, 0 }, point2{ 1,0 }, point2{ 1,1 }, point2{ 2, 0 }, point2{ 3, 1 }, point2{ 4, 0 }, point2{ 5, 0} };
	bsp2 tree(polyline_as_segment_range<segment2>(geometry), partition_policies::first_segment_selector_policy<segment2>(), cmp);

	point2 pos{ 1.75, 0.5 };
	auto result = tree.locate_point(pos, cmp);
	BOOST_CHECK_EQUAL(e_boundary, result);
}

#endif //GEOMETRIX_BSP_TEST_HPP


