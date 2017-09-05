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
#include <geometrix/primitive/segment.hpp>
#include <geometrix/utility/utilities.hpp>
#include <geometrix/primitive/vector_point_sequence.hpp>
#include <geometrix/primitive/array_point_sequence.hpp>
#include <geometrix/primitive/point_sequence_utilities.hpp>
#include <geometrix/algorithm/point_in_polygon.hpp>
#include <geometrix/algorithm/point_sequence/is_polygon_simple.hpp>
#include <geometrix/algorithm/point_sequence/remove_collinear_points.hpp>
#include <geometrix/algorithm/point_sequence/polyline_mid_point.hpp>
#include <geometrix/primitive/polygon.hpp>
#include <geometrix/primitive/polyline.hpp>
#include <geometrix/tensor/matrix.hpp>
#include <geometrix/algorithm/rotation.hpp>
#include "2d_kernel_fixture.hpp"

BOOST_AUTO_TEST_CASE( TestPointSequences )
{
    using namespace geometrix;

    typedef point_double_2d point_2d;
    
    polygon< point_2d > polygon;

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
	typedef polygon<point2> polygon2;
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

BOOST_AUTO_TEST_CASE(TestPolylineMidPoint)
{
	using namespace geometrix;
	typedef point_double_2d point2;
	typedef polyline<point2> polyline2;

	{
		polyline2 poly{ point2(-10, -10), point2(10, -10), point2(10, 10), point2(-10, 10) };
		auto p = polyline_mid_point(poly);
		BOOST_CHECK(numeric_sequence_equals_2d(p, point2{ 10, 0 }, absolute_tolerance_comparison_policy<double>(1e-10)));
	}

	{
		polyline2 poly{ point2(-10, -10), point2(10, -10) };
		auto p = polyline_mid_point(poly);
		BOOST_CHECK(numeric_sequence_equals_2d(p, point2{ 0, -10 }, absolute_tolerance_comparison_policy<double>(1e-10)));
	}

	{
		auto pline = polyline2{ { 0.0 , 0.0 },{ 10.0 , 0.0 },{ 10.0 , 10.0 } };
		auto p = polyline_mid_point(pline);
		BOOST_CHECK(numeric_sequence_equals_2d(p, point2{ 10, 0 }, absolute_tolerance_comparison_policy<double>(1e-10)));
	}
}

#include <geometrix/algorithm/point_sequence/length.hpp>
BOOST_AUTO_TEST_CASE(TestPolylineLength)
{
	using namespace geometrix;
	typedef point_double_2d point2;
	typedef polyline<point2> polyline2;

	polyline2 poly{ point2(-10, -10), point2(10, -10), point2(10, 10), point2(-10, 10) };
	auto p = polyline_length(poly);
	BOOST_CHECK_CLOSE(p, 60.0, 1e-10);
}

BOOST_AUTO_TEST_CASE(TestPolygonLength)
{
	using namespace geometrix;
	typedef point_double_2d point2;
	typedef polygon<point2> polygon2;

	polygon2 poly{ point2(-10, -10), point2(10, -10), point2(10, 10), point2(-10, 10) };
	auto p = polygon_length(poly);
	BOOST_CHECK_CLOSE(p, 80.0, 1e-10);

	polygon2 box{ point2{ 12.916812848489371, -3.5448410595838595 }, point2{ 15.916812591514599, -3.5435993476755812 }, point2{ 15.916812591514599, -9.5435998616251254 }, point2{ 12.916812848489371, -9.5448415735334038 } };
	p = polygon_length(box);
}

#include <geometrix/utility/scope_timer.ipp>
#include <geometrix/algorithm/bsp_tree_2d.hpp>

template <typename Polygon>
void time_point_containment(const Polygon& poly, std::string name)
{
	using namespace geometrix;
	typedef point_double_2d point2;
	typedef segment_double_2d segment2;
	typedef vector_double_2d vector2;
	absolute_tolerance_comparison_policy<double> cmp(1e-10);
	typedef bsp_tree_2d< segment2 > bsp_tree2;
	bsp_tree2 tree(polygon_as_segment_range< segment2 >(poly), partition_policies::random_segment_selector_policy< segment2 >(), cmp);
	auto bounds_poly = get_bounds(poly, cmp);

	random_real_generator<> rng;
	vector2 x{ boost::get<e_xmax>(bounds_poly), 0 };
	vector2 y{ 0, boost::get<e_ymax>(bounds_poly) };
	std::vector<point2> points;
	point2 pmin{ boost::get<e_xmin>(bounds_poly), boost::get<e_ymin>(bounds_poly) };

	for (std::size_t i = 0; i < 1000; ++i)
	{
		points.emplace_back(pmin + rng() * x + rng() * y);
	}

#ifdef NDEBUG
	std::size_t runs = 100;
#else
	std::size_t runs = 1;
#endif

	std::vector<bool> results;
	{
		GEOMETRIX_MEASURE_SCOPE_TIME(name + "bsptree");
		for (std::size_t i = 0; i < runs; ++i)
		{
			for (std::size_t k = 0; k < points.size(); ++k)
				results.push_back(tree.locate_point(points[k], cmp) != e_outside);
		}
	}

	std::vector<bool> results2;
	{
		GEOMETRIX_MEASURE_SCOPE_TIME(name + "is_contained");
		for (std::size_t i = 0; i < runs; ++i)
		{
			for (std::size_t k = 0; k < points.size(); ++k)
				results2.push_back(point_in_polygon(points[k], poly));
		}
	}

	BOOST_CHECK(results == results2);
}

BOOST_AUTO_TEST_CASE(TimeIsPointContained)
{
	using namespace geometrix;
	typedef point_double_2d point2;
	typedef polygon<point2> polygon2;

	polygon2 simple{ point2(-10, -10), point2(10, -10), point2(10, 10), point2(-10, 10) };
	polygon2 complex{ point2{ 604.23649999999907, 165.78850000025705 }, point2{ 604.25150000001304, 166.7375000002794 }, point2{ 604.30850000004284, 166.79550000000745 }, point2{ 605.66750000003958, 166.80649999994785 }, point2{ 624.72350000002189, 166.87550000008196 }, point2{ 628.44949999998789, 166.88450000016019 }, point2{ 628.45150000002468, 166.88450000016019 }, point2{ 629.20650000002934, 166.84750000014901 }, point2{ 629.25250000000233, 166.7214999999851 }, point2{ 628.95350000000326, 166.4484999999404 }, point2{ 628.9484999999986, 166.44450000021607 }, point2{ 628.60450000001583, 166.2035000002943 }, point2{ 628.60350000002654, 166.20250000013039 }, point2{ 628.47750000003725, 165.87850000010803 }, point2{ 639.27050000004238, 165.92750000022352 }, point2{ 639.04250000003958, 166.13250000029802 }, point2{ 639.0395000000135, 166.13450000016019 }, point2{ 638.41550000000279, 166.30649999994785 }, point2{ 638.40049999998882, 166.3125 }, point2{ 638.01949999999488, 166.56749999988824 }, point2{ 638.04450000001816, 166.69750000024214 }, point2{ 639.2445000000298, 166.95050000026822 }, point2{ 639.25250000000233, 166.95149999996647 }, point2{ 640.74150000000373, 167.00849999999627 }, point2{ 640.7445000000298, 167.00849999999627 }, point2{ 644.17149999999674, 166.98150000022724 }, point2{ 653.36450000002515, 167.01649999991059 }, point2{ 653.41649999999208, 168.32750000013039 }, point2{ 639.21549999999115, 168.36349999997765 }, point2{ 639.21250000002328, 168.36349999997765 }, point2{ 637.08950000000186, 168.49250000016764 }, point2{ 637.03450000000885, 168.55349999992177 }, point2{ 637.1765000000014, 171.76550000021234 }, point2{ 637.24249999999302, 173.04350000014529 }, point2{ 637.24150000000373, 173.0445000003092 }, point2{ 629.80249999999069, 173.00950000016019 }, point2{ 629.9035000000149, 170.5625 }, point2{ 630.04450000001816, 168.1695000003092 }, point2{ 629.98749999998836, 168.10650000022724 }, point2{ 628.43249999999534, 168.06850000005215 }, point2{ 628.43150000000605, 168.06850000005215 }, point2{ 613.47850000002654, 168.07049999991432 }, point2{ 606.5515000000014, 168.0785000002943 }, point2{ 606.55050000001211, 168.0785000002943 }, point2{ 605.65850000001956, 168.10149999987334 }, point2{ 605.65750000003027, 168.10149999987334 }, point2{ 604.74150000000373, 168.13950000004843 }, point2{ 604.70350000000326, 168.15849999990314 }, point2{ 603.88050000002841, 169.14149999991059 }, point2{ 603.86850000004051, 169.17450000019744 }, point2{ 603.86050000000978, 170.34450000012293 }, point2{ 603.76449999999022, 191.48549999995157 }, point2{ 603.82250000000931, 191.5445000003092 }, point2{ 605.57050000003073, 191.5445000003092 }, point2{ 605.59549999999581, 192.93149999994785 }, point2{ 605.59650000004331, 192.93149999994785 }, point2{ 603.83449999999721, 192.95650000032037 }, point2{ 603.77650000003632, 193.01349999988452 }, point2{ 603.73749999998836, 196.73050000006333 }, point2{ 603.73649999999907, 196.73350000008941 }, point2{ 603.64949999999953, 199.32550000026822 }, point2{ 603.64949999999953, 199.32750000013039 }, point2{ 603.61350000003586, 204.13750000018626 }, point2{ 603.61350000003586, 204.1875 }, point2{ 603.64350000000559, 213.43949999986216 }, point2{ 603.70050000003539, 213.49750000005588 }, point2{ 616.91850000002887, 213.71950000012293 }, point2{ 616.58250000001863, 215.83250000001863 }, point2{ 608.55249999999069, 215.79850000003353 }, point2{ 603.1984999999986, 215.8015000000596 }, point2{ 603.19550000003073, 215.8015000000596 }, point2{ 601.36450000002515, 215.58850000007078 }, point2{ 601.36350000003586, 215.58749999990687 }, point2{ 601.36249999998836, 215.5625 }, point2{ 601.33250000001863, 215.51649999991059 }, point2{ 600.85250000003725, 215.27650000015274 }, point2{ 600.36650000000373, 214.79050000011921 }, point2{ 600.36650000000373, 214.7894999999553 }, point2{ 600.16250000003492, 214.20550000015646 }, point2{ 600.16250000003492, 213.56650000018999 }, point2{ 601.90450000000419, 213.53450000006706 }, point2{ 601.96150000003399, 213.47750000003725 }, point2{ 602.08150000002934, 204.17450000019744 }, point2{ 602.08150000002934, 204.17350000003353 }, point2{ 602.04850000003353, 199.43550000013784 }, point2{ 602.01350000000093, 196.67450000019744 }, point2{ 602.01449999999022, 196.67249999986961 }, point2{ 602.03350000001956, 192.96350000007078 }, point2{ 601.97649999998976, 192.90549999987707 }, point2{ 600.08950000000186, 192.8804999999702 }, point2{ 600.08850000001257, 192.87950000027195 }, point2{ 600.06950000004144, 191.39550000010058 }, point2{ 602.00349999999162, 191.51649999991059 }, point2{ 602.06550000002608, 191.46249999990687 }, point2{ 602.14949999999953, 190.34349999995902 }, point2{ 602.14949999999953, 190.33950000023469 }, point2{ 602.14449999999488, 188.2894999999553 }, point2{ 602.23649999999907, 170.38750000018626 }, point2{ 602.23649999999907, 170.3855000003241 }, point2{ 602.16750000003958, 169.08449999988079 }, point2{ 602.11550000001444, 169.03049999987707 }, point2{ 600.33150000002934, 168.86550000030547 }, point2{ 600.35350000002654, 168.2785000000149 }, point2{ 600.35350000002654, 168.27750000031665 }, point2{ 600.67050000000745, 167.45149999996647 }, point2{ 601.48450000002049, 166.36849999986589 }, point2{ 602.40950000000885, 165.93350000027567 }, point2{ 603.06450000003679, 165.78850000025705 }, point2{ 603.06849999999395, 165.78850000025705 }, point2{ 604.23550000000978, 165.78750000009313 } };

// 	time_point_containment(complex, "complex");
// 	time_point_containment(simple, "simple");
}

#include <geometrix/algorithm/point_sequence/is_congruent.hpp>
#include <boost/math/constants/constants.hpp>
namespace test_polyline_congruent_detail
{	
	geometrix::point<double,2> rotate_point(const geometrix::point<double,2>& p, double angle, const geometrix::point<double,2>& origin)
	{
		using namespace geometrix;

		double sina = sin(angle);
		double cosa = cos(angle);
		
		geometrix::point<double, 2> newp = p - as_vector(origin);
		double x = newp[0];
		double y = newp[1];
		newp[0] = x * cosa - y * sina;
		newp[1] = x * sina + y * cosa;
		return newp + as_vector(origin);		
	}
	template <typename Polyline>
	inline Polyline translate_rotate(const Polyline& poly, const geometrix::vector<double, 2>& translation, double rotationAngle, const geometrix::point<double,2> rotationOrigin)
	{
		using namespace geometrix;
		Polyline newPoly;
		for (const auto& p : poly)
			newPoly.emplace_back(rotate_point(p + translation, rotationAngle, rotationOrigin));
		return std::move(newPoly);
	}
}
BOOST_AUTO_TEST_CASE(TestPolylineCongruent)
{
	using namespace test_polyline_congruent_detail;
	using namespace geometrix;
	typedef point_double_2d point2;
	typedef vector_double_2d vector2;
	typedef polyline<point2> polyline2;
	absolute_tolerance_comparison_policy<double> cmp(1e-10);
	{
		polyline2 poly{ point2(-10, -10), point2(10, -10), point2(10, 10), point2(-10, 10) };
		polyline2 poly2{ point2(-10, -10), point2(10, -10), point2(10, 10), point2(-10, 10) };
		BOOST_CHECK(are_polylines_congruent(poly, poly2, cmp));
	}

	{
		polyline2 poly{ point2(-10, -10), point2(10, -10), point2(10, 10), point2(-10, 10) };
		polyline2 poly2 = translate_rotate(poly, vector2{ -10,-10 }, boost::math::constants::pi<double>() / 4.0, point2{ 5.0, 5.0 });
		bool result = are_polylines_congruent(poly, poly2, cmp);
		BOOST_CHECK(result);
	}

	{
		polyline2 poly{ point2{ 22.061754739456418, -28.061375952069763 }, point2{ 21.116519954870455, -25.578315029852092 }, point2{ 24.486751650401857, -24.506658169440925 }, point2{ 22.929814714472741, -21.437577894888818 }, point2{ 25.250997998227831, -21.171825721394271 }, point2{ 28.158782569186087, -22.019573262619737 } };
		polyline2 poly2{ point2{ 31.185195635600088, -30.306019306815454 }, point2{ 28.29290831729304, -29.466103640850633 }, point2{ 25.985972390102688, -29.744970626663417 }, point2{ 27.524123214243446, -32.787378201726824 }, point2{ 24.16520165122347, -33.892711666878313 }, point2{ 25.156756465341878, -36.424736908411745 } };
		std::reverse(poly2.begin(), poly2.end());
		bool result = are_polylines_congruent(poly, poly2, cmp);
		BOOST_CHECK(!result);
	}
	{
		polyline2 poly{ point2{ 335.21911099998488, 282.30080400034632 }, point2{ 335.22311099997023, 282.30880400072783 }, point2{ 335.18311099999119, 282.35180400079116 }, point2{ 334.81111100001726, 282.85580400051549 }, point2{ 334.81011099996977, 282.8568040006794 }, point2{ 334.16411100002006, 283.68480400042608 }, point2{ 333.53011100000003, 284.76980400038883 }, point2{ 333.51124662248219, 284.81741409628552 } };
		polyline2 poly2{ point2{ 333.50889589497137, 284.82271788540021 }, point2{ 333.53000000002794, 284.77000000048429 }, point2{ 334.16400000004796, 283.68500000052154 }, point2{ 334.81100000004517, 282.85600000061095 }, point2{ 334.81200000003446, 282.85500000044703 }, point2{ 335.18400000006659, 282.35100000025705 }, point2{ 335.22519345433545, 282.30642458002262 } };
		std::reverse(poly2.begin(), poly2.end());
		bool result = are_polylines_congruent(poly, poly2, cmp);
		BOOST_CHECK(!result);

		result = point_sequences_equal(poly, poly2, cmp);
		BOOST_CHECK(!result);
	}
}

BOOST_AUTO_TEST_CASE(TestRemoveCollinearPoints)
{
	using namespace geometrix;
	typedef point_double_2d point2;
	typedef polyline<point2> polyline2;
	typedef polygon<point2> polygon2;
	absolute_tolerance_comparison_policy<double> cmp(1e-10);
	{
		polygon2 geometry{ point2{ -489.07583699998213, -38.650772999506444 }, point2{ -489.01883699995233, -38.593772999476641 }, point2{ -489.0558369999635, -38.48077299958095 }, point2{ -489.08883699995931, -38.522772999480367 }, point2{ -489.07583699998213, -38.650772999506444 }, point2{ -489.65283699997235, -39.224772999528795 }, point2{ -489.73983699997189, -39.328772999346256 }, point2{ -490.27783699997235, -39.968772999476641 }, point2{ -490.27783699997235, -39.969772999640554 }, point2{ -490.3748369999812, -40.082772999536246 }, point2{ -490.50083699997049, -40.234772999770939 }, point2{ -490.50183699995978, -40.235772999469191 }, point2{ -490.85483699996257, -40.651772999670357 }, point2{ -490.86083699995652, -40.657772999722511 }, point2{ -491.09383699996397, -40.85577299958095 }, point2{ -491.09583699994255, -40.856772999744862 }, point2{ -491.30983699997887, -41.021772999782115 }, point2{ -491.45483699993929, -41.194772999733686 }, point2{ -491.61483699997189, -41.382772999349982 }, point2{ -491.62183699995512, -41.389772999566048 }, point2{ -491.90383699996164, -41.615772999357432 }, point2{ -489.71683699998539, -57.292772999498993 }, point2{ -488.58583699999144, -57.421772999688983 }, point2{ -487.47583699994721, -57.547772999387234 }, point2{ -487.47083699994255, -57.548772999551147 }, point2{ -487.2158369999961, -57.602772999554873 }, point2{ -487.1698369999649, -57.667772999498993 }, point2{ -487.17183699994348, -57.684772999491543 }, point2{ -487.13683699996909, -57.688772999681532 } };
		polygon2 cleaned = remove_collinear_points_polygon(geometry, cmp);
		BOOST_CHECK(geometry.size() == cleaned.size());//! no collinear points.
	}
	{
		polygon2 geometry{ point2(-10, -10), point2(0, -10), point2(10, -10), point2(10, 10), point2(-10, 10) };
		polygon2 cleaned = remove_collinear_points_polygon(geometry, cmp);
		BOOST_CHECK(geometry.size() == cleaned.size() + 1);
	}
	{
		polygon2 geometry{ point2(-10, -10), point2(0, -10), point2(10, -10), point2(10, 10), point2(-10, 10), point2(-10, 0) };
		polygon2 cleaned = remove_collinear_points_polygon(geometry, cmp);
		BOOST_CHECK(geometry.size() == cleaned.size() + 2);
		BOOST_CHECK(numeric_sequence_equals(cleaned.back(), point2{ -10, 10 }, cmp));
	}

	{
		polyline2 geometry{ point2{ -489.07583699998213, -38.650772999506444 }, point2{ -489.01883699995233, -38.593772999476641 }, point2{ -489.0558369999635, -38.48077299958095 }, point2{ -489.08883699995931, -38.522772999480367 }, point2{ -489.07583699998213, -38.650772999506444 }, point2{ -489.65283699997235, -39.224772999528795 }, point2{ -489.73983699997189, -39.328772999346256 }, point2{ -490.27783699997235, -39.968772999476641 }, point2{ -490.27783699997235, -39.969772999640554 }, point2{ -490.3748369999812, -40.082772999536246 }, point2{ -490.50083699997049, -40.234772999770939 }, point2{ -490.50183699995978, -40.235772999469191 }, point2{ -490.85483699996257, -40.651772999670357 }, point2{ -490.86083699995652, -40.657772999722511 }, point2{ -491.09383699996397, -40.85577299958095 }, point2{ -491.09583699994255, -40.856772999744862 }, point2{ -491.30983699997887, -41.021772999782115 }, point2{ -491.45483699993929, -41.194772999733686 }, point2{ -491.61483699997189, -41.382772999349982 }, point2{ -491.62183699995512, -41.389772999566048 }, point2{ -491.90383699996164, -41.615772999357432 }, point2{ -489.71683699998539, -57.292772999498993 }, point2{ -488.58583699999144, -57.421772999688983 }, point2{ -487.47583699994721, -57.547772999387234 }, point2{ -487.47083699994255, -57.548772999551147 }, point2{ -487.2158369999961, -57.602772999554873 }, point2{ -487.1698369999649, -57.667772999498993 }, point2{ -487.17183699994348, -57.684772999491543 }, point2{ -487.13683699996909, -57.688772999681532 } };
		polyline2 cleaned = remove_collinear_points_polyline(geometry, cmp);
		BOOST_CHECK(geometry.size() == cleaned.size());//! no collinear points.
	}
	{
		polyline2 geometry{ point2(-10, -10), point2(0, -10), point2(10, -10), point2(10, 10), point2(-10, 10) };
		polyline2 cleaned = remove_collinear_points_polyline(geometry, cmp);
		BOOST_CHECK(geometry.size() == cleaned.size() + 1);
	}
	{
		polyline2 geometry{ point2(-10, -10), point2(0, -10), point2(10, -10), point2(10, 10), point2(-10, 10), point2(-10, 0) };
		polyline2 cleaned = remove_collinear_points_polyline(geometry, cmp);
		BOOST_CHECK(geometry.size() == cleaned.size() + 1);
		BOOST_CHECK(numeric_sequence_equals(cleaned[1], point2{ 10, -10 }, cmp));
		BOOST_CHECK(numeric_sequence_equals(cleaned.back(), point2{ -10, 0 }, cmp));
	}
	{
		polygon2 geometry{ point2{ -1476.958665006212, -577.68051044084132 }, point2{ -1476.9022796276258, -578.50920464191586 }, point2{ -1476.9032796275569, -578.50920464191586 }, point2{ -1477.25327962765, -580.04720464162529 }, point2{ -1477.2532796275336, -580.04720464162529 }, point2{ -1477.6702796275495, -581.22820464149117 }, point2{ -1478.371279627434, -582.22320464160293 }, point2{ -1478.4514962100657, -582.31074424926192 }, point2{ -1459.6926846662536, -582.92704589385539 }, point2{ -1459.7822796276305, -582.86520464159548 }, point2{ -1459.7832796275616, -582.86420464143157 }, point2{ -1460.23527962761, -582.36720464192331 }, point2{ -1460.2382796276361, -582.36320464219898 }, point2{ -1460.7652796276379, -581.32220464199781 }, point2{ -1460.768279627664, -581.31520464178175 }, point2{ -1461.5320868219715, -578.11610636021942 } };
		polygon2 cleaned = coalesce_adjacent_points_polygon(geometry, cmp);
		BOOST_CHECK(geometry.size() == cleaned.size() + 1);
	}
	{
		polygon2 geometry{ point2{ -1476.958665006212, -577.68051044084132 }, point2{ -1476.9022796276258, -578.50920464191586 }, point2{ -1476.9032796275569, -578.50920464191586 }, point2{ -1477.25327962765, -580.04720464162529 }, point2{ -1477.2532796275336, -580.04720464162529 }, point2{ -1477.6702796275495, -581.22820464149117 }, point2{ -1478.371279627434, -582.22320464160293 }, point2{ -1478.4514962100657, -582.31074424926192 }, point2{ -1459.6926846662536, -582.92704589385539 }, point2{ -1459.7822796276305, -582.86520464159548 }, point2{ -1459.7832796275616, -582.86420464143157 }, point2{ -1460.23527962761, -582.36720464192331 }, point2{ -1460.2382796276361, -582.36320464219898 }, point2{ -1460.7652796276379, -581.32220464199781 }, point2{ -1460.768279627664, -581.31520464178175 }, point2{ -1461.5320868219715, -578.11610636021942 }, point2{ -1461.5320868219715, -578.11610636021555 } };
		polygon2 cleaned = coalesce_adjacent_points_polygon(geometry, cmp);
		BOOST_CHECK(geometry.size() == cleaned.size() + 2);
	}

	//! Clean
	{
		polygon2 geometry{ point2{ -489.07583699998213, -38.650772999506444 }, point2{ -489.01883699995233, -38.593772999476641 }, point2{ -489.0558369999635, -38.48077299958095 }, point2{ -489.08883699995931, -38.522772999480367 }, point2{ -489.07583699998213, -38.650772999506444 }, point2{ -489.65283699997235, -39.224772999528795 }, point2{ -489.73983699997189, -39.328772999346256 }, point2{ -490.27783699997235, -39.968772999476641 }, point2{ -490.27783699997235, -39.969772999640554 }, point2{ -490.3748369999812, -40.082772999536246 }, point2{ -490.50083699997049, -40.234772999770939 }, point2{ -490.50183699995978, -40.235772999469191 }, point2{ -490.85483699996257, -40.651772999670357 }, point2{ -490.86083699995652, -40.657772999722511 }, point2{ -491.09383699996397, -40.85577299958095 }, point2{ -491.09583699994255, -40.856772999744862 }, point2{ -491.30983699997887, -41.021772999782115 }, point2{ -491.45483699993929, -41.194772999733686 }, point2{ -491.61483699997189, -41.382772999349982 }, point2{ -491.62183699995512, -41.389772999566048 }, point2{ -491.90383699996164, -41.615772999357432 }, point2{ -489.71683699998539, -57.292772999498993 }, point2{ -488.58583699999144, -57.421772999688983 }, point2{ -487.47583699994721, -57.547772999387234 }, point2{ -487.47083699994255, -57.548772999551147 }, point2{ -487.2158369999961, -57.602772999554873 }, point2{ -487.1698369999649, -57.667772999498993 }, point2{ -487.17183699994348, -57.684772999491543 }, point2{ -487.13683699996909, -57.688772999681532 } };
		polygon2 cleaned = clean_polygon(geometry, cmp);
		BOOST_CHECK(geometry.size() == cleaned.size());//! no collinear points.
	}
	{
		polygon2 geometry{ point2(-10, -10), point2(0, -10), point2(10, -10), point2(10, 10), point2(-10, 10) };
		polygon2 cleaned = clean_polygon(geometry, cmp);
		BOOST_CHECK(geometry.size() == cleaned.size() + 1);
	}
	{
		polygon2 geometry{ point2(-10, -10), point2(0, -10), point2(10, -10), point2(10, 10), point2(-10, 10), point2(-10, 0) };
		polygon2 cleaned = clean_polygon(geometry, cmp);
		BOOST_CHECK(geometry.size() == cleaned.size() + 2);
		BOOST_CHECK(numeric_sequence_equals(cleaned.back(), point2{ -10, 10 }, cmp));
	}

	{
		polyline2 geometry{ point2{ -489.07583699998213, -38.650772999506444 }, point2{ -489.01883699995233, -38.593772999476641 }, point2{ -489.0558369999635, -38.48077299958095 }, point2{ -489.08883699995931, -38.522772999480367 }, point2{ -489.07583699998213, -38.650772999506444 }, point2{ -489.65283699997235, -39.224772999528795 }, point2{ -489.73983699997189, -39.328772999346256 }, point2{ -490.27783699997235, -39.968772999476641 }, point2{ -490.27783699997235, -39.969772999640554 }, point2{ -490.3748369999812, -40.082772999536246 }, point2{ -490.50083699997049, -40.234772999770939 }, point2{ -490.50183699995978, -40.235772999469191 }, point2{ -490.85483699996257, -40.651772999670357 }, point2{ -490.86083699995652, -40.657772999722511 }, point2{ -491.09383699996397, -40.85577299958095 }, point2{ -491.09583699994255, -40.856772999744862 }, point2{ -491.30983699997887, -41.021772999782115 }, point2{ -491.45483699993929, -41.194772999733686 }, point2{ -491.61483699997189, -41.382772999349982 }, point2{ -491.62183699995512, -41.389772999566048 }, point2{ -491.90383699996164, -41.615772999357432 }, point2{ -489.71683699998539, -57.292772999498993 }, point2{ -488.58583699999144, -57.421772999688983 }, point2{ -487.47583699994721, -57.547772999387234 }, point2{ -487.47083699994255, -57.548772999551147 }, point2{ -487.2158369999961, -57.602772999554873 }, point2{ -487.1698369999649, -57.667772999498993 }, point2{ -487.17183699994348, -57.684772999491543 }, point2{ -487.13683699996909, -57.688772999681532 } };
		polyline2 cleaned = clean_polyline(geometry, cmp);
		BOOST_CHECK(geometry.size() == cleaned.size());//! no collinear points.
	}
	{
		polyline2 geometry{ point2(-10, -10), point2(0, -10), point2(10, -10), point2(10, 10), point2(-10, 10) };
		polyline2 cleaned = clean_polyline(geometry, cmp);
		BOOST_CHECK(geometry.size() == cleaned.size() + 1);
	}
	{
		polyline2 geometry{ point2(-10, -10), point2(0, -10), point2(10, -10), point2(10, 10), point2(-10, 10), point2(-10, 0) };
		polyline2 cleaned = clean_polyline(geometry, cmp);
		BOOST_CHECK(geometry.size() == cleaned.size() + 1);
		BOOST_CHECK(numeric_sequence_equals(cleaned[1], point2{ 10, -10 }, cmp));
		BOOST_CHECK(numeric_sequence_equals(cleaned.back(), point2{ -10, 0 }, cmp));
	}
	{
		polygon2 geometry{ point2{ -1476.958665006212, -577.68051044084132 }, point2{ -1476.9022796276258, -578.50920464191586 }, point2{ -1476.9032796275569, -578.50920464191586 }, point2{ -1477.25327962765, -580.04720464162529 }, point2{ -1477.2532796275336, -580.04720464162529 }, point2{ -1477.6702796275495, -581.22820464149117 }, point2{ -1478.371279627434, -582.22320464160293 }, point2{ -1478.4514962100657, -582.31074424926192 }, point2{ -1459.6926846662536, -582.92704589385539 }, point2{ -1459.7822796276305, -582.86520464159548 }, point2{ -1459.7832796275616, -582.86420464143157 }, point2{ -1460.23527962761, -582.36720464192331 }, point2{ -1460.2382796276361, -582.36320464219898 }, point2{ -1460.7652796276379, -581.32220464199781 }, point2{ -1460.768279627664, -581.31520464178175 }, point2{ -1461.5320868219715, -578.11610636021942 } };
		polygon2 cleaned = clean_polygon(geometry, cmp);
		BOOST_CHECK(geometry.size() == cleaned.size() + 1);
	}
	{
		polygon2 geometry{ point2{ -1476.958665006212, -577.68051044084132 }, point2{ -1476.9022796276258, -578.50920464191586 }, point2{ -1476.9032796275569, -578.50920464191586 }, point2{ -1477.25327962765, -580.04720464162529 }, point2{ -1477.2532796275336, -580.04720464162529 }, point2{ -1477.6702796275495, -581.22820464149117 }, point2{ -1478.371279627434, -582.22320464160293 }, point2{ -1478.4514962100657, -582.31074424926192 }, point2{ -1459.6926846662536, -582.92704589385539 }, point2{ -1459.7822796276305, -582.86520464159548 }, point2{ -1459.7832796275616, -582.86420464143157 }, point2{ -1460.23527962761, -582.36720464192331 }, point2{ -1460.2382796276361, -582.36320464219898 }, point2{ -1460.7652796276379, -581.32220464199781 }, point2{ -1460.768279627664, -581.31520464178175 }, point2{ -1461.5320868219715, -578.11610636021942 }, point2{ -1461.5320868219715, -578.11610636021555 } };
		polygon2 cleaned = clean_polygon(geometry, cmp);
		BOOST_CHECK(geometry.size() == cleaned.size() + 2);
	}
}

BOOST_AUTO_TEST_CASE(TestRotatePolygon)
{
	using namespace geometrix;
	typedef point_double_2d point2;
	typedef polygon<point2> polygon2;
	typedef vector<double, 2> vector2;

	absolute_tolerance_comparison_policy<double> cmp(1e-10);

	//! Rotate 45.
	{
		polygon2 geometry{ point2(0., 0.), point2(10., 0.), point2(15., 5.), point2(10., 10.), point2(0., 10.), point2(5., 5.) };
		polygon2 rotated = rotate_points(geometry, normalize(vector2{ 1, 1 }), normalize(vector2{ 0, 1 }), point2(5., 5.));
		BOOST_CHECK(true);
	}
	
}

#include <geometrix/algorithm/point_sequence/polygon_polygon_containment.hpp>
BOOST_AUTO_TEST_CASE(TestPolygonPolygonContainment)
{
	using namespace geometrix;
	typedef point_double_2d point2;
	typedef polygon<point2> polygon2;
	typedef vector<double, 2> vector2;

	absolute_tolerance_comparison_policy<double> cmp(1e-10);

	//! Rotate 45.
	{
		polygon2 geometry{ point2(0., 0.), point2(10., 0.), point2(15., 5.), point2(10., 10.), point2(0., 10.), point2(5., 5.) };
		polygon2 rotated = rotate_points(geometry, normalize(vector2{ 1, 1 }), normalize(vector2{ 0, 1 }), point2(5., 5.));
		BOOST_CHECK(!polygon_polygon_containment(geometry, rotated, cmp));
	}

	{
		polygon2 geometry1{ point2(0., 0.), point2(10., 0.), point2(15., 5.), point2(10., 10.), point2(0., 10.), point2(5., 5.) };
		polygon2 geometry2{ point2(1., 1.), point2(9., 1.), point2(14., 4.), point2(9., 9.), point2(1., 9.), point2(6., 6.) };
		BOOST_CHECK(polygon_polygon_containment(geometry2, geometry1, cmp));
	}

	{
		polygon2 geometry1{ point2(0., 0.), point2(5., 0.), point2(6., 1.), point2(7., 0.), point2(10., 0.), point2(10., 10.), point2(0, 10) };
		polygon2 geometry2{ point2(1., 0.), point2(9., 0.), point2(9., 5.), point2(1, 5) };
		bool result = polygon_polygon_containment(geometry2, geometry1, cmp);
		BOOST_CHECK(result == false);
	}
}

#include <geometrix/algorithm/point_sequence/polygon_polyline_containment.hpp>
BOOST_AUTO_TEST_CASE(TestPolygonPolylineContainment)
{
	using namespace geometrix;
	typedef point_double_2d point2;
	typedef polygon<point2> polygon2;
	typedef polyline<point2> polyline2;
	typedef vector<double, 2> vector2;

	absolute_tolerance_comparison_policy<double> cmp(1e-10);

	//! Rotate 45.
	{
		polygon2 geometry{ point2(0., 0.), point2(10., 0.), point2(15., 5.), point2(10., 10.), point2(0., 10.), point2(5., 5.) };
		auto rotated = rotate_points(geometry, normalize(vector2{ 1, 1 }), normalize(vector2{ 0, 1 }), point2(5., 5.));
		polyline2 pline(rotated.begin(), rotated.end());
		BOOST_CHECK(!polygon_polyline_containment(geometry, pline, cmp));
	}

	{
		polygon2 geometry1{ point2(0., 0.), point2(10., 0.), point2(15., 5.), point2(10., 10.), point2(0., 10.), point2(5., 5.) };
		polyline2 geometry2{ point2(1., 1.), point2(9., 1.), point2(14., 4.), point2(9., 9.), point2(1., 9.), point2(6., 6.) };
		BOOST_CHECK(polygon_polyline_containment(geometry2, geometry1, cmp));
	}

	{
		polygon2 geometry1{ point2(0., 0.), point2(5., 0.), point2(6., 1.), point2(7., 0.), point2(10., 0.), point2(10., 10.), point2(0, 10) };
		polyline2 geometry2{ point2(1., 0.), point2(9., 0.), point2(9., 5.), point2(1, 5) };
		bool result = polygon_polyline_containment(geometry2, geometry1, cmp);
		BOOST_CHECK(result == false);
	}

	{
		polygon2 geometry{ point2(0., 0.), point2(10., 0.), point2(15., 5.), point2(10., 10.), point2(0., 10.), point2(5., 5.) };
		polyline2 geometry2{ point2(0., 0.), point2(10., 0.), point2(15., 5.), point2(10., 10.), point2(0., 10.), point2(5., 5.) };
	    bool result = polygon_polyline_containment(geometry, geometry2, cmp);
		BOOST_CHECK(result);
	}

	{
		polygon2 geometry1{ point2(0., 0.), point2(10., 0.), point2(15., 5.), point2(10., 10.), point2(0., 10.), point2(5., 5.) };
		polyline2 geometry2{ point2(0., 0.), point2(10., 0.), point2(15., 5.), point2(10., 10.), point2(0., 10.), point2(5., 5.) };
		bool result = polygon_polyline_containment(geometry2, geometry1, cmp);
		BOOST_CHECK(result);
	}

	{
		polygon2 geometry1{ point2(0., 0.), point2(5., 0.), point2(6., 1.), point2(7., 0.), point2(10., 0.), point2(10., 10.), point2(0, 10) };
		polyline2 geometry2{ point2(0., 0.), point2(5., 0.), point2(6., 1.), point2(7., 0.), point2(10., 0.), point2(10., 10.), point2(0, 10) };
		bool result = polygon_polyline_containment(geometry2, geometry1, cmp);
		BOOST_CHECK(result);
	}

	{
		typedef segment<point2> segment2;
		polygon2 pgon{ point2{ 339.92894999997225, 281.53395000007004 }, point2{ 339.97594999993453, 281.58195000002161 }, point2{ 341.46094999997877, 281.81795000005513 }, point2{ 341.46894999995129, 281.81894999975339 }, point2{ 345.56894999998622, 281.81795000005513 }, point2{ 347.66594999993686, 281.87094999989495 }, point2{ 347.66694999998435, 281.87094999989495 }, point2{ 350.70294999994803, 281.8999499999918 }, point2{ 350.70394999993732, 281.8999499999918 }, point2{ 353.15094999998109, 281.8879499998875 }, point2{ 353.20994999993127, 281.79795000003651 }, point2{ 352.41594999993686, 279.95494999969378 }, point2{ 364.73494999995455, 280.02194999996573 }, point2{ 364.73794999998063, 280.02294999966398 }, point2{ 392.97994999994989, 280.21194999990985 }, point2{ 393.65094999998109, 280.21494999993593 }, point2{ 394.64894999994431, 280.21894999966025 }, point2{ 397.64594999997644, 280.23094999976456 }, point2{ 398.64494999992894, 280.2359499996528 }, point2{ 402.86294999998063, 280.24894999992102 }, point2{ 410.43594999995548, 280.29495000001043 }, point2{ 410.35294999997132, 282.50994999986142 }, point2{ 410.4119499999797, 282.57194999977946 }, point2{ 411.77594999998109, 282.58594999974594 }, point2{ 411.80594999995083, 282.58594999974594 }, point2{ 411.85694999992847, 282.58694999990985 }, point2{ 412.52094999997644, 282.59694999968633 }, point2{ 412.58094999997411, 282.53794999979436 }, point2{ 412.5819499999634, 281.48194999992847 }, point2{ 412.52394999994431, 281.42295000003651 }, point2{ 411.43494999996619, 281.41794999968261 }, point2{ 411.44094999996014, 280.30095000006258 }, point2{ 421.64094999997178, 280.33195000002161 }, point2{ 421.64394999993965, 280.33294999971986 }, point2{ 428.75094999995781, 280.38094999967143 }, point2{ 428.7519499999471, 280.38094999967143 }, point2{ 429.07994999998482, 285.34895000001416 }, point2{ 429.07894999993732, 285.34994999971241 }, point2{ 421.30394999997225, 285.40895000007004 }, point2{ 421.29894999996759, 285.4109499999322 }, point2{ 411.86094999994384, 285.36794999986887 }, point2{ 411.80194999993546, 285.42595000006258 }, point2{ 411.74494999996386, 296.25994999986142 }, point2{ 410.19894999993267, 296.2699499996379 }, point2{ 410.19794999994338, 296.26894999993965 }, point2{ 410.33294999995269, 283.07294999994338 }, point2{ 410.2749499999336, 283.0129499998875 }, point2{ 402.6609499999322, 282.96094999974594 }, point2{ 402.65794999996433, 282.95995000004768 }, point2{ 400.11894999997457, 282.9339499999769 }, point2{ 400.19894999993267, 282.68494999967515 }, point2{ 400.13994999998249, 282.60294999973848 }, point2{ 397.53094999998575, 282.55395000008866 }, point2{ 397.52394999994431, 282.55294999992475 }, point2{ 396.95794999995269, 282.41394999995828 }, point2{ 396.94694999995409, 282.41195000009611 }, point2{ 392.8769499999471, 282.3609499996528 }, point2{ 392.87594999995781, 282.3609499996528 }, point2{ 376.36894999997457, 282.44295000005513 }, point2{ 376.31094999995548, 282.49794999975711 }, point2{ 376.21994999994058, 284.36194999981672 }, point2{ 374.89594999997644, 284.3539499999024 }, point2{ 374.89494999992894, 284.35294999973848 }, point2{ 375.13594999996712, 282.49594999989495 }, point2{ 375.07694999995874, 282.42794999992475 }, point2{ 366.8659499999485, 282.38094999967143 }, point2{ 366.80794999992941, 282.43695000000298 }, point2{ 366.77594999998109, 283.23994999984279 }, point2{ 366.7749499999336, 283.24095000000671 }, point2{ 364.60094999993453, 283.29994999989867 }, point2{ 364.58994999993593, 283.29994999989867 }, point2{ 364.5479499999783, 283.30095000006258 }, point2{ 364.54594999994151, 283.29994999989867 }, point2{ 362.80994999996619, 283.30594999995083 }, point2{ 362.7519499999471, 283.36494999984279 }, point2{ 362.76694999996107, 288.98194999992847 }, point2{ 362.76594999997178, 288.98295000009239 }, point2{ 353.72494999994524, 288.90994999976829 }, point2{ 353.76494999998249, 283.44094999972731 }, point2{ 353.7069499999634, 283.38194999983534 }, point2{ 342.57694999995874, 283.27594999969006 }, point2{ 342.51794999995036, 283.33094999985769 }, point2{ 342.35194999998203, 286.62894999980927 }, point2{ 342.3769499999471, 286.67794999992475 }, point2{ 343.45994999993127, 287.33894999977201 }, point2{ 343.46094999997877, 287.33994999993593 }, point2{ 343.25794999994105, 287.52694999985397 }, point2{ 343.24594999995315, 287.59194999979809 }, point2{ 343.39894999994431, 287.90794999990612 }, point2{ 343.40094999998109, 287.91195000009611 }, point2{ 343.61094999994384, 288.25494999997318 }, point2{ 343.77994999993825, 288.83394999988377 }, point2{ 343.78094999998575, 288.83994999993593 }, point2{ 343.79494999995222, 289.59194999979809 }, point2{ 343.50694999995176, 290.11294999998063 }, point2{ 343.12494999996852, 290.63895000005141 }, point2{ 343.12094999995315, 290.64294999977574 }, point2{ 342.74594999995315, 290.93294999981299 }, point2{ 342.74494999996386, 290.93294999981299 }, point2{ 342.23094999993918, 291.23794999998063 }, point2{ 342.2069499999634, 291.30395000008866 }, point2{ 342.24994999996852, 291.43194999964908 }, point2{ 339.65994999994291, 291.35795000009239 }, point2{ 338.50694999995176, 290.12894999980927 }, point2{ 338.50694999995176, 290.12794999964535 }, point2{ 338.5819499999634, 287.78794999979436 }, point2{ 339.53894999995828, 286.7699499996379 }, point2{ 340.48694999993313, 286.5589499999769 }, point2{ 340.53094999998575, 286.50794999999925 }, point2{ 340.72494999994524, 283.27194999996573 }, point2{ 340.66894999996293, 283.20995000004768 }, point2{ 340.19794999994338, 283.19194999989122 }, point2{ 340.19894999993267, 283.16594999982044 }, point2{ 340.15194999997038, 283.10594999976456 }, point2{ 338.68494999996619, 282.85195000004023 }, point2{ 338.6379499999457, 282.86694999970496 }, point2{ 335.38494999997783, 286.20094999996945 }, point2{ 335.38394999993034, 286.2019499996677 }, point2{ 334.64794999995502, 286.99594999989495 }, point2{ 334.63394999993034, 287.03294999990612 }, point2{ 334.92094999994151, 293.84094999963418 }, point2{ 335.81694999994943, 315.72094999998808 }, point2{ 335.81694999994943, 315.72294999985024 }, point2{ 336.46094999997877, 324.49894999992102 }, point2{ 336.46194999996806, 324.50294999964535 }, point2{ 336.76494999998249, 326.57994999969378 }, point2{ 336.81894999998622, 326.62894999980927 }, point2{ 339.03994999994757, 326.71094999974594 }, point2{ 339.04294999997364, 326.71094999974594 }, point2{ 342.38694999995641, 327.19894999964163 }, point2{ 342.45594999997411, 327.1379499998875 }, point2{ 342.4459499999648, 325.69394999975339 }, point2{ 342.44694999995409, 325.69295000005513 }, point2{ 343.72094999992987, 325.71594999963418 }, point2{ 343.71994999994058, 325.71594999963418 }, point2{ 343.73594999994384, 327.32594999996945 }, point2{ 343.75094999995781, 331.63595000002533 }, point2{ 343.74994999996852, 331.63694999972358 }, point2{ 337.48894999996992, 331.52694999985397 }, point2{ 337.47894999996061, 331.52795000001788 }, point2{ 335.55494999996154, 331.90994999976829 }, point2{ 334.37594999995781, 322.70294999983162 }, point2{ 334.37494999996852, 322.70094999996945 }, point2{ 334.31694999994943, 321.85294999973848 }, point2{ 334.31694999994943, 321.84794999985024 }, point2{ 333.09094999998342, 288.23494999995455 }, point2{ 333.04494999995222, 286.39394999993965 }, point2{ 333.04194999998435, 286.38494999986142 }, point2{ 333.04294999997364, 286.38194999983534 }, point2{ 333.04394999996293, 286.36794999986887 }, point2{ 333.0469499999308, 286.36294999998063 }, point2{ 333.28094999998575, 285.39195000007749 }, point2{ 333.52994999993825, 284.7699499996379 }, point2{ 334.16394999995828, 283.68494999967515 }, point2{ 334.81094999995548, 282.85594999976456 }, point2{ 334.81194999994477, 282.85495000006631 }, point2{ 335.1839499999769, 282.35094999987632 }, point2{ 335.92694999993546, 281.5469499998726 }, point2{ 337.04094999993686, 280.81495000002906 }, point2{ 338.05594999995083, 280.33795000007376 }, point2{ 338.0589499999769, 280.33594999974594 }, point2{ 338.06294999993406, 280.33394999988377 }, point2{ 338.06994999997551, 280.33195000002161 }, point2{ 338.0819499999634, 280.32594999996945 }, point2{ 338.08694999996806, 280.3269499996677 }, point2{ 338.8319499999634, 280.08894999977201 }, point2{ 339.71494999993593, 279.94194999989122 } };
		point2 pop{ 335.55539999995381, 281.94894999987446 };
		bool onBorder = false;
		double distance = 10.0;
		for (std::size_t i = 0; i < pgon.size(); ++i)
		{
			std::size_t j = (i + 1) % pgon.size();
			segment2 s(pgon[i], pgon[j]);
			if (is_between(pgon[i], pgon[j], pop, true, cmp))
			{
				onBorder = true;
				break;
			}

			double d = point_segment_distance(pop, segment2{ pgon[i], pgon[j] });
			if ( d < distance)
			{
				distance = d;
			}
		}

		BOOST_CHECK(!onBorder);
	}
}

#include <geometrix/algorithm/point_sequence/clip_polyline_ends.hpp>
BOOST_AUTO_TEST_CASE(TestClipPolylineEnds)
{
	using namespace geometrix;
	typedef point_double_2d point2;
	typedef polyline<point2> polyline2;
	absolute_tolerance_comparison_policy<double> cmp(1e-10);

	{
		polyline2 geometry{ point2(0., 0.), point2(10., 0.), point2(15., 5.), point2(10., 10.), point2(0., 10.), point2(5., 5.) };
		polyline2 clipped = clip_polyline_front(geometry, 5.0, cmp);
		BOOST_CHECK(numeric_sequence_equals(clipped.front(), point2{ 5, 0 }, cmp));
	}

	{
		polyline2 geometry{ point2(0., 0.), point2(10., 0.), point2(15., 5.), point2(10., 10.), point2(0., 10.), point2(5., 5.) };
		polyline2 clipped = clip_polyline_front(geometry, 40.0, cmp);
		BOOST_CHECK(point_sequences_equal(clipped, polyline2{ point2{ 4.1421356237309501, 5.8578643762690499 }, point2{ 5, 5 } }, cmp));
	}

	{
		polyline2 geometry{ point2(0., 0.), point2(10., 0.), point2(15., 5.), point2(10., 10.), point2(0., 10.), point2(5., 5.) };
		polyline2 clipped = clip_polyline_front(geometry, 15.0, cmp);
		BOOST_CHECK(numeric_sequence_equals(clipped.front(), point2{ 13.535533905932738, 3.5355339059327378 }, cmp));
	}

	{
		polyline2 geometry{ point2(0., 0.), point2(10., 0.), point2(10., 5.), point2(10., 10.), point2(0., 10.), point2(5., 5.) };
		polyline2 clipped = clip_polyline_front(geometry, 15.0, cmp);
		BOOST_CHECK(point_sequences_equal(clipped, polyline2{ point2(10., 5.), point2(10., 10.), point2(0., 10.), point2(5., 5.) }, cmp));
	}

	{
		polyline2 geometry{ point2(0., 0.), point2(10., 0.) };
		polyline2 clipped = clip_polyline_front(geometry, 1.0, cmp);
		BOOST_CHECK(point_sequences_equal(clipped, polyline2{ point2(1., 0.), point2(10., 0.) }, cmp));
	}

	{
		polyline2 geometry{ point2{ 339.92894999997225, 281.53395000007004 }, point2{ 339.97594999993453, 281.58195000002161 }, point2{ 341.46094999997877, 281.81795000005513 }, point2{ 341.46894999995129, 281.81894999975339 }, point2{ 345.56894999998622, 281.81795000005513 }, point2{ 347.66594999993686, 281.87094999989495 }, point2{ 347.66694999998435, 281.87094999989495 }, point2{ 350.70294999994803, 281.8999499999918 }, point2{ 350.70394999993732, 281.8999499999918 }, point2{ 353.15094999998109, 281.8879499998875 }, point2{ 353.20994999993127, 281.79795000003651 }, point2{ 352.41594999993686, 279.95494999969378 }, point2{ 364.73494999995455, 280.02194999996573 }, point2{ 364.73794999998063, 280.02294999966398 }, point2{ 392.97994999994989, 280.21194999990985 }, point2{ 393.65094999998109, 280.21494999993593 }, point2{ 394.64894999994431, 280.21894999966025 }, point2{ 397.64594999997644, 280.23094999976456 }, point2{ 398.64494999992894, 280.2359499996528 }, point2{ 402.86294999998063, 280.24894999992102 }, point2{ 410.43594999995548, 280.29495000001043 }, point2{ 410.35294999997132, 282.50994999986142 }, point2{ 410.4119499999797, 282.57194999977946 }, point2{ 411.77594999998109, 282.58594999974594 }, point2{ 411.80594999995083, 282.58594999974594 }, point2{ 411.85694999992847, 282.58694999990985 }, point2{ 412.52094999997644, 282.59694999968633 }, point2{ 412.58094999997411, 282.53794999979436 }, point2{ 412.5819499999634, 281.48194999992847 }, point2{ 412.52394999994431, 281.42295000003651 }, point2{ 411.43494999996619, 281.41794999968261 }, point2{ 411.44094999996014, 280.30095000006258 }, point2{ 421.64094999997178, 280.33195000002161 }, point2{ 421.64394999993965, 280.33294999971986 }, point2{ 428.75094999995781, 280.38094999967143 }, point2{ 428.7519499999471, 280.38094999967143 }, point2{ 429.07994999998482, 285.34895000001416 }, point2{ 429.07894999993732, 285.34994999971241 }, point2{ 421.30394999997225, 285.40895000007004 }, point2{ 421.29894999996759, 285.4109499999322 }, point2{ 411.86094999994384, 285.36794999986887 }, point2{ 411.80194999993546, 285.42595000006258 }, point2{ 411.74494999996386, 296.25994999986142 }, point2{ 410.19894999993267, 296.2699499996379 }, point2{ 410.19794999994338, 296.26894999993965 }, point2{ 410.33294999995269, 283.07294999994338 }, point2{ 410.2749499999336, 283.0129499998875 }, point2{ 402.6609499999322, 282.96094999974594 }, point2{ 402.65794999996433, 282.95995000004768 }, point2{ 400.11894999997457, 282.9339499999769 }, point2{ 400.19894999993267, 282.68494999967515 }, point2{ 400.13994999998249, 282.60294999973848 }, point2{ 397.53094999998575, 282.55395000008866 }, point2{ 397.52394999994431, 282.55294999992475 }, point2{ 396.95794999995269, 282.41394999995828 }, point2{ 396.94694999995409, 282.41195000009611 }, point2{ 392.8769499999471, 282.3609499996528 }, point2{ 392.87594999995781, 282.3609499996528 }, point2{ 376.36894999997457, 282.44295000005513 }, point2{ 376.31094999995548, 282.49794999975711 }, point2{ 376.21994999994058, 284.36194999981672 }, point2{ 374.89594999997644, 284.3539499999024 }, point2{ 374.89494999992894, 284.35294999973848 }, point2{ 375.13594999996712, 282.49594999989495 }, point2{ 375.07694999995874, 282.42794999992475 }, point2{ 366.8659499999485, 282.38094999967143 }, point2{ 366.80794999992941, 282.43695000000298 }, point2{ 366.77594999998109, 283.23994999984279 }, point2{ 366.7749499999336, 283.24095000000671 }, point2{ 364.60094999993453, 283.29994999989867 }, point2{ 364.58994999993593, 283.29994999989867 }, point2{ 364.5479499999783, 283.30095000006258 }, point2{ 364.54594999994151, 283.29994999989867 }, point2{ 362.80994999996619, 283.30594999995083 }, point2{ 362.7519499999471, 283.36494999984279 }, point2{ 362.76694999996107, 288.98194999992847 }, point2{ 362.76594999997178, 288.98295000009239 }, point2{ 353.72494999994524, 288.90994999976829 }, point2{ 353.76494999998249, 283.44094999972731 }, point2{ 353.7069499999634, 283.38194999983534 }, point2{ 342.57694999995874, 283.27594999969006 }, point2{ 342.51794999995036, 283.33094999985769 }, point2{ 342.35194999998203, 286.62894999980927 }, point2{ 342.3769499999471, 286.67794999992475 }, point2{ 343.45994999993127, 287.33894999977201 }, point2{ 343.46094999997877, 287.33994999993593 }, point2{ 343.25794999994105, 287.52694999985397 }, point2{ 343.24594999995315, 287.59194999979809 }, point2{ 343.39894999994431, 287.90794999990612 }, point2{ 343.40094999998109, 287.91195000009611 }, point2{ 343.61094999994384, 288.25494999997318 }, point2{ 343.77994999993825, 288.83394999988377 }, point2{ 343.78094999998575, 288.83994999993593 }, point2{ 343.79494999995222, 289.59194999979809 }, point2{ 343.50694999995176, 290.11294999998063 }, point2{ 343.12494999996852, 290.63895000005141 }, point2{ 343.12094999995315, 290.64294999977574 }, point2{ 342.74594999995315, 290.93294999981299 }, point2{ 342.74494999996386, 290.93294999981299 }, point2{ 342.23094999993918, 291.23794999998063 }, point2{ 342.2069499999634, 291.30395000008866 }, point2{ 342.24994999996852, 291.43194999964908 }, point2{ 339.65994999994291, 291.35795000009239 }, point2{ 338.50694999995176, 290.12894999980927 }, point2{ 338.50694999995176, 290.12794999964535 }, point2{ 338.5819499999634, 287.78794999979436 }, point2{ 339.53894999995828, 286.7699499996379 }, point2{ 340.48694999993313, 286.5589499999769 }, point2{ 340.53094999998575, 286.50794999999925 }, point2{ 340.72494999994524, 283.27194999996573 }, point2{ 340.66894999996293, 283.20995000004768 }, point2{ 340.19794999994338, 283.19194999989122 }, point2{ 340.19894999993267, 283.16594999982044 }, point2{ 340.15194999997038, 283.10594999976456 }, point2{ 338.68494999996619, 282.85195000004023 }, point2{ 338.6379499999457, 282.86694999970496 }, point2{ 335.38494999997783, 286.20094999996945 }, point2{ 335.38394999993034, 286.2019499996677 }, point2{ 334.64794999995502, 286.99594999989495 }, point2{ 334.63394999993034, 287.03294999990612 }, point2{ 334.92094999994151, 293.84094999963418 }, point2{ 335.81694999994943, 315.72094999998808 }, point2{ 335.81694999994943, 315.72294999985024 }, point2{ 336.46094999997877, 324.49894999992102 }, point2{ 336.46194999996806, 324.50294999964535 }, point2{ 336.76494999998249, 326.57994999969378 }, point2{ 336.81894999998622, 326.62894999980927 }, point2{ 339.03994999994757, 326.71094999974594 }, point2{ 339.04294999997364, 326.71094999974594 }, point2{ 342.38694999995641, 327.19894999964163 }, point2{ 342.45594999997411, 327.1379499998875 }, point2{ 342.4459499999648, 325.69394999975339 }, point2{ 342.44694999995409, 325.69295000005513 }, point2{ 343.72094999992987, 325.71594999963418 }, point2{ 343.71994999994058, 325.71594999963418 }, point2{ 343.73594999994384, 327.32594999996945 }, point2{ 343.75094999995781, 331.63595000002533 }, point2{ 343.74994999996852, 331.63694999972358 }, point2{ 337.48894999996992, 331.52694999985397 }, point2{ 337.47894999996061, 331.52795000001788 }, point2{ 335.55494999996154, 331.90994999976829 }, point2{ 334.37594999995781, 322.70294999983162 }, point2{ 334.37494999996852, 322.70094999996945 }, point2{ 334.31694999994943, 321.85294999973848 }, point2{ 334.31694999994943, 321.84794999985024 }, point2{ 333.09094999998342, 288.23494999995455 }, point2{ 333.04494999995222, 286.39394999993965 }, point2{ 333.04194999998435, 286.38494999986142 }, point2{ 333.04294999997364, 286.38194999983534 }, point2{ 333.04394999996293, 286.36794999986887 }, point2{ 333.0469499999308, 286.36294999998063 }, point2{ 333.28094999998575, 285.39195000007749 }, point2{ 333.52994999993825, 284.7699499996379 }, point2{ 334.16394999995828, 283.68494999967515 }, point2{ 334.81094999995548, 282.85594999976456 }, point2{ 334.81194999994477, 282.85495000006631 }, point2{ 335.1839499999769, 282.35094999987632 }, point2{ 335.92694999993546, 281.5469499998726 }, point2{ 337.04094999993686, 280.81495000002906 }, point2{ 338.05594999995083, 280.33795000007376 }, point2{ 338.0589499999769, 280.33594999974594 }, point2{ 338.06294999993406, 280.33394999988377 }, point2{ 338.06994999997551, 280.33195000002161 }, point2{ 338.0819499999634, 280.32594999996945 }, point2{ 338.08694999996806, 280.3269499996677 }, point2{ 338.8319499999634, 280.08894999977201 }, point2{ 339.71494999993593, 279.94194999989122 } };
		polyline2 clipped = clip_polyline_front(geometry, polyline_length(geometry), cmp);
		BOOST_CHECK(clipped.empty());
	}
	
	{
		polyline2 geometry{ point2(0., 0.), point2(10., 0.), point2(15., 5.), point2(10., 10.), point2(0., 10.), point2(5., 5.) };
		polyline2 clipped = clip_polyline_back(geometry, 5.0, cmp);
		BOOST_CHECK(point_sequences_equal(clipped, polyline2{ point2(0., 0.), point2(10.,0.), point2(15., 5.), point2(10., 10.), point2(0., 10.),  point2{ 1.4644660940672625, 8.5355339059327378 } }, cmp));
	}

	{
		polyline2 geometry{ point2(0., 0.), point2(10., 0.) };
		polyline2 clipped = clip_polyline_back(geometry, 1.0, cmp);
		BOOST_CHECK(point_sequences_equal(clipped, polyline2{ point2(0., 0.), point2(9., 0.) }, cmp));
	}

	{
		polyline2 geometry{ point2(0., 0.), point2(10., 0.), point2(15., 5.), point2(10., 10.), point2(0., 10.), point2(5., 5.) };
		polyline2 clipped = clip_polyline_ends(geometry, 5.0, cmp);
		BOOST_CHECK(point_sequences_equal(clipped, polyline2{ point2(5., 0.), point2(10.,0.), point2(15., 5.), point2(10., 10.), point2(0., 10.),  point2{ 1.4644660940672625, 8.5355339059327378 } }, cmp));
	}

	{
		polyline2 geometry{ point2(0., 0.), point2(10., 0.) };
		polyline2 clipped = clip_polyline_ends(geometry, 1.0, cmp);
		BOOST_CHECK(point_sequences_equal(clipped, polyline2{ point2(1., 0.), point2(9., 0.) }, cmp));
	}

	{
		polyline2 geometry{ point2{ 81.468972358786075, 309.98166757887907 }, point2{ 81.773983506490453, 309.96489838253967 } };
		polyline2 clipped = clip_polyline_ends(geometry, 0.2, cmp);
		BOOST_CHECK(clipped.empty());
	}

	{
		polyline2 geometry{};
		polyline2 clipped = clip_polyline_front(geometry, 0.2, cmp);
		BOOST_CHECK(clipped.empty());
	}

	{
		polyline2 geometry{};
		polyline2 clipped = clip_polyline_back(geometry, 0.2, cmp);
		BOOST_CHECK(clipped.empty());
	}

	{
		polyline2 geometry{};
		polyline2 clipped = clip_polyline_ends(geometry, 0.2, cmp);
		BOOST_CHECK(clipped.empty());
	}
}

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
BOOST_AUTO_TEST_CASE(TestBoostGeometryPolygonIntersection)
{
	using namespace geometrix;
	typedef point_double_2d point2;
	typedef polygon<point2> polygon2;
	typedef vector_double_2d vector2;
	absolute_tolerance_comparison_policy<double> cmp(1e-10);

	polygon2 geometry{ point2(0., 0.), point2(10., 0.), point2(15., 5.), point2(10., 10.), point2(0., 10.), point2(5., 5.) };
	polygon2 rotated = rotate_points(geometry, normalize(vector2{ 1, 1 }), normalize(vector2{ 0, 1 }), point2(5., 5.));

	namespace bg = boost::geometry;
	typedef bg::model::polygon<bg::model::d2::point_xy<double> > polygon;
	polygon green, blue;

	std::for_each(geometry.rbegin(), geometry.rend(), [&green](const point2& p) -> void {green.outer().push_back(bg::model::d2::point_xy<double>(p[0], p[1])); });
	std::for_each(rotated.rbegin(), rotated.rend(), [&blue](const point2& p) -> void {blue.outer().push_back(bg::model::d2::point_xy<double>(p[0], p[1])); });
	
	std::deque<polygon> output;
	boost::geometry::intersection(green, blue, output);
	for (const polygon& p : output)
	{
		double a = bg::area(p);
		std::cout << a << std::endl;
	}
}

#include <geometrix/algorithm/doubly_connected_edge_list.hpp>
BOOST_AUTO_TEST_CASE(TestDoublyConnectedEdgeList)
{
	using namespace geometrix;
	typedef point_double_2d point2;
	typedef polygon<point2> polygon2;
	typedef segment<point2> segment2;
	absolute_tolerance_comparison_policy<double> cmp(1e-10);

	polygon2 geometry{ point2(0., 0.), point2(10., 0.), point2(15., 5.), point2(10., 10.), point2(0., 10.), point2(5., 5.) };

	std::vector<segment2> segs;
	for (std::size_t i = 0, j = 1; i < geometry.size(); ++i, j = (j+1)%geometry.size()) 
		segs.emplace_back(geometry[i], geometry[j]);

	polygon2 pgon{ point2{ 339.92894999997225, 281.53395000007004 }, point2{ 339.97594999993453, 281.58195000002161 }, point2{ 341.46094999997877, 281.81795000005513 }, point2{ 341.46894999995129, 281.81894999975339 }, point2{ 345.56894999998622, 281.81795000005513 }, point2{ 347.66594999993686, 281.87094999989495 }, point2{ 347.66694999998435, 281.87094999989495 }, point2{ 350.70294999994803, 281.8999499999918 }, point2{ 350.70394999993732, 281.8999499999918 }, point2{ 353.15094999998109, 281.8879499998875 }, point2{ 353.20994999993127, 281.79795000003651 }, point2{ 352.41594999993686, 279.95494999969378 }, point2{ 364.73494999995455, 280.02194999996573 }, point2{ 364.73794999998063, 280.02294999966398 }, point2{ 392.97994999994989, 280.21194999990985 }, point2{ 393.65094999998109, 280.21494999993593 }, point2{ 394.64894999994431, 280.21894999966025 }, point2{ 397.64594999997644, 280.23094999976456 }, point2{ 398.64494999992894, 280.2359499996528 }, point2{ 402.86294999998063, 280.24894999992102 }, point2{ 410.43594999995548, 280.29495000001043 }, point2{ 410.35294999997132, 282.50994999986142 }, point2{ 410.4119499999797, 282.57194999977946 }, point2{ 411.77594999998109, 282.58594999974594 }, point2{ 411.80594999995083, 282.58594999974594 }, point2{ 411.85694999992847, 282.58694999990985 }, point2{ 412.52094999997644, 282.59694999968633 }, point2{ 412.58094999997411, 282.53794999979436 }, point2{ 412.5819499999634, 281.48194999992847 }, point2{ 412.52394999994431, 281.42295000003651 }, point2{ 411.43494999996619, 281.41794999968261 }, point2{ 411.44094999996014, 280.30095000006258 }, point2{ 421.64094999997178, 280.33195000002161 }, point2{ 421.64394999993965, 280.33294999971986 }, point2{ 428.75094999995781, 280.38094999967143 }, point2{ 428.7519499999471, 280.38094999967143 }, point2{ 429.07994999998482, 285.34895000001416 }, point2{ 429.07894999993732, 285.34994999971241 }, point2{ 421.30394999997225, 285.40895000007004 }, point2{ 421.29894999996759, 285.4109499999322 }, point2{ 411.86094999994384, 285.36794999986887 }, point2{ 411.80194999993546, 285.42595000006258 }, point2{ 411.74494999996386, 296.25994999986142 }, point2{ 410.19894999993267, 296.2699499996379 }, point2{ 410.19794999994338, 296.26894999993965 }, point2{ 410.33294999995269, 283.07294999994338 }, point2{ 410.2749499999336, 283.0129499998875 }, point2{ 402.6609499999322, 282.96094999974594 }, point2{ 402.65794999996433, 282.95995000004768 }, point2{ 400.11894999997457, 282.9339499999769 }, point2{ 400.19894999993267, 282.68494999967515 }, point2{ 400.13994999998249, 282.60294999973848 }, point2{ 397.53094999998575, 282.55395000008866 }, point2{ 397.52394999994431, 282.55294999992475 }, point2{ 396.95794999995269, 282.41394999995828 }, point2{ 396.94694999995409, 282.41195000009611 }, point2{ 392.8769499999471, 282.3609499996528 }, point2{ 392.87594999995781, 282.3609499996528 }, point2{ 376.36894999997457, 282.44295000005513 }, point2{ 376.31094999995548, 282.49794999975711 }, point2{ 376.21994999994058, 284.36194999981672 }, point2{ 374.89594999997644, 284.3539499999024 }, point2{ 374.89494999992894, 284.35294999973848 }, point2{ 375.13594999996712, 282.49594999989495 }, point2{ 375.07694999995874, 282.42794999992475 }, point2{ 366.8659499999485, 282.38094999967143 }, point2{ 366.80794999992941, 282.43695000000298 }, point2{ 366.77594999998109, 283.23994999984279 }, point2{ 366.7749499999336, 283.24095000000671 }, point2{ 364.60094999993453, 283.29994999989867 }, point2{ 364.58994999993593, 283.29994999989867 }, point2{ 364.5479499999783, 283.30095000006258 }, point2{ 364.54594999994151, 283.29994999989867 }, point2{ 362.80994999996619, 283.30594999995083 }, point2{ 362.7519499999471, 283.36494999984279 }, point2{ 362.76694999996107, 288.98194999992847 }, point2{ 362.76594999997178, 288.98295000009239 }, point2{ 353.72494999994524, 288.90994999976829 }, point2{ 353.76494999998249, 283.44094999972731 }, point2{ 353.7069499999634, 283.38194999983534 }, point2{ 342.57694999995874, 283.27594999969006 }, point2{ 342.51794999995036, 283.33094999985769 }, point2{ 342.35194999998203, 286.62894999980927 }, point2{ 342.3769499999471, 286.67794999992475 }, point2{ 343.45994999993127, 287.33894999977201 }, point2{ 343.46094999997877, 287.33994999993593 }, point2{ 343.25794999994105, 287.52694999985397 }, point2{ 343.24594999995315, 287.59194999979809 }, point2{ 343.39894999994431, 287.90794999990612 }, point2{ 343.40094999998109, 287.91195000009611 }, point2{ 343.61094999994384, 288.25494999997318 }, point2{ 343.77994999993825, 288.83394999988377 }, point2{ 343.78094999998575, 288.83994999993593 }, point2{ 343.79494999995222, 289.59194999979809 }, point2{ 343.50694999995176, 290.11294999998063 }, point2{ 343.12494999996852, 290.63895000005141 }, point2{ 343.12094999995315, 290.64294999977574 }, point2{ 342.74594999995315, 290.93294999981299 }, point2{ 342.74494999996386, 290.93294999981299 }, point2{ 342.23094999993918, 291.23794999998063 }, point2{ 342.2069499999634, 291.30395000008866 }, point2{ 342.24994999996852, 291.43194999964908 }, point2{ 339.65994999994291, 291.35795000009239 }, point2{ 338.50694999995176, 290.12894999980927 }, point2{ 338.50694999995176, 290.12794999964535 }, point2{ 338.5819499999634, 287.78794999979436 }, point2{ 339.53894999995828, 286.7699499996379 }, point2{ 340.48694999993313, 286.5589499999769 }, point2{ 340.53094999998575, 286.50794999999925 }, point2{ 340.72494999994524, 283.27194999996573 }, point2{ 340.66894999996293, 283.20995000004768 }, point2{ 340.19794999994338, 283.19194999989122 }, point2{ 340.19894999993267, 283.16594999982044 }, point2{ 340.15194999997038, 283.10594999976456 }, point2{ 338.68494999996619, 282.85195000004023 }, point2{ 338.6379499999457, 282.86694999970496 }, point2{ 335.38494999997783, 286.20094999996945 }, point2{ 335.38394999993034, 286.2019499996677 }, point2{ 334.64794999995502, 286.99594999989495 }, point2{ 334.63394999993034, 287.03294999990612 }, point2{ 334.92094999994151, 293.84094999963418 }, point2{ 335.81694999994943, 315.72094999998808 }, point2{ 335.81694999994943, 315.72294999985024 }, point2{ 336.46094999997877, 324.49894999992102 }, point2{ 336.46194999996806, 324.50294999964535 }, point2{ 336.76494999998249, 326.57994999969378 }, point2{ 336.81894999998622, 326.62894999980927 }, point2{ 339.03994999994757, 326.71094999974594 }, point2{ 339.04294999997364, 326.71094999974594 }, point2{ 342.38694999995641, 327.19894999964163 }, point2{ 342.45594999997411, 327.1379499998875 }, point2{ 342.4459499999648, 325.69394999975339 }, point2{ 342.44694999995409, 325.69295000005513 }, point2{ 343.72094999992987, 325.71594999963418 }, point2{ 343.71994999994058, 325.71594999963418 }, point2{ 343.73594999994384, 327.32594999996945 }, point2{ 343.75094999995781, 331.63595000002533 }, point2{ 343.74994999996852, 331.63694999972358 }, point2{ 337.48894999996992, 331.52694999985397 }, point2{ 337.47894999996061, 331.52795000001788 }, point2{ 335.55494999996154, 331.90994999976829 }, point2{ 334.37594999995781, 322.70294999983162 }, point2{ 334.37494999996852, 322.70094999996945 }, point2{ 334.31694999994943, 321.85294999973848 }, point2{ 334.31694999994943, 321.84794999985024 }, point2{ 333.09094999998342, 288.23494999995455 }, point2{ 333.04494999995222, 286.39394999993965 }, point2{ 333.04194999998435, 286.38494999986142 }, point2{ 333.04294999997364, 286.38194999983534 }, point2{ 333.04394999996293, 286.36794999986887 }, point2{ 333.0469499999308, 286.36294999998063 }, point2{ 333.28094999998575, 285.39195000007749 }, point2{ 333.52994999993825, 284.7699499996379 }, point2{ 334.16394999995828, 283.68494999967515 }, point2{ 334.81094999995548, 282.85594999976456 }, point2{ 334.81194999994477, 282.85495000006631 }, point2{ 335.1839499999769, 282.35094999987632 }, point2{ 335.92694999993546, 281.5469499998726 }, point2{ 337.04094999993686, 280.81495000002906 }, point2{ 338.05594999995083, 280.33795000007376 }, point2{ 338.0589499999769, 280.33594999974594 }, point2{ 338.06294999993406, 280.33394999988377 }, point2{ 338.06994999997551, 280.33195000002161 }, point2{ 338.0819499999634, 280.32594999996945 }, point2{ 338.08694999996806, 280.3269499996677 }, point2{ 338.8319499999634, 280.08894999977201 }, point2{ 339.71494999993593, 279.94194999989122 } };
	for (std::size_t i = 0, j = 1; i < pgon.size(); ++i, j = (j + 1) % pgon.size())
		segs.emplace_back(pgon[i], pgon[j]);
	
	auto dcel = make_dcel<point2>(segs, cmp);

	//! NOTE: These are only equal because the are input in the proper order.
	BOOST_REQUIRE(dcel.get_polygons().size() == 2);
	BOOST_CHECK(point_sequences_equal(dcel.get_polygons()[0], geometry, cmp));
	BOOST_CHECK(point_sequences_equal(dcel.get_polygons()[1], pgon, cmp));
}

BOOST_AUTO_TEST_CASE(TestRandomlyInputDoublyConnectedEdgeList)
{
	using namespace geometrix;
	typedef point_double_2d point2;
	typedef polygon<point2> polygon2;
	typedef polyline<point2> polyline2;
	typedef segment<point2> segment2;
	absolute_tolerance_comparison_policy<double> cmp(1e-10);

	polygon2 geometry{ point2(0., 0.), point2(10., 0.), point2(15., 5.), point2(10., 10.), point2(0., 10.), point2(5., 5.) };

	std::vector<std::pair<std::size_t, std::size_t>> edges;
	std::vector<segment2> segs;
	for (std::size_t i = 0, j = 1; i < geometry.size(); ++i, j = (j + 1) % geometry.size())
		edges.emplace_back(i, j);

	std::random_shuffle(edges.begin(), edges.end());
	for (auto e : edges)
		segs.emplace_back(geometry[e.first], geometry[e.second]);

	polygon2 pgon{ point2{ 339.92894999997225, 281.53395000007004 }, point2{ 339.97594999993453, 281.58195000002161 }, point2{ 341.46094999997877, 281.81795000005513 }, point2{ 341.46894999995129, 281.81894999975339 }, point2{ 345.56894999998622, 281.81795000005513 }, point2{ 347.66594999993686, 281.87094999989495 }, point2{ 347.66694999998435, 281.87094999989495 }, point2{ 350.70294999994803, 281.8999499999918 }, point2{ 350.70394999993732, 281.8999499999918 }, point2{ 353.15094999998109, 281.8879499998875 }, point2{ 353.20994999993127, 281.79795000003651 }, point2{ 352.41594999993686, 279.95494999969378 }, point2{ 364.73494999995455, 280.02194999996573 }, point2{ 364.73794999998063, 280.02294999966398 }, point2{ 392.97994999994989, 280.21194999990985 }, point2{ 393.65094999998109, 280.21494999993593 }, point2{ 394.64894999994431, 280.21894999966025 }, point2{ 397.64594999997644, 280.23094999976456 }, point2{ 398.64494999992894, 280.2359499996528 }, point2{ 402.86294999998063, 280.24894999992102 }, point2{ 410.43594999995548, 280.29495000001043 }, point2{ 410.35294999997132, 282.50994999986142 }, point2{ 410.4119499999797, 282.57194999977946 }, point2{ 411.77594999998109, 282.58594999974594 }, point2{ 411.80594999995083, 282.58594999974594 }, point2{ 411.85694999992847, 282.58694999990985 }, point2{ 412.52094999997644, 282.59694999968633 }, point2{ 412.58094999997411, 282.53794999979436 }, point2{ 412.5819499999634, 281.48194999992847 }, point2{ 412.52394999994431, 281.42295000003651 }, point2{ 411.43494999996619, 281.41794999968261 }, point2{ 411.44094999996014, 280.30095000006258 }, point2{ 421.64094999997178, 280.33195000002161 }, point2{ 421.64394999993965, 280.33294999971986 }, point2{ 428.75094999995781, 280.38094999967143 }, point2{ 428.7519499999471, 280.38094999967143 }, point2{ 429.07994999998482, 285.34895000001416 }, point2{ 429.07894999993732, 285.34994999971241 }, point2{ 421.30394999997225, 285.40895000007004 }, point2{ 421.29894999996759, 285.4109499999322 }, point2{ 411.86094999994384, 285.36794999986887 }, point2{ 411.80194999993546, 285.42595000006258 }, point2{ 411.74494999996386, 296.25994999986142 }, point2{ 410.19894999993267, 296.2699499996379 }, point2{ 410.19794999994338, 296.26894999993965 }, point2{ 410.33294999995269, 283.07294999994338 }, point2{ 410.2749499999336, 283.0129499998875 }, point2{ 402.6609499999322, 282.96094999974594 }, point2{ 402.65794999996433, 282.95995000004768 }, point2{ 400.11894999997457, 282.9339499999769 }, point2{ 400.19894999993267, 282.68494999967515 }, point2{ 400.13994999998249, 282.60294999973848 }, point2{ 397.53094999998575, 282.55395000008866 }, point2{ 397.52394999994431, 282.55294999992475 }, point2{ 396.95794999995269, 282.41394999995828 }, point2{ 396.94694999995409, 282.41195000009611 }, point2{ 392.8769499999471, 282.3609499996528 }, point2{ 392.87594999995781, 282.3609499996528 }, point2{ 376.36894999997457, 282.44295000005513 }, point2{ 376.31094999995548, 282.49794999975711 }, point2{ 376.21994999994058, 284.36194999981672 }, point2{ 374.89594999997644, 284.3539499999024 }, point2{ 374.89494999992894, 284.35294999973848 }, point2{ 375.13594999996712, 282.49594999989495 }, point2{ 375.07694999995874, 282.42794999992475 }, point2{ 366.8659499999485, 282.38094999967143 }, point2{ 366.80794999992941, 282.43695000000298 }, point2{ 366.77594999998109, 283.23994999984279 }, point2{ 366.7749499999336, 283.24095000000671 }, point2{ 364.60094999993453, 283.29994999989867 }, point2{ 364.58994999993593, 283.29994999989867 }, point2{ 364.5479499999783, 283.30095000006258 }, point2{ 364.54594999994151, 283.29994999989867 }, point2{ 362.80994999996619, 283.30594999995083 }, point2{ 362.7519499999471, 283.36494999984279 }, point2{ 362.76694999996107, 288.98194999992847 }, point2{ 362.76594999997178, 288.98295000009239 }, point2{ 353.72494999994524, 288.90994999976829 }, point2{ 353.76494999998249, 283.44094999972731 }, point2{ 353.7069499999634, 283.38194999983534 }, point2{ 342.57694999995874, 283.27594999969006 }, point2{ 342.51794999995036, 283.33094999985769 }, point2{ 342.35194999998203, 286.62894999980927 }, point2{ 342.3769499999471, 286.67794999992475 }, point2{ 343.45994999993127, 287.33894999977201 }, point2{ 343.46094999997877, 287.33994999993593 }, point2{ 343.25794999994105, 287.52694999985397 }, point2{ 343.24594999995315, 287.59194999979809 }, point2{ 343.39894999994431, 287.90794999990612 }, point2{ 343.40094999998109, 287.91195000009611 }, point2{ 343.61094999994384, 288.25494999997318 }, point2{ 343.77994999993825, 288.83394999988377 }, point2{ 343.78094999998575, 288.83994999993593 }, point2{ 343.79494999995222, 289.59194999979809 }, point2{ 343.50694999995176, 290.11294999998063 }, point2{ 343.12494999996852, 290.63895000005141 }, point2{ 343.12094999995315, 290.64294999977574 }, point2{ 342.74594999995315, 290.93294999981299 }, point2{ 342.74494999996386, 290.93294999981299 }, point2{ 342.23094999993918, 291.23794999998063 }, point2{ 342.2069499999634, 291.30395000008866 }, point2{ 342.24994999996852, 291.43194999964908 }, point2{ 339.65994999994291, 291.35795000009239 }, point2{ 338.50694999995176, 290.12894999980927 }, point2{ 338.50694999995176, 290.12794999964535 }, point2{ 338.5819499999634, 287.78794999979436 }, point2{ 339.53894999995828, 286.7699499996379 }, point2{ 340.48694999993313, 286.5589499999769 }, point2{ 340.53094999998575, 286.50794999999925 }, point2{ 340.72494999994524, 283.27194999996573 }, point2{ 340.66894999996293, 283.20995000004768 }, point2{ 340.19794999994338, 283.19194999989122 }, point2{ 340.19894999993267, 283.16594999982044 }, point2{ 340.15194999997038, 283.10594999976456 }, point2{ 338.68494999996619, 282.85195000004023 }, point2{ 338.6379499999457, 282.86694999970496 }, point2{ 335.38494999997783, 286.20094999996945 }, point2{ 335.38394999993034, 286.2019499996677 }, point2{ 334.64794999995502, 286.99594999989495 }, point2{ 334.63394999993034, 287.03294999990612 }, point2{ 334.92094999994151, 293.84094999963418 }, point2{ 335.81694999994943, 315.72094999998808 }, point2{ 335.81694999994943, 315.72294999985024 }, point2{ 336.46094999997877, 324.49894999992102 }, point2{ 336.46194999996806, 324.50294999964535 }, point2{ 336.76494999998249, 326.57994999969378 }, point2{ 336.81894999998622, 326.62894999980927 }, point2{ 339.03994999994757, 326.71094999974594 }, point2{ 339.04294999997364, 326.71094999974594 }, point2{ 342.38694999995641, 327.19894999964163 }, point2{ 342.45594999997411, 327.1379499998875 }, point2{ 342.4459499999648, 325.69394999975339 }, point2{ 342.44694999995409, 325.69295000005513 }, point2{ 343.72094999992987, 325.71594999963418 }, point2{ 343.71994999994058, 325.71594999963418 }, point2{ 343.73594999994384, 327.32594999996945 }, point2{ 343.75094999995781, 331.63595000002533 }, point2{ 343.74994999996852, 331.63694999972358 }, point2{ 337.48894999996992, 331.52694999985397 }, point2{ 337.47894999996061, 331.52795000001788 }, point2{ 335.55494999996154, 331.90994999976829 }, point2{ 334.37594999995781, 322.70294999983162 }, point2{ 334.37494999996852, 322.70094999996945 }, point2{ 334.31694999994943, 321.85294999973848 }, point2{ 334.31694999994943, 321.84794999985024 }, point2{ 333.09094999998342, 288.23494999995455 }, point2{ 333.04494999995222, 286.39394999993965 }, point2{ 333.04194999998435, 286.38494999986142 }, point2{ 333.04294999997364, 286.38194999983534 }, point2{ 333.04394999996293, 286.36794999986887 }, point2{ 333.0469499999308, 286.36294999998063 }, point2{ 333.28094999998575, 285.39195000007749 }, point2{ 333.52994999993825, 284.7699499996379 }, point2{ 334.16394999995828, 283.68494999967515 }, point2{ 334.81094999995548, 282.85594999976456 }, point2{ 334.81194999994477, 282.85495000006631 }, point2{ 335.1839499999769, 282.35094999987632 }, point2{ 335.92694999993546, 281.5469499998726 }, point2{ 337.04094999993686, 280.81495000002906 }, point2{ 338.05594999995083, 280.33795000007376 }, point2{ 338.0589499999769, 280.33594999974594 }, point2{ 338.06294999993406, 280.33394999988377 }, point2{ 338.06994999997551, 280.33195000002161 }, point2{ 338.0819499999634, 280.32594999996945 }, point2{ 338.08694999996806, 280.3269499996677 }, point2{ 338.8319499999634, 280.08894999977201 }, point2{ 339.71494999993593, 279.94194999989122 } };
	
	edges.clear();
	for (std::size_t i = 0, j = 1; i < pgon.size(); ++i, j = (j + 1) % pgon.size())
		edges.emplace_back(i, j);

	std::random_shuffle(edges.begin(), edges.end());
	for (auto e : edges)
		segs.emplace_back(pgon[e.first], pgon[e.second]);

	polyline2 geometry3{ point2{ 81.468972358786075, 309.98166757887907 }, point2{ 81.773983506490453, 309.96489838253967 } };
	edges.clear();
	segs.emplace_back(geometry3[0], geometry3[1]);

	auto dcel = make_dcel<point2>(segs, cmp);

	const auto& faces = dcel.get_polygons();
	const auto& polylines = dcel.get_polylines();
	BOOST_REQUIRE(faces.size() == 2);
	BOOST_REQUIRE(polylines.size() == 1);

	std::size_t offset = polygon_polygon_offset(geometry, faces[0], cmp);
	BOOST_REQUIRE(offset != (std::numeric_limits<std::size_t>::max)());
	BOOST_CHECK(polygons_equal(geometry, faces[0], offset, cmp));
	
	offset = polygon_polygon_offset(pgon, faces[1], cmp);
	BOOST_REQUIRE(offset != (std::numeric_limits<std::size_t>::max)());
	BOOST_CHECK(polygons_equal(pgon, faces[1], offset, cmp));

	BOOST_CHECK(point_sequences_equal(geometry3, polylines[0], cmp));
}

#include <geometrix/algorithm/point_sequence/find_subsequence.hpp>

BOOST_FIXTURE_TEST_CASE(find_containing_polygon_border_segment_tests, geometry_kernel_2d_fixture)
{
	using namespace geometrix;

	polygon2 P{ point2(0., 0.), point2(10., 0.), point2(15., 5.), point2(10., 10.), point2(0., 10.), point2(5., 5.) };	

	{
		point2 p = { 2.5, 2.5 };
		auto result = find_containing_polygon_border_segment(P, p, cmp);
		BOOST_CHECK(result && *result == 5);
	}

	{
		point2 p = { 0.0, 0.0 };
		auto result = find_containing_polygon_border_segment(P, p, cmp);
		BOOST_CHECK(result && *result == 0);
	}

	{
		point2 p = { 10.0, 0.0 };
		auto result = find_containing_polygon_border_segment(P, p, cmp);
		BOOST_CHECK(result && *result == 0);
	}

	{
		point2 p = { 10.0, 1.0 };
		auto result = find_containing_polygon_border_segment(P, p, cmp);
		BOOST_CHECK(!result);
	}
}

BOOST_FIXTURE_TEST_CASE(polygon_subsequence_tests, geometry_kernel_2d_fixture)
{
	using namespace geometrix;
	using geometrix::detail::polygon_subsequence_half;

	std::vector<point2> pvec{ point2(0., 0.), point2(10., 0.), point2(15., 5.), point2(10., 10.), point2(0., 10.), point2(5., 5.) };

	polygon2 pgon(pvec);//! should compile.
	polygon2 pgon2 = pvec;//! should compile.

	polyline2 pline(pvec);

	//! Should not compile.
	//polygon2 pgon3(pline);
	//polygon2 pgon3 = pline;

	pgon = pvec;
	pgon = pgon2;
	pgon = { point2(0., 0.), point2(10., 0.), point2(15., 5.), point2(10., 10.), point2(0., 10.), point2(5., 5.) };
	pgon = std::move(pgon2);
	BOOST_CHECK(pgon2.empty());
	//pgon = pline;

	polygon2 P{ point2(0., 0.), point2(10., 0.), point2(15., 5.), point2(10., 10.), point2(0., 10.), point2(5., 5.) };

	{
		point2 p = { 2.5, 2.5 };
		auto result = polygon_subsequence_half<polygon_winding::counterclockwise>(P, p, 1.0, 5);
		BOOST_CHECK_CLOSE(polyline_length(result), 1.0, 1e-10);
		BOOST_CHECK(point_sequences_equal(result, polyline2{ p, {1.7928932188134525, 1.7928932188134525} }, cmp));
	}
	
	{
		point2 p = { 2.5, 2.5 };
		auto result = polygon_subsequence_half<polygon_winding::clockwise>(P, p, 1.0, 0);
		BOOST_CHECK_CLOSE(polyline_length(result), 1.0, 1e-10);
		BOOST_CHECK(point_sequences_equal(result, polyline2{ p, {3.2071067811865475, 3.2071067811865475} }, cmp));
	}

	{
		point2 p = { 2.5, 2.5 };
		auto result = polygon_subsequence(P, p, 1.0, cmp);
		BOOST_CHECK_CLOSE(polyline_length(result), 2.0, 1e-10);
		BOOST_CHECK(point_sequences_equal(result, polyline2{ { 3.2071067811865475, 3.2071067811865475 }, { 1.7928932188134525, 1.7928932188134525 } }, cmp));
	}

	{
		point2 p = { 2.5, 2.5 };
		auto result = polygon_subsequence_half<polygon_winding::counterclockwise>(P, p, 15.0, 5);
		BOOST_CHECK(point_sequences_equal(result, polyline2{ p, {0, 0}, {10, 0}, {11.035533905932738, 1.0355339059327375} }, cmp));
		BOOST_CHECK_CLOSE(polyline_length(result), 15.0, 1e-10);
	}

	{
		point2 p = { 2.5, 2.5 };
		auto result = polygon_subsequence_half<polygon_winding::clockwise>(P, p, 15.0, 0);
		BOOST_CHECK(point_sequences_equal(result, polyline2{ p,{ 5, 5 },{ 0, 10 }, {4.3933982822017867, 10.000000000000000} }, cmp));
		BOOST_CHECK_CLOSE(polyline_length(result), 15.0, 1e-10);
	}

	{
		point2 p = { 2.5, 2.5 };
		auto result = polygon_subsequence(P, p, 15.0, cmp);
		BOOST_CHECK_CLOSE(polyline_length(result), 30.0, 1e-10);
		BOOST_CHECK(point_sequences_equal(result, polyline2{ { 4.3933982822017867, 10.000000000000000 }, { 0, 10 }, { 5, 5 }, { 0, 0 }, { 10, 0 }, { 11.035533905932738, 1.0355339059327375 } }, cmp));
	}

}

#include <geometrix/algorithm/point_sequence/self_intersection.hpp>
BOOST_FIXTURE_TEST_CASE(polyline_self_intersection_tests, geometry_kernel_2d_fixture)
{
	using namespace geometrix;
	
	//! No intersection
	{
		polyline2 center{ point2{ 20, 0 }, point2{ 10, 0 }, point2{ 5, 5 }, point2{ 5, 6 }, point2{ 0, 5 } };
		auto visitor = [](std::size_t i, std::size_t j, intersection_type iType, const point2& xp1, const point2& xp2) { BOOST_CHECK(false); };
		BOOST_CHECK(!polyline_self_intersection(center, visitor, cmp));
	}

	{
		polyline2 center{ point2{ 20, 0 }, point2{ 10, 0 }, point2{20,0}, point2{ 5, 5 }, point2{ 5, 6 }, point2{ 0, 5 } };
		int count = 0;
		auto visitor = [&count](std::size_t i, std::size_t j, intersection_type iType, const point2& xp1, const point2& xp2) { ++count; };
		BOOST_CHECK(polyline_self_intersection(center, visitor, cmp));
		BOOST_CHECK(count == 2);
	}

	{
		polyline2 center{ point2{ 20, 0 }, point2{ 10, 0 }, point2{ 5, 5 }, point2{ 5, 6 }, point2{ 20, -20 } };
		int count = 0;
		auto visitor = [&count](std::size_t i, std::size_t j, intersection_type iType, const point2& xp1, const point2& xp2) { ++count; };
		BOOST_CHECK(polyline_self_intersection(center, visitor, cmp));
		BOOST_CHECK(count == 1);
	}
}

#include <geometrix/algorithm/point_sequence/polyline_offset.hpp>
#include <iostream>
BOOST_FIXTURE_TEST_CASE(polyline_offset_tests, geometry_kernel_2d_fixture)
{
	using namespace geometrix;
	{
		polyline2 center{ point2{ 20, 0 }, point2{ 10, 0 }, point2{ 5, 5 }, point2{ 5, 6 }, point2{ 0, 5 } };
		
		double offset = 3.7 / 2.0;
		polyline2 right = polyline_offset(center, oriented_right, offset, cmp);
		BOOST_TEST_MESSAGE("right: " << right );
		BOOST_CHECK(point_sequences_equal(right, polyline2{ point2{ 20, 1.8500000000000001 }, point2{ 10.766295090390225, 1.8500000000000001 }, point2{ 6.8499999999999996, 5.7662950903902255 }, point2{ 6.8499999999999996, 8.2566372200293294 }, point2{ -0.36281485000564051, 6.8140742500282023 } }, cmp));
		polyline2 left = polyline_offset(center, oriented_left, offset, cmp);
		BOOST_TEST_MESSAGE("left: " << left);
		BOOST_CHECK(point_sequences_equal(left, polyline2{ point2{ 20, -1.8500000000000001 }, point2{ 9.2337049096097754, -1.8500000000000001 }, point2{ 3.1499999999999999, 4.2337049096097736 }, point2{ 3.1499999999999999, 3.7433627799706697 }, point2{ 0.36281485000564051, 3.1859257499717977 } }, cmp));
	}
	{
		polyline2 center{ point2{ 20, 0 }, point2{ 10, 0 }, point2{ 5, 5 }, point2{ 5, 6 }, point2{ 0, 5 } };

		double offset = 3.7 / 2.0;
		auto results = liu_polyline_offset(center, oriented_right, offset, cmp);
		polyline2 right = results[0];
		BOOST_TEST_MESSAGE("right: " << right);
		BOOST_CHECK(point_sequences_equal(right, polyline2{ point2{ 20, 1.8500000000000001 }, point2{ 10.766295090390225, 1.8500000000000001 }, point2{ 6.8499999999999996, 5.7662950903902255 }, point2{ 6.8499999999999996, 8.2566372200293294 }, point2{ -0.36281485000564051, 6.8140742500282023 } }, cmp));
		auto results2 = liu_polyline_offset(center, oriented_left, offset, cmp);
		BOOST_CHECK(results2.size() == 2);
		//polyline2 left = results2[0];
		//BOOST_TEST_MESSAGE("left: " << left);
		//BOOST_CHECK(point_sequences_equal(left, polyline2{ point2{ 20, -1.8500000000000001 }, point2{ 9.2337049096097754, -1.8500000000000001 }, point2{ 3.1499999999999999, 4.2337049096097736 }, point2{ 3.1499999999999999, 3.7433627799706697 }, point2{ 0.36281485000564051, 3.1859257499717977 } }, cmp));
	}
}

BOOST_FIXTURE_TEST_CASE(polyline_split_tests, geometry_kernel_2d_fixture)
{
	using namespace geometrix;
	using namespace geometrix::detail;

	{
		polyline2 pline = { { 0,0 },{ 1,1 },{ 2,1 },{ 3,0 } };
		intersection_set<polyline2> iset(pline);
		iset.insert(std::make_tuple(0, point2{ 0, 0 }));

		auto results = split(iset, pline, cmp);
		BOOST_REQUIRE(results.size() == 1);
		BOOST_CHECK(point_sequences_equal(results[0], polyline2{ { 0,0 },{ 1,1 },{ 2,1 },{ 3,0 } }, cmp));
	}
	
	{
		polyline2 pline = { { 0,0 },{ 1,1 },{ 2,1 },{ 3,0 } };
		intersection_set<polyline2> iset(pline);
		iset.insert(std::make_tuple(1, point2{ 2, 1 }));

		auto results = split(iset, pline, cmp);
		BOOST_REQUIRE(results.size() == 2);
		BOOST_CHECK(point_sequences_equal(results[0], polyline2{ { 0,0 },{ 1,1 }, {2, 1} }, cmp));
		BOOST_CHECK(point_sequences_equal(results[1], polyline2{ { 2,1 },{ 3,0 } }, cmp));
	}

	{
		polyline2 pline = { { 0,0 },{ 1,1 },{ 2,1 },{ 3,0 } };
		intersection_set<polyline2> iset(pline);
		iset.insert(std::make_tuple(2, point2{ 2, 1 }));

		auto results = split(iset, pline, cmp);
		BOOST_REQUIRE(results.size() == 2);
		BOOST_CHECK(point_sequences_equal(results[0], polyline2{ { 0,0 },{ 1,1 },{ 2, 1 } }, cmp));
		BOOST_CHECK(point_sequences_equal(results[1], polyline2{ { 2,1 },{ 3,0 } }, cmp));
	}

	{
		polyline2 pline = { { 0,0 },{ 1,1 },{ 2,1 },{ 3,0 } };
		intersection_set<polyline2> iset(pline);
		iset.insert(std::make_tuple(0, point2{ 1, 1 }));

		auto results = split(iset, pline, cmp);
		BOOST_REQUIRE(results.size() == 2);
		BOOST_CHECK(point_sequences_equal(results[0], polyline2{ { 0,0 },{ 1,1 } }, cmp));
		BOOST_CHECK(point_sequences_equal(results[1], polyline2{ { 1,1 },{ 2,1 },{ 3,0 } }, cmp));
	}

	{
		polyline2 pline = { { 0,0 },{ 1,1 },{ 2,1 },{ 3,0 } };
		intersection_set<polyline2> iset(pline);
		iset.insert(std::make_tuple(1, point2{ 1, 1 }));

		auto results = split(iset, pline, cmp);
		BOOST_REQUIRE(results.size() == 2);
		BOOST_CHECK(point_sequences_equal(results[0], polyline2{ { 0,0 },{ 1,1 } }, cmp));
		BOOST_CHECK(point_sequences_equal(results[1], polyline2{ { 1,1 },{ 2,1 },{ 3,0 } }, cmp));
	}

	{
		polyline2 pline = { { 0,0 },{ 1,1 },{ 2,1 },{ 3,0 } };
		intersection_set<polyline2> iset(pline);
		iset.insert(std::make_tuple(2, point2{ 3, 0 }));

		auto results = split(iset, pline, cmp);
		BOOST_REQUIRE(results.size() == 1);
		BOOST_CHECK(point_sequences_equal(results[0], polyline2{ { 0,0 },{ 1,1 },{ 2,1 },{ 3,0 } }, cmp));
	}

	{
		polyline2 pline = { {0,0}, {1,1}, {2,1}, {3,0} };
		intersection_set<polyline2> iset(pline);
		iset.insert(std::make_tuple(0, point2{ 0.5, 0.5 }));

		auto results = split(iset, pline, cmp);
		BOOST_REQUIRE(results.size() == 2);
		BOOST_CHECK(point_sequences_equal(results[0], polyline2{ {0,0}, {.5,.5} }, cmp));
		BOOST_CHECK(point_sequences_equal(results[1], polyline2{ { .5,.5 }, {1,1}, {2,1}, {3,0} }, cmp));
	}

	{
		polyline2 pline = { { 0,0 },{ 1,1 },{ 2,1 },{ 3,0 } };
		intersection_set<polyline2> iset(pline);
		iset.insert(std::make_tuple(0, point2{ 0.5, 0.5 }));
		iset.insert(std::make_tuple(0, point2{ 0.6, 0.6 }));
		auto results = split(iset, pline, cmp);
		BOOST_REQUIRE(results.size() == 3);
		BOOST_CHECK(point_sequences_equal(results[0], polyline2{ { 0,0 },{ .5,.5 } }, cmp));
		BOOST_CHECK(point_sequences_equal(results[1], polyline2{ { 0.5, 0.5 },{ .6,.6 } }, cmp));
		BOOST_CHECK(point_sequences_equal(results[2], polyline2{ { .6,.6 },{ 1,1 },{ 2,1 },{ 3,0 } }, cmp));
	}

	{
		polyline2 pline = { { 0,0 },{ 1,1 },{ 2,1 },{ 3,0 } };
		intersection_set<polyline2> iset(pline);
		iset.insert(std::make_tuple(0, point2{ 0.5, 0.5 }));
		iset.insert(std::make_tuple(0, point2{ 0.6, 0.6 }));
		iset.insert(std::make_tuple(1, point2{ 1.5, 1.0 }));
		auto results = split(iset, pline, cmp);
		BOOST_REQUIRE(results.size() == 4);
		BOOST_CHECK(point_sequences_equal(results[0], polyline2{ { 0,0 },{ .5,.5 } }, cmp));
		BOOST_CHECK(point_sequences_equal(results[1], polyline2{ { 0.5, 0.5 },{ .6,.6 } }, cmp));
		BOOST_CHECK(point_sequences_equal(results[2], polyline2{ { .6,.6 },{ 1,1 },{ 1.5,1 } }, cmp));
		BOOST_CHECK(point_sequences_equal(results[3], polyline2{ { 1.5, 1 },{ 2,1 },{ 3,0 } }, cmp));
	}

	{
		polyline2 pline = { { 0,0 },{ 1,1 },{ 2,1 },{ 3,0 } };
		intersection_set<polyline2> iset(pline);
		iset.insert(std::make_tuple(0, point2{ 0.5, 0.5 }));
		iset.insert(std::make_tuple(0, point2{ 0.6, 0.6 }));
		iset.insert(std::make_tuple(1, point2{ 1.5, 1.0 }));
		iset.insert(std::make_tuple(1, point2{ 1.7, 1.0 }));
		auto results = split(iset, pline, cmp);
		BOOST_REQUIRE(results.size() == 5);
		BOOST_CHECK(point_sequences_equal(results[0], polyline2{ { 0,0 },{ .5,.5 } }, cmp));
		BOOST_CHECK(point_sequences_equal(results[1], polyline2{ { 0.5, 0.5 },{ .6,.6 } }, cmp));
		BOOST_CHECK(point_sequences_equal(results[2], polyline2{ { .6,.6 },{ 1,1 },{ 1.5,1 } }, cmp));
		BOOST_CHECK(point_sequences_equal(results[3], polyline2{ { 1.5, 1 },{ 1.7,1 } }, cmp));
		BOOST_CHECK(point_sequences_equal(results[4], polyline2{ { 1.7, 1 },{ 2,1 },{ 3,0 } }, cmp));
	}
}

BOOST_FIXTURE_TEST_CASE(polyline_characterize_test, geometry_kernel_2d_fixture)
{
	using namespace geometrix;

	point2 p{ -485.00745619647205, 167.08107061358169 };
	polyline2 pline{ point2{ -484.95360673742834, 139.28299748897552 }, point2{ -484.95808840339305, 141.19283876754344 }, point2{ -484.96282397024333, 143.22407058859244 }, point2{ -484.96772876312025, 145.35396864823997 }, point2{ -484.97271810699021, 147.55980863701552 }, point2{ -484.97770732670324, 149.81886625150219 }, point2{ -484.98261174780782, 152.10841718502343 }, point2{ -484.98734669486294, 154.40573713136837 }, point2{ -484.99182749318425, 156.68810178618878 }, point2{ -484.99596946779639, 158.93278684094548 }, point2{ -484.99968794372398, 161.11706799035892 }, point2{ -485.00289824587526, 163.21822093101218 }, point2{ -485.00551569933305, 165.21352135483176 }, point2{ -485.00745562964585, 167.08024495467544 }, point2{ -485.00863336131442, 168.79566742572933 }, point2{ -485.00896421936341, 170.33706446364522 }, point2{ -485.00836352899205, 171.68171176034957 }, point2{ -485.00674661557423, 172.80688501009718 }, point2{ -485.00402880378533, 173.68985990760848 }, point2{ -485.00012541870819, 174.30791214806959 } };

	distance_from_start_to_point<double> prefix;
	distance_from_point_to_end<double> suffix;
	polyline_segment_index index;
	auto cmp2 = absolute_tolerance_comparison_policy<double>(1e-6);//! Need a more coarse tolerance for this due to large offsets in the translations.
	auto cmp3 = absolute_tolerance_comparison_policy<double>(1e-2);//! Need a more coarse tolerance for this due to large offsets in the translations.

	std::tie(prefix, suffix, index) = point_on_polyline_length_characteristics(pline, p, cmp2);
	BOOST_CHECK(index != (std::numeric_limits<std::size_t>::max)());
	auto length = polyline_length(pline);
	auto clength = (prefix + suffix).value();
	BOOST_CHECK(cmp3.equals(clength, length));
}

BOOST_FIXTURE_TEST_CASE(polygon_with_holes_test, geometry_kernel_2d_fixture)
{
	using namespace geometrix;
	{
		polygon2 outer = { { 0,0 },{ 1,1 },{ 2,1 },{ 3,0 } };
		polygon2 hole1 = { { 0,0 },{ 1,1 },{ 2,1 },{ 3,1 } };
		polygon2 hole2 = { { 0,0 },{ 1,1 },{ 2,1 },{ 3,1 } };
		polygon2 hole3 = { { 0,0 },{ 1,1 },{ 2,1 },{ 3,1 } };
		polygon_with_holes2 pgons(std::move(outer), std::vector<polygon2>{ std::move(hole1), std::move(hole2), std::move(hole3) });

		BOOST_CHECK(pgons.get_outer().size() == 4);
		BOOST_CHECK(pgons.get_holes().size() == 3);

		BOOST_CHECK(outer.empty());
		BOOST_CHECK(hole1.empty());
		BOOST_CHECK(hole2.empty());
		BOOST_CHECK(hole3.empty());
	}

	{		
		polygon_with_holes2 pgons{ polygon2{ { 0,0 },{ 1,1 },{ 2,1 },{ 3,0 } } };

		BOOST_CHECK(pgons.get_outer().size() == 4);
		BOOST_CHECK(pgons.get_holes().size() == 0);
	}

	{
		polygon2 pgon = { { 10, 5.25 },{ 10, 10 },{ 0, 10 },{ 0, 0 },{ 10, 0 },{ 10, 4.75 },{ 1, 4.75 },{ 1, 5.25 } };
		polygon2 hole = { { 0.5, 0.5 },{ 0.5, 1 },{ 1, 1 },{ 1, 0.5 } };
		polygon_with_holes2 geometry{ pgon, std::vector<polygon2>{hole} };
	}
}

#include <geometrix/algorithm/point_sequence/ramer_douglas_peucker_algorithm.hpp>
BOOST_FIXTURE_TEST_CASE(polyline_simplify_test, geometry_kernel_2d_fixture)
{
	using namespace geometrix;
	auto pline = polyline2{ { 1098.47527107,1178.48809441 },{ 1071.39171392,1185.84823745 },{ 1059.99795823,1189.00357638 },{ 1049.91310331,1191.87260701 },{ 1041.04481327,1194.50296442 },{ 1033.30075223,1196.94228366 },{ 1026.58858431,1199.2381998 },{ 1020.81597365,1201.43834793 },{ 1015.89058435,1203.59036309 },{ 1011.72008055,1205.74188038 },{ 1008.21212637,1207.94053484 },{ 1005.27438592,1210.23396156 },{ 1002.81452334,1212.6697956 },{ 1000.74020273,1215.29567203 },{ 998.959088237,1218.15922592 },{ 997.378843969,1221.30809234 },{ 995.907134052,1224.78990635 },{ 994.451622609,1228.65230303 },{ 992.919973761,1232.94291745 },{ 993.292221797,1237.59252211 },{ 993.702560876,1241.76720346 },{ 994.228998352,1245.50790274 },{ 994.94954158,1248.85556122 },{ 995.942197916,1251.85112014 },{ 997.284974713,1254.53552075 },{ 999.055879328,1256.94970431 },{ 1001.33291911,1259.13461207 },{ 1004.19410143,1261.13118529 },{ 1007.71743362,1262.98036521 },{ 1011.98092305,1264.72309309 },{ 1017.06257707,1266.40031019 },{ 1023.04040304,1268.05295775 },{ 1029.99240831,1269.72197703 },{ 1037.99660023,1271.44830928 },{ 1047.13098617,1273.27289576 },{ 1057.47357347,1275.23667771 },{ 1082.09538158,1279.74559306 } };
	double eps = 5.0;
	auto result = ramer_douglas_peucker_algorithm(pline, eps);
}

#include <geometrix/algorithm/point_sequence/point_polyline_orientation.hpp>

BOOST_FIXTURE_TEST_CASE(polyline_point_orientation_corner_test_point_right, geometry_kernel_2d_fixture)
{
	using namespace geometrix;
	auto pline = polyline2{ {0.0,0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0, 1.0} };
	point2 p = pline[2] + 1.0 * normalize(vector2{ 1.0, 1.0 });
	auto result = point_polyline_orientation(p, pline, cmp);
	BOOST_CHECK(result == oriented_right);
}

BOOST_FIXTURE_TEST_CASE(polyline_point_orientation_corner_test_point_left, geometry_kernel_2d_fixture)
{
	using namespace geometrix;
	auto pline = polyline2{ { 0.0,0.0 },{ 1.0, 0.0 },{ 1.0, 1.0 },{ 0.0, 1.0 } };
	point2 p = pline[2] - 0.2 * normalize(vector2{ 1.0, 1.0 });
	auto result = point_polyline_orientation(p, pline, cmp);
	BOOST_CHECK(result == oriented_left);
}

BOOST_FIXTURE_TEST_CASE(polyline_point_orientation_test, geometry_kernel_2d_fixture)
{
	using namespace geometrix;
	auto pline = polyline2{ { 1098.47527107,1178.48809441 },{ 1071.39171392,1185.84823745 },{ 1059.99795823,1189.00357638 },{ 1049.91310331,1191.87260701 },{ 1041.04481327,1194.50296442 },{ 1033.30075223,1196.94228366 },{ 1026.58858431,1199.2381998 },{ 1020.81597365,1201.43834793 },{ 1015.89058435,1203.59036309 },{ 1011.72008055,1205.74188038 },{ 1008.21212637,1207.94053484 },{ 1005.27438592,1210.23396156 },{ 1002.81452334,1212.6697956 },{ 1000.74020273,1215.29567203 },{ 998.959088237,1218.15922592 },{ 997.378843969,1221.30809234 },{ 995.907134052,1224.78990635 },{ 994.451622609,1228.65230303 },{ 992.919973761,1232.94291745 },{ 993.292221797,1237.59252211 },{ 993.702560876,1241.76720346 },{ 994.228998352,1245.50790274 },{ 994.94954158,1248.85556122 },{ 995.942197916,1251.85112014 },{ 997.284974713,1254.53552075 },{ 999.055879328,1256.94970431 },{ 1001.33291911,1259.13461207 },{ 1004.19410143,1261.13118529 },{ 1007.71743362,1262.98036521 },{ 1011.98092305,1264.72309309 },{ 1017.06257707,1266.40031019 },{ 1023.04040304,1268.05295775 },{ 1029.99240831,1269.72197703 },{ 1037.99660023,1271.44830928 },{ 1047.13098617,1273.27289576 },{ 1057.47357347,1275.23667771 },{ 1082.09538158,1279.74559306 } };
	
	for (std::size_t i = 0, j = 1; j < pline.size(); i = j++)
	{
		auto pi = pline[i];
		auto pj = pline[j];

		//! Collinearity tests.
		{
			auto p = pline[i];
			auto result = point_polyline_orientation(p, pline, cmp);
			BOOST_CHECK(result == oriented_collinear);
		}

		{
			auto pmid = segment_mid_point(segment2{ pi, pj });
			auto result = point_polyline_orientation(pmid, pline, cmp);
			BOOST_CHECK(result == oriented_collinear);
		}
		
		{
			auto p = pline[j];
			auto result = point_polyline_orientation(p, pline, cmp);
			BOOST_CHECK(result == oriented_collinear);
		}

		//! Oriented right tests.
		auto vright = right_normal(normalize(pj - pi));
		{
			point2 p = pline[i] + 0.1 * vright;
			auto result = point_polyline_orientation(p, pline, cmp);
			BOOST_CHECK(result == oriented_right);
		}

		{
			point2 pmid = segment_mid_point(segment2{ pi, pj }) + 0.1 * vright;
			auto result = point_polyline_orientation(pmid, pline, cmp);
			BOOST_CHECK(result == oriented_right);
		}

		{
			point2 p = pline[j] + 0.1 * vright;
			auto result = point_polyline_orientation(p, pline, cmp);
			BOOST_CHECK(result == oriented_right);
		}
		
		//! Oriented left tests.
		auto vleft = left_normal(normalize(pj - pi));
		{
			point2 p = pline[i] + 0.1 * vleft;
			auto result = point_polyline_orientation(p, pline, cmp);
			BOOST_CHECK(result == oriented_left);
		}

		{
			point2 pmid = segment_mid_point(segment2{ pi, pj }) + 0.1 * vleft;
			auto result = point_polyline_orientation(pmid, pline, cmp);
			BOOST_CHECK(result == oriented_left);
		}

		{
			point2 p = pline[j] + 0.1 * vleft;
			auto result = point_polyline_orientation(p, pline, cmp);
			BOOST_CHECK(result == oriented_left);
		}
	}
}

#endif //GEOMETRIX_POINT_SEQUENCE_TESTS_HPP
