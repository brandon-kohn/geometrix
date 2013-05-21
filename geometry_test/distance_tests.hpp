//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_DISTANCE_TESTS_HPP
#define GEOMETRIX_DISTANCE_TESTS_HPP


#include <boost/test/unit_test.hpp>
#include <geometrix/numeric/number_comparison_policy.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/algorithm/euclidean_distance.hpp>
#include <iostream>

BOOST_AUTO_TEST_CASE( TestDistance )
{
    using namespace geometrix;
    
    fraction_tolerance_comparison_policy<double> compare(1e-10);
    {	    
        typedef point_double_3d point_3d;    
        
        point_3d p1( 0., 0., 0. );
        point_3d p2( 1., 1., 1. );
        point_3d p3( 2., 2., 2. );

        double d1 = point_point_distance( p1, p2 );
        double d2 = point_point_distance( p1, p3 );
        double d3 = point_point_distance( p2, p3 );
        BOOST_CHECK( compare.equals( d1, std::sqrt( 3.0 ) ) );
        BOOST_CHECK( compare.equals( d2, std::sqrt( 12.0 ) ) );
        BOOST_CHECK( compare.equals( d3, std::sqrt( 3.0 ) ) );
    }

    {
        typedef point_int_3d point_3d;
        
        point_3d p1( 0, 0, 0 );
        point_3d p2( 1, 1, 1 );
        point_3d p3( 2, 2, 2 );

        double d1 = point_point_distance( p1, p2 );
        double d2 = point_point_distance( p1, p3 );
        double d3 = point_point_distance( p2, p3 );
        BOOST_CHECK( compare.equals( d1, std::sqrt( 3.0 ) ) );
        BOOST_CHECK( compare.equals( d2, std::sqrt( 12.0 ) ) );
        BOOST_CHECK( compare.equals( d3, std::sqrt( 3.0 ) ) );
    }

}

#endif //GEOMETRIX_DISTANCE_TESTS_HPP
