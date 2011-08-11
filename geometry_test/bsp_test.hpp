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


#include <boost/test/unit_test.hpp>

#include <geometrix/numeric/number_comparison_policy.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/utility/utilities.hpp>
#include <geometrix/primitive/point_sequence.hpp>
#include <geometrix/primitive/point_sequence_utilities.hpp>
#include <geometrix/arithmetic/vector_arithmetic.hpp>
#include <geometrix/primitive/segment.hpp>
#include <geometrix/algorithm/bsp_tree_2D.hpp>

#include <iostream>

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

template <typename Segment, typename Polygon>
std::vector< Segment > as_segment_range( const Polygon& p )
{
    using namespace geometrix;

    std::vector< Segment > segments;
    point_sequence_traits< Polygon >::const_iterator pNext = point_sequence_traits< Polygon >::begin( p ); 
    point_sequence_traits< Polygon >::const_iterator pIt = pNext++;
    point_sequence_traits< Polygon >::const_iterator pEnd = point_sequence_traits< Polygon >::end( p );        
    while( pNext != pEnd )
    {
        Segment segment = construction_policy< Segment >::construct( *pIt++, *pNext++ );
        segments.push_back( segment );
    }

    return segments;
}

BOOST_AUTO_TEST_CASE( TestBooleanBSP )
{
    using namespace geometrix;

    typedef point_double_2D point_2D;
    typedef segment< point_2D > segment_2D;
    std::vector< segment_2D > edges;

    point_2D p1( 0., 0. );
    point_2D p2( 10., 0. );
    edges.push_back( segment_2D( p1, p2 ) );
    point_2D p3( 10., 10. );
    edges.push_back( segment_2D( p2, p3 ) );
    point_2D p4( 0., 10. );
    edges.push_back( segment_2D( p3, p4 ) );
    edges.push_back( segment_2D( p4, p1 ) );

    std::vector< point_2D > box1;
    box1.push_back( p1 );
    box1.push_back( p2 );
    box1.push_back( p3 );
    box1.push_back( p4 );
    box1.push_back( p1 );

    point_2D p5( 50., 50. );
    point_2D p6( 150., 50. ); 
    edges.push_back( segment_2D( p5, p6 ) );
    point_2D p7( 150., 150. );
    edges.push_back( segment_2D( p6, p7 ) );
    point_2D p8( 50., 150. );
    edges.push_back( segment_2D( p7, p8 ) );
    edges.push_back( segment_2D( p8, p5 ) );

    std::vector< point_2D > box2;
    box2.push_back( p5 );
    box2.push_back( p6 );
    box2.push_back( p7 );
    box2.push_back( p8 );
    box2.push_back( p5 );

    fraction_tolerance_comparison_policy<double> compare(1e-10); 
    typedef bsp_tree_2D< segment_2D > bsp_tree;
    bsp_tree tree( as_segment_range< segment_2D >( box1 ), partition_policies::random_segment_selector_policy< segment_2D >(), compare );
    tree.insert( as_segment_range< segment_2D >( box2 ), partition_policies::random_segment_selector_policy< segment_2D >(), compare );

    BOOST_CHECK( tree.locate_point( get_centroid( box1 ), compare ) == e_inside );
    BOOST_CHECK( tree.locate_point( get_centroid( box2 ), compare ) == e_inside );
    BOOST_CHECK( tree.locate_point( point_2D( 20., 20. ), compare )  == e_outside );
    BOOST_CHECK( tree.locate_point( p1, compare )  == e_boundary );
    BOOST_CHECK( tree.locate_point( p6, compare )  == e_boundary );

}

#endif //GEOMETRIX_BSP_TEST_HPP


