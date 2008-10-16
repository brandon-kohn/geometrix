//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_SEGMENT_INTERSECTION_TESTS_HPP
#define _BOOST_GEOMETRY_SEGMENT_INTERSECTION_TESTS_HPP
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
	using namespace boost::numeric::geometry;

	typedef point_double_2d CPoint2D;
	typedef point_double_3d CPoint3D;
	
	CPoint2D p1( 0., 0. );
    CPoint2D p2( 1., 1. );
    CPoint2D p3( 1., 0. );
    CPoint2D p4( 0., 1. );
    CPoint2D p5( 1., 2. );
    CPoint2D p6( 0.5, -0.1 ); 
    CPoint2D p7( 1.1, 0.8 ); 

    segment<CPoint2D> seg1( p1, p2 );
    segment<CPoint2D> seg2( p3, p4 );
    segment<CPoint2D> seg3( p3, p2 );
    segment<CPoint2D> seg4( p2, p4 );
    segment<CPoint2D> seg5( p6, p7 );
    segment<CPoint2D> seg6( p1, p3 );

    std::vector< segment<CPoint2D> > segments;
    segments.push_back( seg1 );
    segments.push_back( seg2 );
    segments.push_back( seg3 );
    segments.push_back( seg4 );
    segments.push_back( seg5 );
    segments.push_back( seg6 );

    {
        CPoint2D xPoints[2];    
        intersection_type iType = intersect( seg5, seg6, xPoints, fraction_tolerance_comparison_policy<double>(1e-10) );
        std::cout << iType << " at point: " << xPoints[0].get<0>() << ", " << xPoints[0].get<1>() << std::endl;
    }

    bentley_ottmann_segment_intersection( segments, IntersectionVisitor(), fraction_tolerance_comparison_policy<double>(1e-10) );
    
}

#endif //_BOOST_GEOMETRY_SEGMENT_INTERSECTION_TESTS_HPP


