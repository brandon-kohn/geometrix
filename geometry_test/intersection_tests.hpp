//
//! Copyright � 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_INTERSECTION_TESTS_HPP
#define GEOMETRIX_INTERSECTION_TESTS_HPP


#include <boost/test/included/unit_test.hpp>
#include <geometrix/numeric/number_comparison_policy.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/utility/utilities.hpp>
#include <geometrix/arithmetic/vector.hpp>
#include <geometrix/algorithm/segment_intersection.hpp>
#include <geometrix/algorithm/line_intersection.hpp>
#include <geometrix/primitive/segment.hpp>
#include <geometrix/algebra/algebra.hpp>

#include <iostream>

BOOST_AUTO_TEST_CASE( TestIntersections )
{
    using namespace geometrix;

    typedef point_double_2d point_2d;
    typedef point_double_3d point_3d;
    
    point_2d p1( 0., 0. );
    point_2d p2( 1., 1. );
    point_2d p3( 1., 0. );
    point_2d p4( 0., 1. );

    segment<point_2d> seg1( p1, p2 );
    segment<point_2d> seg2( p3, p4 );

    {
        point_2d xPoints[2];
        intersection_type iType = calculate_intersection( seg1, seg2, xPoints, fraction_tolerance_comparison_policy<double>(1e-10) );
        BOOST_CHECK( iType == e_crossing );
        std::cout << iType << " at point: " << xPoints[0].get<0>() << ", " << xPoints[0].get<1>() << std::endl;
    }

    {
        point_2d xPoints[2];    
        intersection_type iType = calculate_intersection( seg2, seg1, xPoints, fraction_tolerance_comparison_policy<double>(1e-10) );
        BOOST_CHECK( iType == e_crossing );
        std::cout << iType << " at point: " << xPoints[0].get<0>() << ", " << xPoints[0].get<1>() << std::endl;
    }

    {
        point_2d p;
        segment<point_2d> seg( p1, p3 );

        intersection_type iType = line_intersect( point_2d( 0.5, -50. ), point_2d( 0.5, 50. ), seg, p, fraction_tolerance_comparison_policy<double>(1e-10) );
        BOOST_CHECK( iType == e_crossing );
        std::cout << iType << " at point: " << p.get<0>() << ", " << p.get<1>() << std::endl;

        iType = line_intersect( point_2d( 0.5, -50. ), point_2d( 0.5, -40. ), seg, p, fraction_tolerance_comparison_policy<double>(1e-10) );
        BOOST_CHECK( iType == e_crossing );
        std::cout << iType << " at point: " << p.get<0>() << ", " << p.get<1>() << std::endl;
    }
}

#endif //GEOMETRIX_INTERSECTION_TESTS_HPP



