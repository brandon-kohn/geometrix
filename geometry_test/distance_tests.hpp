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


#include <boost/test/included/unit_test.hpp>
#include <geometrix/numeric/number_comparison_policy.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/primitive/segment.hpp>
#include <geometrix/algorithm/segment_intersection.hpp>
#include <geometrix/algorithm/euclidean_distance.hpp>
#include <geometrix/algorithm/distance/point_aabb_distance.hpp>
#include <iostream>

BOOST_AUTO_TEST_CASE( TestDistance )
{
    using namespace geometrix;
    
    absolute_tolerance_comparison_policy<double> compare(1e-10);
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
        typedef point_double_3d point_3d;    
        
        const point_3d p1( 0., 0., 0. );
        const point_3d p2( 1., 1., 1. );
        const point_3d p3( 2., 2., 2. );

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

	typedef point_double_2d point2;
	typedef segment<point2> segment2;
	{		
		segment2 s1( point2( 1.0, 0.0 ), point2( 0.0, 0.0 ) );
		segment2 s2( point2( 0.0, 1.0 ), point2( 0.0, 2.0 ) );
		BOOST_CHECK( compare.equals( segment_segment_distance_sqrd( s1, s2 ), 1.0 ) );
		BOOST_CHECK( compare.equals( eberly_segment_segment_distance_sqrd( s1, s2 ), 1.0 ) );
	}
	{
		segment2 s1( point2( -1.0264718499965966, 9.6163341007195407e-7 ), point2( 0.91950808032415809, -1.0094441192690283e-6 ) );
		segment2 s2( point2( -1.0629447383806110, 9.2709540082141753e-7 ), point2( 1.0811583868227901, -1.0670017179567367e-6 ) );
		BOOST_CHECK( segment_segment_intersection( s1, s2, (point2*)nullptr, compare ) == e_crossing );//! It should be zero as they intersect.
		BOOST_CHECK( compare.equals( segment_segment_distance_sqrd( s1, s2 ), 0.0 ) );
		//! TODO: This fails for the version which takes the sqrt of the above result due to roundoff errors.
	}
}

BOOST_AUTO_TEST_CASE( TestPointAABBDistance )
{
	using namespace geometrix;

	typedef point<double, 2> point2;
	absolute_tolerance_comparison_policy<double> cmp( 1e-10 );
	axis_aligned_bounding_box<point2> box( point2( 0., 0. ), point2( 10., 10. ) );

	{
		point2 p( 5, 5 );
		BOOST_CHECK( cmp.equals( point_aabb_distance( p, box ), 0.0 ) );
	}

	{
		point2 p( 15, 5 );
		BOOST_CHECK( cmp.equals( point_aabb_distance( p, box ), 5.0 ) );
	}

	{
		point2 p( 11, 11 );
		BOOST_CHECK( cmp.equals( point_aabb_distance( p, box ), sqrt(2.0) ) );
	}

	{
		point2 p( -1, -1 );
		BOOST_CHECK( cmp.equals( point_aabb_distance( p, box ), sqrt( 2.0 ) ) );
	}
}

#endif //GEOMETRIX_DISTANCE_TESTS_HPP
