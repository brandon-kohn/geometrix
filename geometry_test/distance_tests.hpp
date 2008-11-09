//
//! Copyright © 2008
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_DISTANCE_TESTS_HPP
#define _BOOST_GEOMETRY_DISTANCE_TESTS_HPP
#pragma once

#include <boost/test/unit_test.hpp>
#include "../geometry/number_comparison_policy.hpp"
#include "../geometry/point.hpp"
#include "../geometry/euclidian_distance.hpp"
#include <iostream>

BOOST_AUTO_TEST_CASE( TestDistance )
{
	using namespace boost::numeric::geometry;

    {	    
	    typedef point_double_3d CPoint3D;    
    	fraction_tolerance_comparison_policy<double> compare(1e-10);

	    CPoint3D p1( 0., 0., 0. );
        CPoint3D p2( 1., 1., 1. );
        CPoint3D p3( 2., 2., 2. );

        double d1 = euclidian_distance( p1, p2 );
        double d2 = euclidian_distance( p1, p3 );
        double d3 = euclidian_distance( p2, p3 );
        BOOST_CHECK( compare.equals( d1, std::sqrt( 3.0 ) ) );
        BOOST_CHECK( compare.equals( d2, std::sqrt( 12.0 ) ) );
        BOOST_CHECK( compare.equals( d3, std::sqrt( 3.0 ) ) );
    }

    {
        typedef point_int_3d CPoint3D;
        
        CPoint3D p1( 0, 0, 0 );
        CPoint3D p2( 1, 1, 1 );
        CPoint3D p3( 2, 2, 2 );

        BOOST_CHECK( euclidian_distance( p1, p2 ) == 1 );
        BOOST_CHECK( euclidian_distance( p1, p3 ) == 3 );
        BOOST_CHECK( euclidian_distance( p2, p3 ) == 1 );   
    }

}

#endif //_BOOST_GEOMETRY_DISTANCE_TESTS_HPP
