//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_TUPLE_KERNAL_HPP
#define GEOMETRIX_TUPLE_KERNAL_HPP

#include <boost/tuple/tuple.hpp>
#include <boost/fusion/include/boost_tuple.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#include <geometrix/tensor/numeric_sequence.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/utility/member_function_fusion_adaptor.hpp>
#include <geometrix/tensor/tuple_vector_access_policy.hpp>

typedef boost::tuple<double,double> tuple_double_2;
struct point_tuple_2 : public tuple_double_2
{
    point_tuple_2( double x = 0.0, double y = 0.0 )
        : tuple_double_2(x,y)
    {}
};

//! Convert the point_tuple_2 into a fusion sequence of the tuple get function (i.e: get<0>, get<1>, get<2>).
GEOMETRIX_COMPILE_INDEXED_MEMBER_FUNCTION_FUSION_SEQUENCE( point_tuple_2, double, get, 2 );

namespace geometrix {
    template <>
    struct construction_policy< point_tuple_2 >
    {    
        static point_tuple_2 construct( const double& x, const double& y )
        {
            return point_tuple_2( x, y );
        }

        template <typename NumericSequence>
        static point_tuple_2 construct( const NumericSequence& args )
        {
            return point_tuple_2( geometrix::get<0>( args ),
                                  geometrix::get<1>( args ) );
        }

        static point_tuple_2 construct( const boost::array<double,2>& args )
        {
            return point_tuple_2( args[0], args[1] );
        }
    };
}//namespace geometrix;

struct vector_tuple_2 : public tuple_double_2
{
    vector_tuple_2( double x = 0.0, double y = 0.0 )
        : tuple_double_2( x, y )
    {}
};

GEOMETRIX_COMPILE_INDEXED_MEMBER_FUNCTION_FUSION_SEQUENCE( vector_tuple_2, double, get, 2 );

namespace geometrix {

    template <>
    struct construction_policy< vector_tuple_2 >
    {    
        static vector_tuple_2 construct( const double& x, const double& y )
        {
            return vector_tuple_2( x, y );
        }

        template <typename NumericSequence>
        static vector_tuple_2 construct( const NumericSequence& args )
        {
            return vector_tuple_2( geometrix::get<0>( args ),
                                   geometrix::get<1>( args ) );
        }

        static vector_tuple_2 construct( const boost::array<double,2>& args )
        {
            return vector_tuple_2( args[0], args[1] );
        }
    };

}//namespace geometrix;

GEOMETRIX_DEFINE_POINT_TRAITS( point_tuple_2, (double), 2, double, double, neutral_reference_frame_2d, tuple_vector_access_policy<point_tuple_2> );
GEOMETRIX_DEFINE_VECTOR_TRAITS( vector_tuple_2, (double), 2, double, double, neutral_reference_frame_2d, tuple_vector_access_policy<vector_tuple_2> );

//! 3d
typedef boost::tuple<double,double,double> tuple_double_3;
struct point_tuple_3 : public tuple_double_3
{
    point_tuple_3( double x=0., double y=0., double z=0. )
        : tuple_double_3(x,y,z)
    {}
};

//! Convert the point_tuple_3 into a fusion sequence of the tuple get function (i.e: get<0>, get<1>, get<2>).
GEOMETRIX_COMPILE_INDEXED_MEMBER_FUNCTION_FUSION_SEQUENCE( point_tuple_3, double, get, 3 );

namespace geometrix {

    template <>
    struct construction_policy< point_tuple_3 >
    {    
        static point_tuple_3 construct( const double& x, const double& y, const double& z )
        {
            return point_tuple_3( x,y,z );
        }

        template <typename NumericSequence>
        static point_tuple_3 construct( const NumericSequence& args )
        {
            return point_tuple_3( geometrix::get<0>( args ),
                geometrix::get<1>( args ),
                geometrix::get<2>( args ) );
        }

        static point_tuple_3 construct( const boost::array<double,3>& args )
        {
            return point_tuple_3( args[0], args[1], args[2] );
        }
    };

}//namespace geometrix;
struct vector_tuple_3 : public tuple_double_3
{
    vector_tuple_3( double x=0., double y=0., double z=0. )
        : tuple_double_3(x,y,z)
    {}
};

GEOMETRIX_COMPILE_INDEXED_MEMBER_FUNCTION_FUSION_SEQUENCE( vector_tuple_3, double, get, 3 );

namespace geometrix {
    
    template <>
    struct construction_policy< vector_tuple_3 >
    {    
        static vector_tuple_3 construct( const double& x, const double& y, const double& z )
        {
            return vector_tuple_3( x,y,z );
        }

        template <typename NumericSequence>
        static vector_tuple_3 construct( const NumericSequence& args )
        {
            return vector_tuple_3( geometrix::get<0>( args ),
                                   geometrix::get<1>( args ),
                                   geometrix::get<2>( args ) );         
        }

        static vector_tuple_3 construct( const boost::array<double,3>& args )
        {
            return vector_tuple_3( args[0], args[1], args[2] );
        }
    };

}//namespace geometrix;

GEOMETRIX_DEFINE_POINT_TRAITS( point_tuple_3, (double), 3, double, double, neutral_reference_frame_3d, tuple_vector_access_policy<point_tuple_3> );
GEOMETRIX_DEFINE_VECTOR_TRAITS( vector_tuple_3, (double), 3, double, double, neutral_reference_frame_3d, tuple_vector_access_policy<vector_tuple_3> );

#endif //GEOMETRIX_TUPLE_KERNAL_HPP
