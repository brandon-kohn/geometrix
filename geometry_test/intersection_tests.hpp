//
//! Copyright © 2008
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_INTERSECTION_TESTS_HPP
#define _BOOST_GEOMETRY_INTERSECTION_TESTS_HPP
#pragma once

#include <boost/test/unit_test.hpp>
#include "../geometry/number_comparison_policy.hpp"
#include "../geometry/point.hpp"
#include "../geometry/utilities.hpp"
#include "../geometry/products.hpp"
#include "../geometry/segment_intersection.hpp"
#include "../geometry/line_intersection.hpp"
#include "../geometry/segment.hpp"

#include <iostream>

BOOST_AUTO_TEST_CASE( TestIntersections )
{
	using namespace boost::numeric::geometry;

	typedef point_double_2d CPoint2D;
	typedef point_double_3d CPoint3D;
	
	CPoint2D p1( 0., 0. );
    CPoint2D p2( 1., 1. );
    CPoint2D p3( 1., 0. );
    CPoint2D p4( 0., 1. );

    segment<CPoint2D> seg1( p1, p2 );
    segment<CPoint2D> seg2( p3, p4 );

    {
        CPoint2D xPoints[2];
        intersection_type iType = intersect( seg1, seg2, xPoints, fraction_tolerance_comparison_policy<double>(1e-10) );
        BOOST_CHECK( iType == e_crossing );
        std::cout << iType << " at point: " << xPoints[0].get<0>() << ", " << xPoints[0].get<1>() << std::endl;
    }

    {
        CPoint2D xPoints[2];    
        intersection_type iType = intersect( seg2, seg1, xPoints, fraction_tolerance_comparison_policy<double>(1e-10) );
        BOOST_CHECK( iType == e_crossing );
        std::cout << iType << " at point: " << xPoints[0].get<0>() << ", " << xPoints[0].get<1>() << std::endl;
    }

    {
        CPoint2D p;
        segment<CPoint2D> seg( p1, p3 );

        intersection_type iType = line_intersect( CPoint2D( 0.5, -50. ), CPoint2D( 0.5, 50. ), seg, p, fraction_tolerance_comparison_policy<double>(1e-10) );
        BOOST_CHECK( iType == e_crossing );
        std::cout << iType << " at point: " << p.get<0>() << ", " << p.get<1>() << std::endl;

        iType = line_intersect( CPoint2D( 0.5, -50. ), CPoint2D( 0.5, -40. ), seg, p, fraction_tolerance_comparison_policy<double>(1e-10) );
        BOOST_CHECK( iType == e_crossing );
        std::cout << iType << " at point: " << p.get<0>() << ", " << p.get<1>() << std::endl;
    }
}

#endif //_BOOST_GEOMETRY_INTERSECTION_TESTS_HPP



