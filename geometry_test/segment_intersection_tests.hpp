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

#include <iostream>

struct IntersectionVisitor
{
    template <typename Point, typename SegmentIterator>
    void operator()( const Point& event, SegmentIterator first, SegmentIterator last )
    {
        using namespace geometrix;
        std::cout << "Intersection: " << get<0>(event) << "," << get<1>(event) << std::endl;
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
        intersection_type iType = calculate_intersection( seg5, seg6, xPoints, fraction_tolerance_comparison_policy<double>(1e-10) );
        std::cout << iType << " at point: " << get<0>(xPoints[0]) << ", " << get<1>(xPoints[0]) << std::endl;
    }

    IntersectionVisitor v;
    bentley_ottmann_segment_intersection( segments, v, fraction_tolerance_comparison_policy<double>(1e-10) );
    
}

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
    intersection_type s = calculate_intersection( segment_t(p1, p2), segment_t(p3, p4), iPoint, fraction_tolerance_comparison_policy<double>(1e-10) );
    BOOST_CHECK( s == e_non_crossing );

    p1 = construct<point_double_3d>( 0, 0, 0 );
    v1 = construct<vector_double_3d>( 1, 1, 1 );
    p2 <<= p1 + v1;

    p3 = construct<point_double_3d>( 0.5, 0.5, 0.5 );
    p4 <<= p3 + v1;
    s = calculate_intersection( segment_t(p1, p2), segment_t(p3, p4), iPoint, fraction_tolerance_comparison_policy<double>(1e-10) );
    BOOST_CHECK( s == e_overlapping );

    p1 = construct<point_double_3d>( 0, 0, 0 );
    set<1>( p1, 1.0 );
    v1 = construct<vector_double_3d>( 1, 1, 1 );
    p2 <<= p1 + v1;

    p3 = construct<point_double_3d>( 0.5, 0.5, 0.5 );
    p4 <<= p3 + v1;
    s = calculate_intersection( segment_t(p1, p2), segment_t(p3, p4), iPoint, fraction_tolerance_comparison_policy<double>(1e-10) );
    BOOST_CHECK( s == e_non_crossing );
}

#endif //GEOMETRIX_SEGMENT_INTERSECTION_TESTS_HPP


