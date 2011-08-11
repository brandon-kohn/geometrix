///////////////////////////////////////////////////////////////////////////////
//  Copyright 2008 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// This example constructs a mini-library for linear algebra, using
// expression templates to eliminate the need for temporaries when
// adding arrays of numbers. It duplicates the vector example from
// PETE (http://www.codesourcery.com/pooma/download.html)

#include "vector_kernal.hpp"
#include "tuple_kernal.hpp"

#include <geometry\vector.hpp>
#include <geometry\point.hpp>
#include <geometry\affine_space.hpp>
#include <geometry\indexed_access_traits.hpp>
#include <geometry\indexed_sequence_expression.hpp>

#include <vector>
#include <iostream>
#include <stdexcept>

#include <boost\test\unit_test.hpp>
#include <boost\test\floating_point_comparison.hpp>

// Here is indexed_sequence_expression, which extends a proto expr type by
// giving it an operator [] which uses the indexed_sequence_subscript_context
// to evaluate an expression with a given index.
template <typename T>
double access( T t, std::size_t i )
{
    using namespace generative::geometry;
    switch( i )
    {
        case 0:
        return indexed_access_traits< T >::get<0>( t );
        case 1:
        return indexed_access_traits< T >::get<1>( t );
        case 2:
        return indexed_access_traits< T >::get<2>( t );
    default:
        throw std::out_of_range( "" );
    }
}

template <typename Vector>
void TestVectorOperations()
{
    using namespace generative::geometry;
    using namespace generative::geometry::indexed_sequence_operators;
    
    BOOST_CHECK( (numeric_width_compare< int, double >::type::value) );
    BOOST_CHECK( (numeric_width_compare< float, double >::type::value) );

    BOOST_CHECK( !(numeric_width_compare< double, int >::type::value) );
    BOOST_CHECK( !(numeric_width_compare< double, float >::type::value) );

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

    int five = 5 * 1;

    BOOST_CHECK( is_indexed_sequence< Vector >::value );
    e << e + f;
    BOOST_CHECK_CLOSE( get<0>( e ), 3., 1e-10 );
    BOOST_CHECK_CLOSE( get<1>( e ), 3., 1e-10 );
    BOOST_CHECK_CLOSE( get<2>( e ), 3., 1e-10 );

    e << e * 4;
    BOOST_CHECK_CLOSE( get<0>( e ), 12., 1e-10 );
    BOOST_CHECK_CLOSE( get<1>( e ), 12., 1e-10 );
    BOOST_CHECK_CLOSE( get<2>( e ), 12., 1e-10 );
 
    typedef BOOST_TYPEOF( e - 4 / (c + 1) ) SpecificExpression;
    BOOST_AUTO( expr, e - 4 / (c + 1) );
 
    BOOST_CHECK( is_indexed_sequence< SpecificExpression >::value );

    double x0 = expr.get<0>();
    double x1 = x0 * 3;
    
    double x = get<0>( expr );
    BOOST_CHECK( x0 == x );
    std::cout << "x: " << x << std::endl;
    e << ( e - 4 / (c + 1) );

    Vector j = construct<Vector>(1,1,1);
    e << ( (j - 2) * f );
    e << ( e * 4.0 );
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

    //! Test doesn't interfere with regular shift.
    int t = 1;
    t = t << 1;
    BOOST_CHECK( t == 2 );
}

BOOST_AUTO_TEST_CASE( TestProtoExpressions )
{
    TestVectorOperations<vector_vector_3>();
    TestVectorOperations<vector_tuple_3>();
}
