///////////////////////////////////////////////////////////////////////////////
// numeric_sequence_fail_construction.cpp
//
//  Copyright 2013 Brandon Kohn. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/included/unit_test.hpp>
#include "2d_kernel_units_fixture.hpp"

BOOST_FIXTURE_TEST_CASE(construction_test_with_units, geometry_kernel_2d_units_fixture)
{
    using namespace geometrix;

    point2 A = { 413942.3119* boost::units::si::meters, 3698734.116* boost::units::si::meters };// , 324.0720825
    
    auto p = point<double, 2>{ A };//! Should not compile.
}

void StandardExceptionTranslator( const std::exception& e )
{
    BOOST_TEST_MESSAGE( e.what() );
}

boost::unit_test::test_suite* init_unit_test_suite( int , char* [] )
{
    boost::unit_test::unit_test_log.set_threshold_level( boost::unit_test::log_messages );
    boost::unit_test::unit_test_monitor.register_exception_translator<std::exception>( &StandardExceptionTranslator );
    boost::unit_test::framework::master_test_suite().p_name.value = "Geometrix Testing Framework";
    return 0; 
}
