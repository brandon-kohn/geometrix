//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_TRAPEZOIDAL_DECOMPOSITION_TEST_HPP
#define GEOMETRIX_TRAPEZOIDAL_DECOMPOSITION_TEST_HPP

#include <boost/test/unit_test.hpp>

#include <geometrix/numeric/number_comparison_policy.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/utility/utilities.hpp>
#include <geometrix/arithmetic/vector_arithmetic.hpp>
#include <geometrix/algorithm/trapezoidal_decomposition.hpp>
#include <iostream>

BOOST_AUTO_TEST_CASE( TestTrapezoidalDecomposition )
{
    using namespace geometrix;
    
    point_double_2D p1( 0., 0. );
    point_double_2D p2( 1., 1. );
    point_double_2D p3( 1., 0. );
    point_double_2D p4( 0., 1. );

    typedef std::vector< point_double_2D > Polygon;
    Polygon points;
    points.push_back( p1 );
    points.push_back( p2 );
    points.push_back( p3 );
    points.push_back( p4 );

    trapezoidal_decomposition_polygon( points, fraction_tolerance_comparison_policy<double>(1e-10) );
}

#endif //GEOMETRIX_TRAPEZOIDAL_DECOMPOSITION_TEST_HPP
