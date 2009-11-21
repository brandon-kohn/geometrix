//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GENERATIVE_GEOMETRY_ACCESS_TIME_TESTS_HPP
#define GENERATIVE_GEOMETRY_ACCESS_TIME_TESTS_HPP
#pragma once

#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/fusion/include/boost_tuple.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#include <boost/cstdint.hpp>

#include <geometry/numeric_sequence.hpp>
#include <geometry/point.hpp>
#include <geometry/vector.hpp>
#include <geometry/indexed_sequence_traversal.hpp>
#include <geometry/indexed_sequence_operators.hpp>

#include "tuple_kernal.hpp"
#include "vector_kernal.hpp"
#include "scope_timer.hpp"

BOOST_AUTO_TEST_CASE( AccessTimeTests )
{
    using namespace generative::numeric::geometry;
    using namespace generative::numeric::geometry::detail;
    using namespace boost::lambda;

    typedef numeric_sequence< double, 3 > double_3;
    typedef point< double, 3 > point_3;
    typedef vector< double, 3 > vector_3;
    typedef double numeric_type;

    boost::int64_t runs = 10000000000;

    //! Measure the compile-time access.
//     {
//         point_3 p( 69.0, 69.0, 69.0 );
//         double sum = 0;
//         MEASURE_SCOPE_TIME( "CompileTimeAccess_GGPoint" );
// 
//         for( boost::int64_t i=0;i < runs; ++i )
//         {
//             sum += indexed_access_traits< point_3 >::get<0>( p );
//             sum += indexed_access_traits< point_3 >::get<1>( p );
//             sum += indexed_access_traits< point_3 >::get<2>( p );
//         }
//     }

    //! Measure the compile-time access.
//     {
//         point_tuple_3 p( 69.0, 69.0, 69.0 );
//         double sum = 0;
//         MEASURE_SCOPE_TIME( "CompileTimeAccess" );
// 
//         for( boost::int64_t i=0;i < runs; ++i )
//         {
//             sum += indexed_access_traits< point_tuple_3 >::get<0>( p );
//             sum += indexed_access_traits< point_tuple_3 >::get<1>( p );
//             sum += indexed_access_traits< point_tuple_3 >::get<2>( p );
//         }
//     }
// 
     //! Measure the run-time access.
    {
        point_vector_3 p( 69.0, 69.0, 69.0 );
        double sum = 0;
        MEASURE_SCOPE_TIME( "RunTimeAccess" );

        for( boost::int64_t i=0;i < runs; ++i )
        {
            sum += indexed_access_traits< point_vector_3 >::get<0>( p );
            sum += indexed_access_traits< point_vector_3 >::get<1>( p );
            sum += indexed_access_traits< point_vector_3 >::get<2>( p );
        }
    }

}

#endif //GENERATIVE_GEOMETRY_ACCESS_TIME_TESTS_HPP
