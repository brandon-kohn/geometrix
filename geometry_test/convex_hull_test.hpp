//
//! Copyright © 2008
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _GENERATIVE_GEOMETRY_CONVEX_HULL_TEST_HPP
#define _GENERATIVE_GEOMETRY_CONVEX_HULL_TEST_HPP
#pragma once

#include <boost/test/unit_test.hpp>
#include "../geometry/number_comparison_policy.hpp"
#include "../geometry/point.hpp"
#include "../geometry/utilities.hpp"
#include "../geometry/products.hpp"
#include "../geometry/convex_hull_divide_and_conquer.hpp"
#include "../geometry/convex_hull_graham.hpp"

#include <iostream>

BOOST_AUTO_TEST_CASE( TestConvexHull )
{
	using namespace generative::numeric::geometry;
	
    point_double_2d p1( 0., 0. );
    point_double_2d p2( 1., 1. );
    point_double_2d p3( 1., 0. );
    point_double_2d p4( 0., 1. );

    typedef std::vector< point_double_2d > Polygon;
    Polygon points;
    points.push_back( p1 );
    points.push_back( p2 );
    points.push_back( p3 );
    points.push_back( p4 );
    boost::shared_ptr< Polygon > pHull = graham_scan::get_convex_hull< Polygon >( points, fraction_tolerance_comparison_policy<double>( 1e-10 ) );
}

#endif //_GENERATIVE_GEOMETRY_CONVEX_HULL_TEST_HPP



