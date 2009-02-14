// geometry_test.cpp : Defines the entry point for the console application.
//

// Boost.Test
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_monitor.hpp>
#include <boost/test/unit_test_log.hpp>

#include "access_time_tests.hpp"
#include "point_tests.hpp"
#include "utility_tests.hpp"
#include "product_tests.hpp"
#include "point_sequence_tests.hpp"
#include "tolerance_comparison_tests.hpp"
#include "intersection_tests.hpp"
#include "convex_hull_test.hpp"
#include "segment_intersection_tests.hpp"
#include "trapezoidal_decomposition_test.hpp"
#include "segment_interval_test.hpp"
//#include "boolean_operation_bsp_test.hpp"
#include "reference_frame_tests.hpp"
#include "numeric_sequence_tests.hpp"
//#include "matrix_expression_tests.hpp"
#include "constant_tests.hpp"
#include "indexed_sequence_tests.hpp"
#include "distance_tests.hpp"
//#include "proto_matrix_grammar_test.hpp"
//#include "kd_tree_test.hpp"
//#include "plot_test.hpp"
//#include "units_tests.hpp"

void StandardExceptionTranslator( const std::exception& e )
{
    BOOST_TEST_MESSAGE( e.what() );
}

boost::unit_test::test_suite* init_unit_test_suite( int , char* [] )
{
    boost::unit_test::unit_test_log.set_threshold_level( boost::unit_test::log_messages );
    boost::unit_test::unit_test_monitor.register_exception_translator<std::exception>( &StandardExceptionTranslator );
    boost::unit_test::framework::master_test_suite().p_name.value = "Boost Geometry Testing Framework";
    
    //with explicit registration we could specify a test case timeout
    //boost::unit_test::framework::master_test_suite().add( BOOST_TEST_CASE( &infinite_loop ), 0, /* timeout */ 2 );

    return 0; 
}
