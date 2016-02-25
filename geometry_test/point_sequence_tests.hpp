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
	using namespace geometrix;
	typedef point_double_2d point2;
	typedef std::vector<point2> polygon2;
	auto cmp = absolute_tolerance_comparison_policy<double>( 1e-10 );

	{
		polygon2 geometry{point2{-489.07583699998213, -38.650772999506444}, point2{-489.01883699995233, -38.593772999476641}, point2{-489.0558369999635, -38.48077299958095}, point2{-489.08883699995931, -38.522772999480367}, point2{-489.07583699998213, -38.650772999506444}, point2{-489.65283699997235, -39.224772999528795}, point2{-489.73983699997189, -39.328772999346256}, point2{-490.27783699997235, -39.968772999476641}, point2{-490.27783699997235, -39.969772999640554}, point2{-490.3748369999812, -40.082772999536246}, point2{-490.50083699997049, -40.234772999770939}, point2{-490.50183699995978, -40.235772999469191}, point2{-490.85483699996257, -40.651772999670357}, point2{-490.86083699995652, -40.657772999722511}, point2{-491.09383699996397, -40.85577299958095}, point2{-491.09583699994255, -40.856772999744862}, point2{-491.30983699997887, -41.021772999782115}, point2{-491.45483699993929, -41.194772999733686}, point2{-491.61483699997189, -41.382772999349982}, point2{-491.62183699995512, -41.389772999566048}, point2{-491.90383699996164, -41.615772999357432}, point2{-489.71683699998539, -57.292772999498993}, point2{-488.58583699999144, -57.421772999688983}, point2{-487.47583699994721, -57.547772999387234}, point2{-487.47083699994255, -57.548772999551147}, point2{-487.2158369999961, -57.602772999554873}, point2{-487.1698369999649, -57.667772999498993}, point2{-487.17183699994348, -57.684772999491543}, point2{-487.13683699996909, -57.688772999681532}};		
		BOOST_CHECK( !is_polygon_simple( geometry, cmp ) );
	}

	{
		polygon2 geometry;
		geometry.push_back( point2( 0., 0. ) );
		geometry.push_back( point2( 10., 0. ) );
		geometry.push_back( point2( 15., 5. ) );
		geometry.push_back( point2( 10., 10. ) );
		geometry.push_back( point2( 0., 10. ) );
		geometry.push_back( point2( 5., 5. ) );
		BOOST_CHECK( is_polygon_simple( geometry, cmp ) );
	}

	{
		polygon2 geometry{point2{0.0, 0.0}, point2{1.0, 0.0}, point2{-0.5, 0.0}, point2{1.0, 1.0}, point2{0.0, 1.0}};
		BOOST_CHECK( !is_polygon_simple( geometry, cmp ) );
	}

	{
		polygon2 geometry{ point2{ 604.23649999999907, 165.78850000025705 }, point2{ 604.25150000001304, 166.7375000002794 }, point2{ 604.30850000004284, 166.79550000000745 }, point2{ 605.66750000003958, 166.80649999994785 }, point2{ 624.72350000002189, 166.87550000008196 }, point2{ 628.44949999998789, 166.88450000016019 }, point2{ 628.45150000002468, 166.88450000016019 }, point2{ 629.20650000002934, 166.84750000014901 }, point2{ 629.25250000000233, 166.7214999999851 }, point2{ 628.95350000000326, 166.4484999999404 }, point2{ 628.9484999999986, 166.44450000021607 }, point2{ 628.60450000001583, 166.2035000002943 }, point2{ 628.60350000002654, 166.20250000013039 }, point2{ 628.47750000003725, 165.87850000010803 }, point2{ 639.27050000004238, 165.92750000022352 }, point2{ 639.04250000003958, 166.13250000029802 }, point2{ 639.0395000000135, 166.13450000016019 }, point2{ 638.41550000000279, 166.30649999994785 }, point2{ 638.40049999998882, 166.3125 }, point2{ 638.01949999999488, 166.56749999988824 }, point2{ 638.04450000001816, 166.69750000024214 }, point2{ 639.2445000000298, 166.95050000026822 }, point2{ 639.25250000000233, 166.95149999996647 }, point2{ 640.74150000000373, 167.00849999999627 }, point2{ 640.7445000000298, 167.00849999999627 }, point2{ 644.17149999999674, 166.98150000022724 }, point2{ 653.36450000002515, 167.01649999991059 }, point2{ 653.41649999999208, 168.32750000013039 }, point2{ 639.21549999999115, 168.36349999997765 }, point2{ 639.21250000002328, 168.36349999997765 }, point2{ 637.08950000000186, 168.49250000016764 }, point2{ 637.03450000000885, 168.55349999992177 }, point2{ 637.1765000000014, 171.76550000021234 }, point2{ 637.24249999999302, 173.04350000014529 }, point2{ 637.24150000000373, 173.0445000003092 }, point2{ 629.80249999999069, 173.00950000016019 }, point2{ 629.9035000000149, 170.5625 }, point2{ 630.04450000001816, 168.1695000003092 }, point2{ 629.98749999998836, 168.10650000022724 }, point2{ 628.43249999999534, 168.06850000005215 }, point2{ 628.43150000000605, 168.06850000005215 }, point2{ 613.47850000002654, 168.07049999991432 }, point2{ 606.5515000000014, 168.0785000002943 }, point2{ 606.55050000001211, 168.0785000002943 }, point2{ 605.65850000001956, 168.10149999987334 }, point2{ 605.65750000003027, 168.10149999987334 }, point2{ 604.74150000000373, 168.13950000004843 }, point2{ 604.70350000000326, 168.15849999990314 }, point2{ 603.88050000002841, 169.14149999991059 }, point2{ 603.86850000004051, 169.17450000019744 }, point2{ 603.86050000000978, 170.34450000012293 }, point2{ 603.76449999999022, 191.48549999995157 }, point2{ 603.82250000000931, 191.5445000003092 }, point2{ 605.57050000003073, 191.5445000003092 }, point2{ 605.59549999999581, 192.93149999994785 }, point2{ 605.59650000004331, 192.93149999994785 }, point2{ 603.83449999999721, 192.95650000032037 }, point2{ 603.77650000003632, 193.01349999988452 }, point2{ 603.73749999998836, 196.73050000006333 }, point2{ 603.73649999999907, 196.73350000008941 }, point2{ 603.64949999999953, 199.32550000026822 }, point2{ 603.64949999999953, 199.32750000013039 }, point2{ 603.61350000003586, 204.13750000018626 }, point2{ 603.61350000003586, 204.1875 }, point2{ 603.64350000000559, 213.43949999986216 }, point2{ 603.70050000003539, 213.49750000005588 }, point2{ 616.91850000002887, 213.71950000012293 }, point2{ 616.58250000001863, 215.83250000001863 }, point2{ 608.55249999999069, 215.79850000003353 }, point2{ 603.1984999999986, 215.8015000000596 }, point2{ 603.19550000003073, 215.8015000000596 }, point2{ 601.36450000002515, 215.58850000007078 }, point2{ 601.36350000003586, 215.58749999990687 }, point2{ 601.36249999998836, 215.5625 }, point2{ 601.33250000001863, 215.51649999991059 }, point2{ 600.85250000003725, 215.27650000015274 }, point2{ 600.36650000000373, 214.79050000011921 }, point2{ 600.36650000000373, 214.7894999999553 }, point2{ 600.16250000003492, 214.20550000015646 }, point2{ 600.16250000003492, 213.56650000018999 }, point2{ 601.90450000000419, 213.53450000006706 }, point2{ 601.96150000003399, 213.47750000003725 }, point2{ 602.08150000002934, 204.17450000019744 }, point2{ 602.08150000002934, 204.17350000003353 }, point2{ 602.04850000003353, 199.43550000013784 }, point2{ 602.01350000000093, 196.67450000019744 }, point2{ 602.01449999999022, 196.67249999986961 }, point2{ 602.03350000001956, 192.96350000007078 }, point2{ 601.97649999998976, 192.90549999987707 }, point2{ 600.08950000000186, 192.8804999999702 }, point2{ 600.08850000001257, 192.87950000027195 }, point2{ 600.06950000004144, 191.39550000010058 }, point2{ 602.00349999999162, 191.51649999991059 }, point2{ 602.06550000002608, 191.46249999990687 }, point2{ 602.14949999999953, 190.34349999995902 }, point2{ 602.14949999999953, 190.33950000023469 }, point2{ 602.14449999999488, 188.2894999999553 }, point2{ 602.23649999999907, 170.38750000018626 }, point2{ 602.23649999999907, 170.3855000003241 }, point2{ 602.16750000003958, 169.08449999988079 }, point2{ 602.11550000001444, 169.03049999987707 }, point2{ 600.33150000002934, 168.86550000030547 }, point2{ 600.35350000002654, 168.2785000000149 }, point2{ 600.35350000002654, 168.27750000031665 }, point2{ 600.67050000000745, 167.45149999996647 }, point2{ 601.48450000002049, 166.36849999986589 }, point2{ 602.40950000000885, 165.93350000027567 }, point2{ 603.06450000003679, 165.78850000025705 }, point2{ 603.06849999999395, 165.78850000025705 }, point2{ 604.23550000000978, 165.78750000009313 } };
		BOOST_CHECK(is_polygon_simple(geometry, cmp));
	}
}

#endif //GEOMETRIX_POINT_SEQUENCE_TESTS_HPP
