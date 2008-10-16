//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_UTILITY_TESTS_HPP
#define _BOOST_GEOMETRY_UTILITY_TESTS_HPP
#pragma once

#include <boost/test/unit_test.hpp>
#include "../geometry/number_comparison_policy.hpp"
#include "../geometry/point.hpp"
#include "../geometry/utilities.hpp"
#include "../geometry/products.hpp"

#include <iostream>

BOOST_AUTO_TEST_CASE( TestUtilityOperators )
{
	using namespace boost::numeric::geometry;

    {
	    typedef point_double_2d CPoint2D;
	    typedef point_double_3d CPoint3D;    
    	
	    CPoint2D p1( 0., 0. );
        CPoint2D p2( 1., 0. );
        CPoint2D p3( 2., 0. );

        //! Collinearity check.
        double dot = dot_product( p1, p2 );
        std::cout << "Dot: " << dot << std::endl;

        double cross = cross_product( p1, p2, p3 );
        std::cout << "Cross: " << cross << std::endl;

        BOOST_CHECK( is_collinear( p1, p2, p3, fraction_tolerance_comparison_policy<double>(1e-10) ) );
    }

    {
        typedef point_int_2d CPoint2D;
        
        CPoint2D p1( 0, 0 );
        CPoint2D p2( 1, 1 );
        CPoint2D p3( 2, 2 );

        //! Collinearity check.
        int dot = dot_product( p1, p2 );
        std::cout << "Dot: " << dot << std::endl;

        int cross = cross_product( p1, p2, p3 );
        std::cout << "Cross: " << cross << std::endl;

        BOOST_CHECK( is_collinear( p1, p2, p3, direct_comparison_policy() ) );   
    }

}

#endif //_BOOST_GEOMETRY_UTILITY_TESTS_HPP



