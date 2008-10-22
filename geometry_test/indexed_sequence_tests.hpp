//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_INDEXED_SEQUENCE_TESTS_HPP
#define _BOOST_GEOMETRY_INDEXED_SEQUENCE_TESTS_HPP
#pragma once

#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include "../geometry/numeric_sequence.hpp"
#include "../geometry/point.hpp"
#include "../geometry/vector.hpp"
#include "../geometry/indexed_sequence_traversal.hpp"
#include "../geometry/detail/pairwise_fusion_operations.hpp"

struct point_s_3
{
    point_s_3( double x, double y, double z )
        : p(3)
    {
        p[0]=x; p[1]=y; p[2]=3;
    }

    double  operator[]( size_t i ) const { return p[i]; }
    double& operator[]( size_t i ) { return p[i]; }

    std::vector< double > p;
};

struct vector_s_3
{
    vector_s_3( double x, double y, double z )
        : p(3)
    {
        p[0]=x; p[1]=y; p[2]=3;
    }

    double  operator[]( size_t i ) const { return p[i]; }
    double& operator[]( size_t i ) { return p[i]; }

    std::vector< double > p;
};

BOOST_DEFINE_USER_POINT_TRAITS( point_s_3, double, 3, use_run_time_access<true>, use_compile_time_access<false> );
BOOST_DEFINE_USER_VECTOR_TRAITS( vector_s_3, double, 3, use_run_time_access<true>, use_compile_time_access<false> );

BOOST_DEFINE_CARTESIAN_ACCESS_TRAITS( point_s_3 );
BOOST_DEFINE_CARTESIAN_ACCESS_TRAITS( vector_s_3 );

BOOST_AUTO_TEST_CASE( TestIndexedSequence )
{
    using namespace boost::numeric::geometry;
    using namespace boost::numeric::geometry::detail;

    typedef numeric_sequence< double, 3 > double_3;
    typedef point< double, 3 > point_3;
    typedef vector< double, 3 > vector_3;
    typedef double numeric_type;

    //double_3 sequence(
    typedef boost::array<double,3> array3;

    array3 ar = { 0., 1., 2. };

    point_s_3 a( 1., 2., 0. );
	vector_s_3 b( 1., 2., 0. );
    
	//! Check addition
	{
        point_s_3 c = a;
        indexed_sequence_traversal::for_each( c, detail::pairwise_binary_operation< point_s_3, std::plus< numeric_type > >( a ) );
        BOOST_CHECK_CLOSE( cartesian_access_traits<point_s_3>::get<0>( c ), 2., 1e-10 );
        BOOST_CHECK_CLOSE( cartesian_access_traits<point_s_3>::get<1>( c ), 4., 1e-10 );
	}

	//! Check subtraction
	{
		point_s_3 c = a;
        indexed_sequence_traversal::for_each( c, detail::pairwise_binary_operation< point_s_3, std::minus< numeric_type > >( a ) );
        BOOST_CHECK_CLOSE( cartesian_access_traits<point_s_3>::get<0>( c ), 0., 1e-10 );
        BOOST_CHECK_CLOSE( cartesian_access_traits<point_s_3>::get<1>( c ), 0., 1e-10 );
	}

	//! Check scalar multiplication
	{
		point_s_3 c = a;
        indexed_sequence_traversal::for_each( c, detail::assign_operation_result< numeric_type, std::multiplies< numeric_type > >( 4. ) );
        BOOST_CHECK_CLOSE( cartesian_access_traits<point_s_3>::get<0>( c ), 4., 1e-10 );
        BOOST_CHECK_CLOSE( cartesian_access_traits<point_s_3>::get<1>( c ), 8., 1e-10 );
	}

	//! Check scalar division
	{
		point_s_3 c = a;
        indexed_sequence_traversal::for_each( c, detail::assign_operation_result< numeric_type, std::divides< numeric_type > >( 4. ) );
        BOOST_CHECK_CLOSE( cartesian_access_traits<point_s_3>::get<0>( c ), .25, 1e-10 );
        BOOST_CHECK_CLOSE( cartesian_access_traits<point_s_3>::get<1>( c ), .5, 1e-10 );
	}
}

#endif //_BOOST_GEOMETRY_INDEXED_SEQUENCE_TESTS_HPP
