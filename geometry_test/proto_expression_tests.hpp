//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#if !defined(GEOMETRIX_PROTO_EXPRESSION_TESTS_HPP)
#define GEOMETRIX_PROTO_EXPRESSION_TESTS_HPP

#include "vector_kernal.hpp"
#include "tuple_kernal.hpp"

#include <geometrix/tensor/vector.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/tensor/matrix.hpp>
#include <geometrix/space/affine_space.hpp>
#include <geometrix/tensor/tensor_traits.hpp>
#include <geometrix/algebra/algebra.hpp>
#include <geometrix/primitive/line.hpp>
#include <geometrix/algorithm/segment_intersection.hpp>

#include <vector>
#include <iostream>
#include <stdexcept>

#include <boost/test/included/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/fusion/algorithm/query/all.hpp>
#include <boost/lambda/lambda.hpp>

template <typename T>
inline double access( T t, std::size_t i )
{
    using namespace geometrix;
    switch( i )
    {
        case 0:
        return get<0>( t );
        case 1:
        return get<1>( t );
        case 2:
        return get<2>( t );
    default:
        throw std::out_of_range( "" );
    }
}

template <typename Vector>
inline void TestVectorOperations()
{
    using namespace geometrix;
    
    
    BOOST_CHECK( (numeric_width_compare< int, double >::value) );
    BOOST_CHECK( (numeric_width_compare< float, double >::value) );

    BOOST_CHECK( !(numeric_width_compare< double, int >::value) );
    BOOST_CHECK( !(numeric_width_compare< double, float >::value) );
    
    vector_vector_3 a,b,c,d;
    Vector e = construct<Vector>(1,1,1);
    Vector f = construct<Vector>(2,2,2);
    
    for( int i = 0; i < 3; ++i)
    {
        a[i] = i;
        b[i] = (2*i);
        c[i] = (3*i);
        d[i] = (i);
    }
        
    BOOST_CHECK( is_tensor< Vector >::value );
    
    e <<= e + f;
    
    BOOST_CHECK_CLOSE( get<0>( e ), 3., 1e-10 );
    BOOST_CHECK_CLOSE( get<1>( e ), 3., 1e-10 );
    BOOST_CHECK_CLOSE( get<2>( e ), 3., 1e-10 );

    
    e <<= 4 * e;
    BOOST_CHECK_CLOSE( get<0>( e ), 12., 1e-10 );
    BOOST_CHECK_CLOSE( get<1>( e ), 12., 1e-10 );
    BOOST_CHECK_CLOSE( get<2>( e ), 12., 1e-10 );
    
    double v;
    v <<= e * e;
    BOOST_CHECK_CLOSE( v, 432.0, 1e-10 );
    //e <<= e * e;//This shouldn't compile.    
    vector_vector_3 g = construct< vector_vector_3 >( 0, 1, 2 );
    e <<= e ^ g;
    BOOST_CHECK_CLOSE( get<0>( e ), 12., 1e-10 );
    BOOST_CHECK_CLOSE( get<1>( e ), -24., 1e-10 );
    BOOST_CHECK_CLOSE( get<2>( e ), 12., 1e-10 );

    e <<= e - f;
        
    typedef BOOST_TYPEOF( e - c ) SpecificExpression;
    //std::cout << typeid(typename SpecificExpression::traits).name() << std::endl;
    BOOST_STATIC_ASSERT(( is_vector<SpecificExpression>::value ));

    typedef BOOST_TYPEOF( (e - c) ^ e ) SpecificExpression2;
    //std::cout << typeid(typename SpecificExpression2).name() << std::endl;
    BOOST_STATIC_ASSERT(( is_vector<SpecificExpression2>::value ));

    BOOST_AUTO( expr, e - c );
	ignore_unused_warning_of( expr );
    BOOST_CHECK( is_tensor< SpecificExpression >::value );

    e <<= ( e - c );

    Vector j = construct<Vector>(1,1,1);
    //e <<= e ^ f;

    e <<= ( (j - e) - f );
    e <<= ( (j - e) ^ f );
    v <<= ( (j - e) * f );
    //e <<= ( e * 4.0 );
    for( int i = 0; i < 3; ++i)
    {
        std::cout
            << " a(" << i << ") = " << a[i]
            << " b(" << i << ") = " << b[i]
            << " c(" << i << ") = " << c[i]
            << " d(" << i << ") = " << d[i]
            << " e(" << i << ") = " << access( e, i )
            << std::endl;
    }

    //! Test adding point + vector
    point_double_3d p1(1,2,3);
    point_double_3d p2;
    p2 <<= p1 + e;
        
    //! Test doesn't interfere with regular shift.
    int t = 1;
    t <<= 1;
    BOOST_CHECK( t == 2 );    
}

inline void TestMatrixOperations()
{
    using namespace geometrix;
    
    
    matrix<double,2,3> m2 = {{ {0, 1, 2}, {3, 4, 5} }};

    matrix<double,3,3> m3 = {{ {0, 1, 2}, {3, 4, 5}, {6, 7, 8} }};
    
    matrix<double,3,3> m4 = {{ {1, 1, 2}, {3, 2, 5}, {6, 7, 3} }};

    matrix<double,3,3> v;

    v <<= m3 + m4;
    v <<= m3 - m4;
    v <<= m3 * m3;
    
    matrix<double,2,3> v2;
    v2 <<= -(m2 * m3);
    
    double a;
    a <<= det(m3);
    BOOST_CHECK_SMALL(a,1e-10);

    matrix<double,3,2> v3;
    BOOST_AUTO(m2_m3, m2*m3);
    v3 <<= trans(m2_m3);
    BOOST_CHECK((get<0,0>(v3) == get<0,0>(m2_m3)));
    BOOST_CHECK((get<0,1>(v3) == get<1,0>(m2_m3)));
    BOOST_CHECK((get<1,0>(v3) == get<0,1>(m2_m3)));
    BOOST_CHECK((get<1,1>(v3) == get<1,1>(m2_m3)));
    BOOST_CHECK((get<2,0>(v3) == get<0,2>(m2_m3)));
    BOOST_CHECK((get<2,1>(v3) == get<1,2>(m2_m3)));
            
    try
    {
        v <<= inv(m3);
        BOOST_CHECK( false );
    }
    catch(...)
    {}

    v <<= inv( m4 );

    matrix<double,2,2> v5, m5 = {{ {1, 2}, {3, 4} }};
    v5 <<= inv( m5 );
}

BOOST_AUTO_TEST_CASE( TestProtoExpressions )
{
    TestVectorOperations<geometrix::vector_double_3d>();
    TestVectorOperations<vector_vector_3>();
    TestVectorOperations<vector_tuple_3>();
    TestMatrixOperations();
}

template <typename Point1, typename Point2, typename Point3, typename Point4>
inline bool is_coplanar( const Point1& x1, const Point2& x2, const Point3& x3, const Point4& x4 )
{
    using namespace geometrix;
    
    double a;
    a <<= (x3-x1)*((x2-x1)^(x4-x3));
    return a == 0; 
}

template <typename Point1, typename Vector1, typename Point2, typename Vector2, typename IPoint>
inline bool lines_intersect( const Point1& p1, const Vector1& v1, const Point2& p2, const Vector2& v2, IPoint& iPoint )
{
    using namespace geometrix;
    
    
    if( !is_coplanar( p1, p1+v1, p2, p2+v2 ) )
        return false;
    
    BOOST_AUTO(u, v1^v2); 
    double m = magnitude_sqrd(u);

    //parallel
    if( m == 0 )
        return boost::fusion::all( u, boost::lambda::_1 == 0 );        
    
    iPoint <<= p1 + ( (((p2-p1)^v2) * u ) / m ) * v1;
    
    return true;
}

#endif//GEOMETRIX_PROTO_EXPRESSION_TESTS_HPP