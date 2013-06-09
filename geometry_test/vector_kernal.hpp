//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_VECTOR_KERNAL_HPP
#define GEOMETRIX_VECTOR_KERNAL_HPP

#include <boost/fusion/include/boost_tuple.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#include <geometrix/tensor/numeric_sequence.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/utility/member_function_fusion_adaptor.hpp>
#include <geometrix/tensor/fusion_vector_access_policy.hpp>
#include <vector>

struct point_vector_3
{
    point_vector_3( double x=0., double y=0., double z=0. )
        : p(3)
    {
        p[0]=x; p[1]=y; p[2]=z;
    }

    const double&  operator[]( std::size_t i ) const { return p[i]; }
    double& operator[]( std::size_t i ) { return p[i]; }

    std::vector< double > p;
};

GEOMETRIX_INDEX_OPERATOR_FUSION_SEQUENCE( point_vector_3, double, 3 );

namespace geometrix {

    template <>
    struct construction_policy< point_vector_3 >
    {    
        static point_vector_3 construct( const double& x, const double& y, const double& z )
        {
            return point_vector_3( x,y,z );
        }

        template <typename NumericSequence>
        static point_vector_3 construct( const NumericSequence& args )
        {
            return point_vector_3( geometrix::get<0>( args ),
                                   geometrix::get<1>( args ),
                                   geometrix::get<2>( args ) );
        }
    };
    
}//namespace geometrix;
struct vector_vector_3
{
    vector_vector_3( double x=0., double y=0., double z=0. )
        : p(3)
    {
        p[0]=x; p[1]=y; p[2]=z;
    }

    template <typename Expr>
    vector_vector_3( const Expr& e )
        : p(3)    
    {
        p[0] = geometrix::get<0>( e );
        p[1] = geometrix::get<1>( e );
        p[2] = geometrix::get<2>( e );
    }

    vector_vector_3( const vector_vector_3& p )        
        : p(p.p)
    {
        static int c = 0;
        std::cout << "Copying vector_vector_3: " << ++c << std::endl;
    }
    
    const double& operator[]( std::size_t i ) const { return p[i]; }
    double&       operator[]( std::size_t i ) { return p[i]; }

    std::vector< double > p;
};

GEOMETRIX_INDEX_OPERATOR_FUSION_SEQUENCE( vector_vector_3, double, 3 );

namespace geometrix {

    template <>
    struct construction_policy< vector_vector_3 >
    {    
        static vector_vector_3 construct( const double& x, const double& y, const double& z )
        {
            return vector_vector_3( x,y,z );
        }

        template <typename NumericSequence>
        static vector_vector_3 construct( const NumericSequence& args )
        {
            return vector_vector_3( geometrix::get<0>( args ),
                                    geometrix::get<1>( args ),
                                    geometrix::get<2>( args ) );
        }
    };

}//namespace geometrix;
GEOMETRIX_DEFINE_POINT_TRAITS( point_vector_3, (double), 3, double, neutral_reference_frame_3d, fusion_vector_access_policy<point_vector_3> );
GEOMETRIX_DEFINE_VECTOR_TRAITS( vector_vector_3, (double), 3, double, neutral_reference_frame_3d, fusion_vector_access_policy<vector_vector_3> );

#endif //GEOMETRIX_VECTOR_KERNAL_HPP
