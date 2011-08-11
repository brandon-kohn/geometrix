//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_MATRIX_TRAITS_TEST_HPP
#define GEOMETRIX_MATRIX_TRAITS_TEST_HPP

#include <geometrix/tensor/matrix_traits.hpp>
#include <geometrix/tensor/index_operator_matrix_access_policy.hpp>
#include <geometrix/tensor/matrix.hpp>
#include <geometrix/algebra/algebra.hpp>
#include <geometrix/arithmetic/matrix_arithmetic.hpp>

#include <boost/fusion/adapted/array.hpp>
#include <boost/fusion/mpl.hpp>

#include <boost/test/unit_test.hpp>
#include <boost/fusion/mpl.hpp>

#include <boost/mpl/less.hpp>

BOOST_AUTO_TEST_CASE( TestMatrixDeclaration )
{
    using namespace geometrix;

    matrix<double,2,2> m = { 0, 1,
                             2, 3 };

    std::cout << m[0][0];
    std::cout << m[0][1];
    std::cout << m[1][0];
    std::cout << m[1][1];

    std::cout << get<0,0>(m);
    std::cout << get<0,1>(m);
    std::cout << get<1,0>(m);
    std::cout << get<1,1>(m);

    row< matrix<double,2,2>, 0 > r0( m );
    std::cout << get<0>(r0);
    std::cout << get<1>(r0);
        
    row< matrix<double,2,2>, 1 > r1( m );
    std::cout << get<0>(r1);
    std::cout << get<1>(r1);

    column< matrix<double,2,2>, 0 > c0( m );
    std::cout << get<0>(c0);
    std::cout << get<1>(c0);
        
    column< matrix<double,2,2>, 1 > c1( m );
    std::cout << get<0>(c1);
    std::cout << get<1>(c1);
}

BOOST_AUTO_TEST_CASE( TestMatrixMultiplication )
{
    using namespace geometrix;
    using namespace geometrix::algebra;

    matrix<double,2,2> m = { 0, 1,
                            2, 3 };

    matrix<double,2,2> n = { 0, 1,
                            2, 3 };

    BOOST_STATIC_ASSERT(( is_homogeneous< row<matrix<double,2,2>,0> >::value ));
        
    typedef result_of::dot_product
    <
        row<matrix<double,2,2>, 0>
      , column<matrix<double,2,2>, 0>
    >::type dot_t;
    
    std::cout << typeid(dot_t).name() << std::endl;

    typedef select_arithmetic_type_from_sequence<row<matrix<double,2,2>,0> >::type t;
        
    double v00 = matrix_product_element<0,0>( m, n );
    std::cout << v00 << std::endl;
    double v01 = matrix_product_element<0,1>( m, n );
    std::cout << v01 << std::endl;
    double v10 = matrix_product_element<1,0>( m, n );
    std::cout << v10 << std::endl;
    double v11 = matrix_product_element<1,1>( m, n );
    std::cout << v11 << std::endl;
   
    matrix<double,2,2> p; p <<= m * n;
    std::cout << get<0,0>( p ) << std::endl;
    std::cout << get<0,1>( p ) << std::endl;
    std::cout << get<1,0>( p ) << std::endl;
    std::cout << get<1,1>( p ) << std::endl;
       
    std::cout << get<0,0>( m * n ) << std::endl;
    std::cout << get<0,1>( m * n ) << std::endl;
    std::cout << get<1,0>( m * n ) << std::endl;
    std::cout << get<1,1>( m * n ) << std::endl;

    vector_double_2D v( 1, 3 );
    BOOST_AUTO( lz , m * v );
    get<0>( lz );
    get<1>( lz );
    std::cout << get<0>( lz ) << std::endl;
    std::cout << get<1>( lz ) << std::endl;
    
    BOOST_AUTO( rlz , v * m );
    get<0,0>( rlz );
    get<0,1>( rlz );
    std::cout << get<0,0>( rlz ) << std::endl;
    std::cout << get<0,1>( rlz ) << std::endl;
    //std::cout << get<1,0>( rlz ) << std::endl;//! out of bounds.
   
}

BOOST_AUTO_TEST_CASE( TestMatrixDeterminant )
{
    using namespace geometrix;

    matrix<double,2,2> m = { 0, 1,
                            2, 3 };

    double d = determinant( m );
    std::cout << d << std::endl;

    matrix<double,3,3> m3 = { 0, 1, 2,
                             3, 4, 5,
                             6, 7, 8 };
    
    {
        typedef matrix_minor<matrix<double,3,3>, 0, 0> m_minor;
        m_minor minor(m3);
        std::cout << get<0,0>( minor ) << " " << get<0,1>(minor) << std::endl;
        std::cout << get<1,0>( minor ) << " " << get<1,1>(minor) << std::endl;
        std::cout << std::endl;
    }

    {
        typedef matrix_minor<matrix<double,3,3>, 0, 1> m_minor;
        m_minor minor(m3);
        std::cout << get<0,0>( minor ) << " " << get<0,1>(minor) << std::endl;
        std::cout << get<1,0>( minor ) << " " << get<1,1>(minor) << std::endl;
        std::cout << std::endl;
    }
    
    {
        typedef matrix_minor<matrix<double,3,3>, 0, 2> m_minor;
        m_minor minor(m3);
        std::cout << get<0,0>( minor ) << " " << get<0,1>(minor) << std::endl;
        std::cout << get<1,0>( minor ) << " " << get<1,1>(minor) << std::endl;
        std::cout << std::endl;
    }

    {
        typedef matrix_minor<matrix<double,3,3>, 0, 0> m_minor;
        m_minor minor(m3);
        std::cout << get<0,0>( minor ) << " " << get<0,1>(minor) << std::endl;
        std::cout << get<1,0>( minor ) << " " << get<1,1>(minor) << std::endl;
        std::cout << std::endl;
    }

    {
        typedef matrix_minor<matrix<double,3,3>, 1, 0> m_minor;
        m_minor minor(m3);
        std::cout << get<0,0>( minor ) << " " << get<0,1>(minor) << std::endl;
        std::cout << get<1,0>( minor ) << " " << get<1,1>(minor) << std::endl;
        std::cout << std::endl;
    }
    
    {
        typedef matrix_minor<matrix<double,3,3>, 2, 0> m_minor;
        m_minor minor(m3);
        std::cout << get<0,0>( minor ) << " " << get<0,1>(minor) << std::endl;
        std::cout << get<1,0>( minor ) << " " << get<1,1>(minor) << std::endl;
        std::cout << std::endl;
    }

    d = determinant( m3 );
    BOOST_CHECK_CLOSE( d, 0, 1e-10 );
    std::cout << d << std::endl;

    matrix<double,3,3> m4 = { 1, 1, 2,
                             3, 2, 5,
                             6, 7, 3 };

    d = determinant( m4 );   
    
    BOOST_CHECK_CLOSE( d, 10.0, 1e-10 );
}

BOOST_AUTO_TEST_CASE( TestMatrixTensorProduct )
{
    matrix<double,2,2> m = { 1, 2,
                             3, 4 };

    matrix<double,2,2> n = { 0, 5,
                             6, 7 };

    using namespace algebra;
    matrix<double,4,4> mn;
    mn <<= m & n;
}

#endif

