//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_ACCESS_TIME_TESTS_HPP
#define GEOMETRIX_ACCESS_TIME_TESTS_HPP


#include <boost/test/included/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/fusion/include/boost_tuple.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#include <boost/cstdint.hpp>

#include <geometrix/tensor/numeric_sequence.hpp>
#include <geometrix/space/cartesian_access_traits.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/tensor/vector.hpp>

#include "tuple_kernal.hpp"
#include "vector_kernal.hpp"
#include "scope_timer.hpp"

double s_pointTolerance = 1e-10;

struct point_s
{
    point_s( double x = 0., double y = 0. )
        : x( x ), y( y )
    {}

    inline bool operator ==( const point_s& p ) const
    {
        return !( operator !=( p ) );
//         double difX = x - p.x;
//         double difY = y - p.y;
// 
//         return ((difX > -s_pointTolerance) && (difX < s_pointTolerance)) &&
//             ((difY > -s_pointTolerance) && (difY < s_pointTolerance));
    }

    inline bool operator !=( const point_s& p ) const
    {
        //return !( operator==( p ) );
         		double difX = x - p.x;
         		double difY = y - p.y;
        // 
        //         return !((difX > -s_pointTolerance) && (difX < s_pointTolerance)) &&
        //                ((difY > -s_pointTolerance) && (difY < s_pointTolerance));
         		return ((difX > s_pointTolerance) || (difX < -s_pointTolerance)) ||
         				((difY > s_pointTolerance) || (difY < -s_pointTolerance));
        //		return ( !EQUALS_TOL(x,p.x,s_pointTolerance) || !EQUALS_TOL(y,p.y,s_pointTolerance));
    }

    double x;
    double y;
};

BOOST_AUTO_TEST_CASE( AccessTimeTests )
{
    using namespace geometrix;
    using namespace geometrix::detail;
    using namespace boost::lambda;

    typedef numeric_sequence< double, 3 > double_3;
    typedef point< double, 3 > point_3;
    typedef vector< double, 3 > vector_3;
    typedef double numeric_type;

    boost::int64_t runs = 10000000000;

    //! Measure the run-time access.
    {
        point_s p1( 69.0, 69.0 );
        point_s p2( 69.0, 69.0 );
        double sum = 0;
        MEASURE_SCOPE_TIME( "Operator==true" );

        for( boost::int64_t i=0;i < runs; ++i )
        {
            sum += p1 == p2;
        }
    }

    //! Measure the run-time access.
    {
        point_s p1( 69.0, 69.0 );
        point_s p3( 70.0, 70.0 );
        double sum = 0;
        MEASURE_SCOPE_TIME( "Operator==false" );

        for( boost::int64_t i=0;i < runs; ++i )
        {
            sum += p1 == p3;
        }
    }

    //! Measure the run-time access.
    {
        point_s p1( 69.0, 69.0 );
        point_s p2( 69.0, 69.0 );
        double sum = 0;
        MEASURE_SCOPE_TIME( "Operator!=true" );

        for( boost::int64_t i=0;i < runs; ++i )
        {
            sum += p1 != p2;
        }
    }

    //! Measure the run-time access.
    {
        point_s p1( 69.0, 69.0 );
        point_s p3( 70.0, 70.0 );
        double sum = 0;
        MEASURE_SCOPE_TIME( "Operator!=false" );

        for( boost::int64_t i=0;i < runs; ++i )
        {
            sum += p1 != p3;
        }
    }

    //! Measure the compile-time access.
     {
         point_3 p( 69.0, 69.0, 69.0 );
         double sum = 0;
         MEASURE_SCOPE_TIME( "CompileTimeAccess_GGPoint" );
 
         for( boost::int64_t i=0;i < runs; ++i )
         {
             sum += get<0>( p );
             sum += get<1>( p );
             sum += get<2>( p );
         }
     }

     //! Measure the compile-time access.
     {
         point_3 p( 69.0, 69.0, 69.0 );
         double sum = 0;
         MEASURE_SCOPE_TIME( "CompileTimeCartesianAccess_GGPoint" );
 
         for( boost::int64_t i=0;i < runs; ++i )
         {
             sum += boost::units::quantity_cast<double>( get_x( p ) );
             sum += boost::units::quantity_cast<double>( get_y( p ) );
             sum += boost::units::quantity_cast<double>( get_z( p ) );
         }
     }

    //! Measure the compile-time access.
     {
         point_tuple_3 p( 69.0, 69.0, 69.0 );
         double sum = 0;
         MEASURE_SCOPE_TIME( "CompileTimeAccess" );
 
         for( boost::int64_t i=0;i < runs; ++i )
         {
             sum += get<0>( p );
             sum += get<1>( p );
             sum += get<2>( p );
         }
     }
// 
     //! Measure the run-time access.
//     {
//         point_vector_3 p( 69.0, 69.0, 69.0 );
//         double sum = 0;
//         MEASURE_SCOPE_TIME( "RunTimeAccess" );
// 
//         for( boost::int64_t i=0;i < runs; ++i )
//         {
//             sum += get<0>( p );
//             sum += get<1>( p );
//             sum += get<2>( p );
//         }
//     }

}

#endif //GEOMETRIX_ACCESS_TIME_TESTS_HPP
