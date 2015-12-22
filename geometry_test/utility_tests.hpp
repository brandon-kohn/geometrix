//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_UTILITY_TESTS_HPP
#define GEOMETRIX_UTILITY_TESTS_HPP

#include <boost/test/included/unit_test.hpp>
#include <geometrix/numeric/number_comparison_policy.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/utility/utilities.hpp>
#include <geometrix/arithmetic/vector.hpp>
#include <geometrix/algebra/algebra.hpp>

#include <iostream>

BOOST_AUTO_TEST_CASE( TestUtilityOperators )
{
    using namespace geometrix;

    {
        typedef point_double_2d point_2d;
        
        point_2d p1( 0., 0. );
        point_2d p2( 1., 0. );
        point_2d p3( 2., 0. );

        //! Collinearity check.
        double dot = dot_product( as_vector(p2), as_vector(p3) );
        std::cout << "Dot: " << dot << std::endl;

        double cross = exterior_product_area( p2-p1, p3-p1 );
        std::cout << "Cross: " << cross << std::endl;

        BOOST_CHECK( is_collinear( p1, p2, p3, fraction_tolerance_comparison_policy<double>(1e-10) ) );
    }

    {
        typedef point_int_2d point_2d;
        
        point_2d p1( 0, 0 );
        point_2d p2( 1, 1 );
        point_2d p3( 2, 2 );

        //! Collinearity check.
        double dot = dot_product( as_vector(p2), as_vector(p3) );
        std::cout << "Dot: " << dot << std::endl;

        double cross = exterior_product_area( p2-p1, p3-p1 );
        std::cout << "Cross: " << cross << std::endl;

        BOOST_CHECK( is_collinear( p1, p2, p3, direct_comparison_policy() ) );   
    }

}

#endif //GEOMETRIX_UTILITY_TESTS_HPP
