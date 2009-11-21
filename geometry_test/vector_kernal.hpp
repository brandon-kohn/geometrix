//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GENERATIVE_GEOMETRY_VECTOR_KERNAL_HPP
#define GENERATIVE_GEOMETRY_VECTOR_KERNAL_HPP
#pragma once

#include <boost/fusion/include/boost_tuple.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#include <geometry\../geometry/numeric_sequence.hpp>
#include <geometry\../geometry/point.hpp>
#include <geometry\../geometry/vector.hpp>
#include <geometry\../geometry/indexed_sequence_traversal.hpp>
#include <geometry\../geometry/indexed_sequence_operators.hpp>
#include <geometry\../geometry/detail/member_function_fusion_adaptor.hpp>
#include <vector>

struct point_vector_3
{
    point_vector_3( double x=0., double y=0., double z=0. )
        : p(3)
    {
        p[0]=x; p[1]=y; p[2]=z;
    }

    const double&  operator[]( size_t i ) const { return p[i]; }
    double& operator[]( size_t i ) { return p[i]; }

    std::vector< double > p;
};

GENERATIVE_GEOMETRY_INDEX_OPERATOR_FUSION_SEQUENCE( point_vector_3, double, 3 );

template <>
struct generative::numeric::geometry::construction_traits< point_vector_3 >
{    
    static point_vector_3 construct( const double& x, const double& y, const double& z )
    {
        return point_vector_3( x,y,z );
    }

    template <typename NumericSequence>
    static point_vector_3 construct( const NumericSequence& args )
    {
        return point_vector_3( generative::numeric::geometry::indexed_access_traits<NumericSequence>::get<0>( args ),
                               generative::numeric::geometry::indexed_access_traits<NumericSequence>::get<1>( args ),
                               generative::numeric::geometry::indexed_access_traits<NumericSequence>::get<2>( args ) );
    }
};

struct vector_vector_3
{
    vector_vector_3( double x=0., double y=0., double z=0. )
        : p(3)
    {
        p[0]=x; p[1]=y; p[2]=z;
    }

    const double& operator[]( size_t i ) const { return p[i]; }
    double&       operator[]( size_t i ) { return p[i]; }

    std::vector< double > p;
};

GENERATIVE_GEOMETRY_INDEX_OPERATOR_FUSION_SEQUENCE( vector_vector_3, double, 3 );

template <>
struct generative::numeric::geometry::construction_traits< vector_vector_3 >
{    
    static vector_vector_3 construct( const double& x, const double& y, const double& z )
    {
        return vector_vector_3( x,y,z );
    }

    template <typename NumericSequence>
    static vector_vector_3 construct( const NumericSequence& args )
    {
        return vector_vector_3( generative::numeric::geometry::indexed_access_traits<NumericSequence>::get<0>( args ),
                                generative::numeric::geometry::indexed_access_traits<NumericSequence>::get<1>( args ),
                                generative::numeric::geometry::indexed_access_traits<NumericSequence>::get<2>( args ) );
    }
};

GENERATIVE_GEOMETRY_DEFINE_USER_POINT_TRAITS( point_vector_3, double, 3, neutral_reference_frame_double_3d, require_run_time_access_policy );
GENERATIVE_GEOMETRY_DEFINE_USER_VECTOR_TRAITS( vector_vector_3, double, 3, neutral_reference_frame_double_3d, require_run_time_access_policy );

#endif //GENERATIVE_GEOMETRY_VECTOR_KERNAL_HPP
