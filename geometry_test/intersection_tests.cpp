///////////////////////////////////////////////////////////////////////////////
// intersection_tests.cpp
//
//  Copyright 2013 Brandon Kohn. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/included/unit_test.hpp>
#include "intersection_tests.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <geometrix/algorithm/point_sequence/is_polygon_simple.hpp>
#include <geometrix/algorithm/polygon_with_holes_as_segment_range.hpp>
#include <geometrix/algorithm/point_in_polygon.hpp>
#include <exact/predicates.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/algorithm_ext/erase.hpp>
#include <exception>
#include <geometrix/utility/scope_timer.ipp>
#include <geometrix/algorithm/intersection/convex_polygon_polygon_intersection.hpp>
#include <geometrix/numeric/number_comparison_policy.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/primitive/segment.hpp>
#include <geometrix/primitive/capsule.hpp>
#include <geometrix/primitive/polygon.hpp>
#include <geometrix/utility/utilities.hpp>

#include "./2d_kernel_fixture.hpp"

#include <iostream>

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

