//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GENERATIVE_GEOMETRY_UTILITY_TESTS_HPP
#define GENERATIVE_GEOMETRY_UTILITY_TESTS_HPP
#pragma once

#include <boost/test/unit_test.hpp>
#include <geometry\../geometry/number_comparison_policy.hpp>
#include <geometry\../geometry/point.hpp>
#include <geometry\../geometry/utilities.hpp>
#include <geometry\../geometry/products.hpp>

#include <iostream>

BOOST_AUTO_TEST_CASE( TestUtilityOperators )
{
	using namespace generative::numeric::geometry;

    {
	    typedef point_double_2d point_2d;
	    typedef point_double_3d point_3d;    
    	
	    point_2d p1( 0., 0. );
        point_2d p2( 1., 0. );
        point_2d p3( 2., 0. );

        //! Collinearity check.
        double dot = dot_product( p1, p2 );
        std::cout << "Dot: " << dot << std::endl;

        double cross = cross_product( p1, p2, p3 );
        std::cout << "Cross: " << cross << std::endl;

        BOOST_CHECK( is_collinear( p1, p2, p3, fraction_tolerance_comparison_policy<double>(1e-10) ) );
    }

    {
        typedef point_int_2d point_2d;
        
        point_2d p1( 0, 0 );
        point_2d p2( 1, 1 );
        point_2d p3( 2, 2 );

        //! Collinearity check.
        int dot = dot_product( p1, p2 );
        std::cout << "Dot: " << dot << std::endl;

        int cross = cross_product( p1, p2, p3 );
        std::cout << "Cross: " << cross << std::endl;

        BOOST_CHECK( is_collinear( p1, p2, p3, direct_comparison_policy() ) );   
    }

}

#endif //GENERATIVE_GEOMETRY_UTILITY_TESTS_HPP
