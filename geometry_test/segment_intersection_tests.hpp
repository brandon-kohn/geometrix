//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _GENERATIVE_GEOMETRY_SEGMENT_INTERSECTION_TESTS_HPP
#define _GENERATIVE_GEOMETRY_SEGMENT_INTERSECTION_TESTS_HPP
#pragma once

#include <boost/test/unit_test.hpp>
#include "../geometry/number_comparison_policy.hpp"
#include "../geometry/point.hpp"
#include "../geometry/utilities.hpp"
#include "../geometry/products.hpp"
#include "../geometry/segment_intersection.hpp"
#include "../geometry/segment.hpp"
#include "../geometry/bentley_ottmann_segment_intersection.hpp"

#include <iostream>

struct IntersectionVisitor
{
    template <typename Point, typename SegmentIterator>
    void operator()( const Point& event, SegmentIterator first, SegmentIterator last )
    {
        std::cout << "Intersection: " << event.get<0>() << "," << event.get<1>() << std::endl;
    }
};

BOOST_AUTO_TEST_CASE( TestBentleyOttmannSegmentIntersection )
{
	using namespace generative::numeric::geometry;

	typedef point_double_2d point_2d;
	typedef point_double_3d point_3d;
	
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
        intersection_type iType = intersect( seg5, seg6, xPoints, fraction_tolerance_comparison_policy<double>(1e-10) );
        std::cout << iType << " at point: " << xPoints[0].get<0>() << ", " << xPoints[0].get<1>() << std::endl;
    }

    bentley_ottmann_segment_intersection( segments, IntersectionVisitor(), fraction_tolerance_comparison_policy<double>(1e-10) );
    
}

#endif //_GENERATIVE_GEOMETRY_SEGMENT_INTERSECTION_TESTS_HPP


