//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_POINT_SEQUENCE_TESTS_HPP
#define GEOMETRIX_POINT_SEQUENCE_TESTS_HPP


#include <boost/test/included/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/utility/utilities.hpp>
#include <geometrix/primitive/point_sequence.hpp>
#include <geometrix/primitive/point_sequence_utilities.hpp>
#include <geometrix/algorithm/point_in_polygon.hpp>
#include <geometrix/algorithm/point_sequence/is_polygon_simple.hpp>
#include <geometrix/algorithm/point_sequence/remove_collinear_points.hpp>

BOOST_AUTO_TEST_CASE( TestPointSequences )
{
    using namespace geometrix;

    typedef point_double_2d point_2d;
    
    std::vector< point_2d > polygon;

    //! Simple square.
    {
        polygon.push_back( point_2d( 0., 0. ) ); 
        polygon.push_back( point_2d( 10., 0. ) ); 
        polygon.push_back( point_2d( 10., 10. ) ); 
        polygon.push_back( point_2d( 0., 10. ) );

        double area = get_area( polygon );
        BOOST_CHECK_CLOSE( area, 100.0, 1e-10 );

        point_2d centroid = get_centroid( polygon );
        BOOST_CHECK( numeric_sequence_equals( centroid, point_2d( 5., 5. ), fraction_tolerance_comparison_policy<double>(1e-10) ) );

        BOOST_CHECK( point_in_polygon( centroid, polygon ) );
        BOOST_CHECK( point_in_convex_polygon( centroid, polygon, fraction_tolerance_comparison_policy<double>(1e-10) ) );
        BOOST_CHECK( point_in_convex_quadrilateral( centroid, polygon, fraction_tolerance_comparison_policy<double>(1e-10) ) );

        BOOST_CHECK( !point_in_polygon( point_2d( -5, -5 ), polygon ) );
        BOOST_CHECK( !point_in_convex_polygon( point_2d( -5, -5 ), polygon, fraction_tolerance_comparison_policy<double>(1e-10) ) );
        BOOST_CHECK( !point_in_convex_quadrilateral( point_2d( -5, -5 ), polygon, fraction_tolerance_comparison_policy<double>(1e-10) ) );

        BOOST_CHECK( point_in_polygon( point_2d( 5, 6 ), polygon ) );
        BOOST_CHECK( point_in_convex_polygon( point_2d( 5, 6 ), polygon, fraction_tolerance_comparison_policy<double>(1e-10) ) );
        BOOST_CHECK( point_in_convex_quadrilateral( point_2d( 5, 6 ), polygon, fraction_tolerance_comparison_policy<double>(1e-10) ) );

        BOOST_CHECK( point_in_polygon( point_2d( 5, 4 ), polygon ) );
        BOOST_CHECK( point_in_convex_polygon( point_2d( 5, 4 ), polygon, fraction_tolerance_comparison_policy<double>(1e-10) ) );
        BOOST_CHECK( point_in_convex_quadrilateral( point_2d( 5, 4 ), polygon, fraction_tolerance_comparison_policy<double>(1e-10) ) );

        BOOST_CHECK( !point_in_polygon( point_2d( 5, 11 ), polygon ) );
        BOOST_CHECK( !point_in_convex_polygon( point_2d( 5, 11 ), polygon, fraction_tolerance_comparison_policy<double>(1e-10) ) );
        BOOST_CHECK( !point_in_convex_quadrilateral( point_2d( 5, 11 ), polygon, fraction_tolerance_comparison_policy<double>(1e-10) ) );

		for( std::size_t i = 0; i < polygon.size(); ++i )
		{
			BOOST_CHECK( !is_point_concave( polygon, i ) );
		}
    }

    //! hexagon.
    {
        polygon.clear();
        polygon.push_back( point_2d( 0., 0. ) ); 
        polygon.push_back( point_2d( 10., 0. ) ); 
        polygon.push_back( point_2d( 15., 5. ) ); 
        polygon.push_back( point_2d( 10., 10. ) ); 
        polygon.push_back( point_2d( 0., 10. ) ); 
        polygon.push_back( point_2d( -5., 5. ) );
        
        point_2d centroid = get_centroid( polygon );
        BOOST_CHECK( numeric_sequence_equals( centroid, point_2d( 5., 5. ), fraction_tolerance_comparison_policy<double>(1e-10) ) );

        BOOST_CHECK( point_in_polygon( centroid, polygon ) );
        BOOST_CHECK( point_in_convex_polygon( centroid, polygon, fraction_tolerance_comparison_policy<double>(1e-10) ) );
        
        BOOST_CHECK( !point_in_polygon( point_2d( -5, -5 ), polygon ) );
        BOOST_CHECK( !point_in_convex_polygon( point_2d( -5, -5 ), polygon, fraction_tolerance_comparison_policy<double>(1e-10) ) );
        
        BOOST_CHECK( point_in_polygon( point_2d( 5, 6 ), polygon ) );
        BOOST_CHECK( point_in_convex_polygon( point_2d( 5, 6 ), polygon, fraction_tolerance_comparison_policy<double>(1e-10) ) );
        
        BOOST_CHECK( point_in_polygon( point_2d( 5, 4 ), polygon ) );
        BOOST_CHECK( point_in_convex_polygon( point_2d( 5, 4 ), polygon, fraction_tolerance_comparison_policy<double>(1e-10) ) );
        
        BOOST_CHECK( !point_in_polygon( point_2d( 5, 16 ), polygon ) );
        BOOST_CHECK( !point_in_convex_polygon( point_2d( 5, 16 ), polygon, fraction_tolerance_comparison_policy<double>(1e-10) ) );

		for( std::size_t i = 0; i < polygon.size(); ++i )
		{
			BOOST_CHECK( !is_point_concave( polygon, i ) );
		}
    }

	{
		polygon.clear();
		polygon.push_back( point_2d( 0., 0. ) );
		polygon.push_back( point_2d( 10., 0. ) );
		polygon.push_back( point_2d( 15., 5. ) );
		polygon.push_back( point_2d( 10., 10. ) );
		polygon.push_back( point_2d( 0., 10. ) );
		polygon.push_back( point_2d( 5., 5. ) );
		BOOST_CHECK( !is_point_concave( polygon, 0 ) );
		BOOST_CHECK( !is_point_concave( polygon, 1 ) );
		BOOST_CHECK( !is_point_concave( polygon, 2 ) );
		BOOST_CHECK( !is_point_concave( polygon, 3 ) );
		BOOST_CHECK( !is_point_concave( polygon, 4 ) );
		BOOST_CHECK( is_point_concave( polygon, 5 ) );
	}
}

BOOST_AUTO_TEST_CASE( TestIsSimplePolygon )
{
	typedef point_double_2d point2;
	typedef std::vector<point2> polygon2;

	polygon2 geometry{point2{-489.07583699998213, -38.650772999506444}, point2{-489.01883699995233, -38.593772999476641}, point2{-489.0558369999635, -38.48077299958095}, point2{-489.08883699995931, -38.522772999480367}, point2{-489.07583699998213, -38.650772999506444}, point2{-489.65283699997235, -39.224772999528795}, point2{-489.73983699997189, -39.328772999346256}, point2{-490.27783699997235, -39.968772999476641}, point2{-490.27783699997235, -39.969772999640554}, point2{-490.3748369999812, -40.082772999536246}, point2{-490.50083699997049, -40.234772999770939}, point2{-490.50183699995978, -40.235772999469191}, point2{-490.85483699996257, -40.651772999670357}, point2{-490.86083699995652, -40.657772999722511}, point2{-491.09383699996397, -40.85577299958095}, point2{-491.09583699994255, -40.856772999744862}, point2{-491.30983699997887, -41.021772999782115}, point2{-491.45483699993929, -41.194772999733686}, point2{-491.61483699997189, -41.382772999349982}, point2{-491.62183699995512, -41.389772999566048}, point2{-491.90383699996164, -41.615772999357432}, point2{-489.71683699998539, -57.292772999498993}, point2{-488.58583699999144, -57.421772999688983}, point2{-487.47583699994721, -57.547772999387234}, point2{-487.47083699994255, -57.548772999551147}, point2{-487.2158369999961, -57.602772999554873}, point2{-487.1698369999649, -57.667772999498993}, point2{-487.17183699994348, -57.684772999491543}, point2{-487.13683699996909, -57.688772999681532}};
	auto cmp = absolute_tolerance_comparison_policy<double>(1e-10);
	BOOST_CHECK( !is_polygon_simple( geometry, cmp ) );

	geometry.clear();
	geometry.push_back( point2( 0., 0. ) );
	geometry.push_back( point2( 10., 0. ) );
	geometry.push_back( point2( 15., 5. ) );
	geometry.push_back( point2( 10., 10. ) );
	geometry.push_back( point2( 0., 10. ) );
	geometry.push_back( point2( 5., 5. ) );
	BOOST_CHECK( is_polygon_simple( geometry, cmp ) );
}

#endif //GEOMETRIX_POINT_SEQUENCE_TESTS_HPP
