//
//! Copyright © 2008
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_SEGMENT_INTERVAL_TESTS_HPP
#define _BOOST_GEOMETRY_SEGMENT_INTERVAL_TESTS_HPP
#pragma once

#include <boost/test/unit_test.hpp>
#include "../geometry/number_comparison_policy.hpp"
#include "../geometry/point.hpp"
#include "../geometry/utilities.hpp"
#include "../geometry/products.hpp"
#include "../geometry/segment_intersection.hpp"
#include "../geometry/segment.hpp"
#include <iostream>

using namespace boost::numeric::geometry;

template <typename SegmentIntervalSet>
void print_segments( const SegmentIntervalSet& segments )
{
    typedef segment_access_traits< SegmentIntervalSet::value_type > segment_access;
    typedef segment_access::point_type point_type;
    typedef cartesian_access_traits< point_type > point_access;
    std::cout << "-----------------------" << std::endl;
    BOOST_FOREACH( const segment_access::segment_type& seg, segments )
    {
        const point_type& C = segment_access::get_start( seg );
        const point_type& D = segment_access::get_end( seg );
        std::cout << "( " << point_access::get<0>( C ) << "," << point_access::get<1>( C ) << " )--";
        std::cout << "( " << point_access::get<0>( D ) << "," << point_access::get<1>( D ) << " )" << std::endl;
    }
}

BOOST_AUTO_TEST_CASE( TestSegmentInterval )
{
	typedef point_double_2d CPoint2D;
	
	CPoint2D p1( 0., 0. );
    CPoint2D p2( 10., 0. );
    CPoint2D p3( 1., 0. );
    CPoint2D p4( 3., 0. );
    CPoint2D p5( 2., 0. );
    CPoint2D p6( 4., 0. ); 
    CPoint2D p7( 2., 0. );
    CPoint2D p8( 9., 0. ); 
    
    segment<CPoint2D> seg1( p1, p2 );
    segment<CPoint2D> seg2( p3, p4 );
    segment<CPoint2D> seg3( p5, p6 );
    segment<CPoint2D> seg4( p7, p8 );
    segment<CPoint2D> seg5( p6, p5 );

    std::set< segment<CPoint2D>, segment_interval_compare< fraction_tolerance_comparison_policy<double> > > segments;
    segments.insert( seg1 );

    collinear_segment_difference( segments, seg2, fraction_tolerance_comparison_policy<double>( 1e-10 ) );
    print_segments( segments );   

    collinear_segment_difference( segments, seg2, fraction_tolerance_comparison_policy<double>( 1e-10 ) );
    print_segments( segments );   

    collinear_segment_difference( segments, seg5, fraction_tolerance_comparison_policy<double>( 1e-10 ) );
    print_segments( segments );   

    collinear_segment_difference( segments, seg3, fraction_tolerance_comparison_policy<double>( 1e-10 ) );
    print_segments( segments );   

    collinear_segment_difference( segments, seg4, fraction_tolerance_comparison_policy<double>( 1e-10 ) );
    print_segments( segments );   

}

#endif //_BOOST_GEOMETRY_SEGMENT_INTERVAL_TESTS_HPP


