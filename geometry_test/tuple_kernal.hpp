//
//! Copyright © 2008
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _GENERATIVE_GEOMETRY_TUPLE_KERNAL_HPP
#define _GENERATIVE_GEOMETRY_TUPLE_KERNAL_HPP
#pragma once

#include <boost/tuple/tuple.hpp>
#include <boost/fusion/include/boost_tuple.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#include "../geometry/numeric_sequence.hpp"
#include "../geometry/point.hpp"
#include "../geometry/vector.hpp"
#include "../geometry/indexed_sequence_traversal.hpp"
#include "../geometry/indexed_sequence_operators.hpp"
#include "../geometry/detail/member_function_fusion_adaptor.hpp"

typedef boost::tuple<double,double,double> tuple_double_3;
struct point_tuple_3 : public tuple_double_3
{
    point_tuple_3( double x, double y, double z )
        : tuple_double_3(x,y,z)
    {}
};

//! Convert the point_tuple_3 into a fusion sequence of the tuple get function (i.e: get<0>, get<1>, get<2>).
BOOST_GEOMETRY_COMPILE_INDEXED_MEMBER_FUNCTION_FUSION_SEQUENCE( point_tuple_3, double, get, 3 );

template <>
struct generative::numeric::geometry::construction_traits< point_tuple_3 >
{    
    static inline point_tuple_3 construct( const double& x, const double& y, const double& z )
    {
        return point_tuple_3( x,y,z );
    }

    template <typename NumericSequence>
    static inline point_tuple_3 construct( const NumericSequence& args )
    {
        return point_tuple_3( generative::numeric::geometry::indexed_access_traits<NumericSequence>::get<0>( args ),
            generative::numeric::geometry::indexed_access_traits<NumericSequence>::get<1>( args ),
            generative::numeric::geometry::indexed_access_traits<NumericSequence>::get<2>( args ) );
    }

    static inline point_tuple_3 construct( const boost::array<double,3>& args )
    {
        return point_tuple_3( args[0], args[1], args[2] );
    }
};

struct vector_tuple_3 : public tuple_double_3
{
    vector_tuple_3( double x, double y, double z )
        : tuple_double_3(x,y,z)
    {}
};

BOOST_GEOMETRY_COMPILE_INDEXED_MEMBER_FUNCTION_FUSION_SEQUENCE( vector_tuple_3, double, get, 3 );

template <>
struct generative::numeric::geometry::construction_traits< vector_tuple_3 >
{    
    static inline vector_tuple_3 construct( const double& x, const double& y, const double& z )
    {
        return vector_tuple_3( x,y,z );
    }

    template <typename NumericSequence>
    static inline vector_tuple_3 construct( const NumericSequence& args )
    {
        return vector_tuple_3( generative::numeric::geometry::indexed_access_traits<NumericSequence>::get<0>( args ),
            generative::numeric::geometry::indexed_access_traits<NumericSequence>::get<1>( args ),
            generative::numeric::geometry::indexed_access_traits<NumericSequence>::get<2>( args ) );         
    }

    static inline vector_tuple_3 construct( const boost::array<double,3>& args )
    {
        return vector_tuple_3( args[0], args[1], args[2] );
    }
};

BOOST_DEFINE_USER_POINT_TRAITS( point_tuple_3, double, 3, neutral_reference_frame_double_3d, use_compile_time_access );
BOOST_DEFINE_USER_VECTOR_TRAITS( vector_tuple_3, double, 3, neutral_reference_frame_double_3d, use_compile_time_access );

#endif //_GENERATIVE_GEOMETRY_TUPLE_KERNAL_HPP
