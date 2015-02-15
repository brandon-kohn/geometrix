//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_COMPOSE_MATRIX_TEST_HPP
#define GEOMETRIX_COMPOSE_MATRIX_TEST_HPP

#include <geometrix/tensor/matrix_traits.hpp>
#include <geometrix/tensor/index_operator_matrix_access_policy.hpp>
#include <geometrix/tensor/matrix.hpp>
#include <geometrix/algebra/algebra.hpp>
#include <geometrix/arithmetic/matrix_arithmetic.hpp>

#include <boost/fusion/adapted/array.hpp>
#include <boost/fusion/mpl.hpp>
#include <boost/test/included/unit_test.hpp>
#include <boost/fusion/mpl.hpp>
#include <boost/mpl/less.hpp>
        
BOOST_AUTO_TEST_CASE( ComposeMatrixTest )
{
    using namespace geometrix;
    
    
    typedef boost::mpl::vector
        <
            matrix<int,4,4>, matrix<int,1,2>, matrix<int,3,2>
          ,    /**/         matrix<int,4,4>,    /**/
            matrix<int,1,2>     /**/            /**/
        > sequence;
        
    std::cout << typeid(result_of::detail::compose_matrix_schema< sequence, 6 >::type).name() << std::endl;
    
    typedef boost::mpl::vector
        <
            matrix<int,1,2>, matrix<int,4,4>, matrix<int,1,2>
          , matrix<int,4,4>,    /**/         matrix<int,1,2>
          ,    /**/        matrix<double,2,2>, matrix<int,4,4>
          , matrix<int,1,2>
        > sequence2;
     
    std::cout << typeid(result_of::detail::compose_matrix_schema< sequence2, 6 >::type).name() << std::endl;
    
    std::cout << typeid(result_of::detail::compose_matrix_schema
                <
                    boost::mpl::vector<matrix<double,3,3>, column_vector<vector_int_3d>, row_vector<vector_float_4d> >, 4
                >::type).name() << std::endl;
        
    typedef boost::result_of
        <
            result_of::compose_matrix_helper<4,4>
            (
                matrix<double,3,3>
              , column_vector<vector_int_3d>
              , row_vector<vector_float_4d>
            )
        >::type cmatrix;
    
    typedef boost::result_of
        <
            result_of::compose_matrix_helper<4,4>
            (
                matrix<double,3,3>
              , column_vector<vector_float_3d>
              , row_vector<vector_double_4d>
            )
        >::type cmatrix2;
    
    matrix<double,3,3> m4 = { 1, 1, 2,
                             3, 2, 5,
                             6, 7, 3 };
    vector_int_3d v1( 7, 8, 9 );
    vector_float_4d v2( 10, 11, 12, 13 );

    column_vector<vector_int_3d> cv1(v1);
    row_vector<vector_float_4d> rv2(v2);
    cmatrix cm( as_matrix<4,4>( m4, cv1, rv2 ) );
    
    std::cout << cm.get<0,0>() << " ";
    
    std::cout << cm.get<0,1>() << " ";
    std::cout << cm.get<0,2>() << " ";
    std::cout << cm.get<0,3>() << " ";
    std::cout << "\n";
    std::cout << cm.get<1,0>() << " ";
    std::cout << cm.get<1,1>() << " ";
    std::cout << cm.get<1,2>() << " ";
    std::cout << cm.get<1,3>() << " ";
    std::cout << "\n";
    std::cout << cm.get<2,0>() << " ";
    std::cout << cm.get<2,1>() << " ";
    std::cout << cm.get<2,2>() << " ";
    std::cout << cm.get<2,3>() << " ";
    std::cout << "\n";
    std::cout << cm.get<3,0>() << " ";
    std::cout << cm.get<3,1>() << " ";
    std::cout << cm.get<3,2>() << " ";
    std::cout << cm.get<3,3>() << " ";
        
    std::cout << "Det: " << get( det(cm) ) << std::endl;
    
    row<cmatrix,0> r0(cm);
    row<cmatrix,1> r1(cm);
    row<cmatrix,2> r2(cm);
    row<cmatrix,3> r3(cm);

    std::cout << get<0>( r0 ) << " ";
    std::cout << get<1>( r0 ) << " ";
    std::cout << get<2>( r0 ) << " ";
    std::cout << get<3>( r0 ) << " ";
    std::cout << "\n";
    std::cout << get<0>( r1 ) << " ";
    std::cout << get<1>( r1 ) << " ";
    std::cout << get<2>( r1 ) << " ";
    std::cout << get<3>( r1 ) << " ";
    std::cout << "\n";
    std::cout << get<0>( r2 ) << " ";
    std::cout << get<1>( r2 ) << " ";
    std::cout << get<2>( r2 ) << " ";
    std::cout << get<3>( r2 ) << " ";
    std::cout << "\n";
    std::cout << get<0>( r3 ) << " ";
    std::cout << get<1>( r3 ) << " ";
    std::cout << get<2>( r3 ) << " ";
    std::cout << get<3>( r3 ) << " ";
    std::cout << "\n";

    column<cmatrix,0> c0(cm);
    column<cmatrix,1> c1(cm);
    column<cmatrix,2> c2(cm);
    column<cmatrix,3> c3(cm);

    std::cout << get<0>( c0 ) << " ";
    std::cout << get<1>( c0 ) << " ";
    std::cout << get<2>( c0 ) << " ";
    std::cout << get<3>( c0 ) << " ";
    std::cout << "\n";
    std::cout << get<0>( c1 ) << " ";
    std::cout << get<1>( c1 ) << " ";
    std::cout << get<2>( c1 ) << " ";
    std::cout << get<3>( c1 ) << " ";
    std::cout << "\n";
    std::cout << get<0>( c2 ) << " ";
    std::cout << get<1>( c2 ) << " ";
    std::cout << get<2>( c2 ) << " ";
    std::cout << get<3>( c2 ) << " ";
    std::cout << "\n";
    std::cout << get<0>( c3 ) << " ";
    std::cout << get<1>( c3 ) << " ";
    std::cout << get<2>( c3 ) << " ";
    std::cout << get<3>( c3 ) << " ";
    std::cout << "\n";
    
    matrix<double,4,4> dm;
    dm <<= cm * cm;

    vector_float_3d v3( 7, 8, 9 );
    vector_double_4d v4( 10, 11, 12, 13 );

    column_vector<vector_float_3d> cv3(v3);
    row_vector<vector_double_4d> rv4(v4);
    cmatrix2 cm2( as_matrix<4,4>( m4, cv3, rv4 ) );
    dm <<= cm2 * cm2;
    
    std::cout << get<0,0>( dm ) << " ";
    std::cout << get<0,1>( dm ) << " ";
    std::cout << get<0,2>( dm ) << " ";
    std::cout << get<0,3>( dm ) << " ";
    std::cout << "\n";
    std::cout << get<1,0>( dm ) << " ";
    std::cout << get<1,1>( dm ) << " ";
    std::cout << get<1,2>( dm ) << " ";
    std::cout << get<1,3>( dm ) << " ";
    std::cout << "\n";
    std::cout << get<2,0>( dm ) << " ";
    std::cout << get<2,1>( dm ) << " ";
    std::cout << get<2,2>( dm ) << " ";
    std::cout << get<2,3>( dm ) << " ";
    std::cout << "\n";
    std::cout << get<3,0>( dm ) << " ";
    std::cout << get<3,1>( dm ) << " ";
    std::cout << get<3,2>( dm ) << " ";
    std::cout << get<3,3>( dm ) << " ";
    std::cout << "\n";

    column_vector<vector_float_3d> ca(v3);
    typedef result_of::as_matrix
        <
            3
          , 3
          , column_vector<vector_float_3d>
          , column_vector<vector_float_3d>
          , column_vector<vector_float_3d>
    >::type col_matrix;
    col_matrix cma( as_matrix<3,3>( ca, ca, ca ) );
    
    matrix<float,3,3> dm1;
    dm1 <<= cma * cma;
    
    std::cout << get<0,0>( as_matrix<3,3>( ca, ca, ca ) * as_matrix<3,3>( ca, ca, ca ) ) << " ";
    std::cout << get<0,1>( as_matrix<3,3>( ca, ca, ca ) * as_matrix<3,3>( ca, ca, ca ) ) << " ";
    std::cout << get<0,2>( as_matrix<3,3>( ca, ca, ca ) * as_matrix<3,3>( ca, ca, ca ) ) << " ";
    std::cout << "\n";
    std::cout << get<1,0>( as_matrix<3,3>( ca, ca, ca ) * as_matrix<3,3>( ca, ca, ca ) ) << " ";
    std::cout << get<1,1>( as_matrix<3,3>( ca, ca, ca ) * as_matrix<3,3>( ca, ca, ca ) ) << " ";
    std::cout << get<1,2>( as_matrix<3,3>( ca, ca, ca ) * as_matrix<3,3>( ca, ca, ca ) ) << " ";
    std::cout << "\n";
    std::cout << get<2,0>( as_matrix<3,3>( ca, ca, ca ) * as_matrix<3,3>( ca, ca, ca ) ) << " ";
    std::cout << get<2,1>( as_matrix<3,3>( ca, ca, ca ) * as_matrix<3,3>( ca, ca, ca ) ) << " ";
    std::cout << get<2,2>( as_matrix<3,3>( ca, ca, ca ) * as_matrix<3,3>( ca, ca, ca ) ) << " ";

    std::cout << "\n";
    std::cout << get<0,0>( as_matrix<4,4>( rv4, rv4, rv4, rv4 ) * as_matrix<4,4>( rv4, rv4, rv4, rv4 ) ) << " ";
    std::cout << get<0,1>( as_matrix<4,4>( rv4, rv4, rv4, rv4 ) * as_matrix<4,4>( rv4, rv4, rv4, rv4 ) ) << " ";
    std::cout << get<0,2>( as_matrix<4,4>( rv4, rv4, rv4, rv4 ) * as_matrix<4,4>( rv4, rv4, rv4, rv4 ) ) << " ";
    std::cout << get<0,3>( as_matrix<4,4>( rv4, rv4, rv4, rv4 ) * as_matrix<4,4>( rv4, rv4, rv4, rv4 ) ) << " ";
    std::cout << "\n";
    std::cout << get<1,0>( as_matrix<4,4>( rv4, rv4, rv4, rv4 ) * as_matrix<4,4>( rv4, rv4, rv4, rv4 ) ) << " ";
    std::cout << get<1,1>( as_matrix<4,4>( rv4, rv4, rv4, rv4 ) * as_matrix<4,4>( rv4, rv4, rv4, rv4 ) ) << " ";
    std::cout << get<1,2>( as_matrix<4,4>( rv4, rv4, rv4, rv4 ) * as_matrix<4,4>( rv4, rv4, rv4, rv4 ) ) << " ";
    std::cout << get<1,3>( as_matrix<4,4>( rv4, rv4, rv4, rv4 ) * as_matrix<4,4>( rv4, rv4, rv4, rv4 ) ) << " ";
    std::cout << "\n";
    std::cout << get<2,0>( as_matrix<4,4>( rv4, rv4, rv4, rv4 ) * as_matrix<4,4>( rv4, rv4, rv4, rv4 ) ) << " ";
    std::cout << get<2,1>( as_matrix<4,4>( rv4, rv4, rv4, rv4 ) * as_matrix<4,4>( rv4, rv4, rv4, rv4 ) ) << " ";
    std::cout << get<2,2>( as_matrix<4,4>( rv4, rv4, rv4, rv4 ) * as_matrix<4,4>( rv4, rv4, rv4, rv4 ) ) << " ";
    std::cout << get<2,3>( as_matrix<4,4>( rv4, rv4, rv4, rv4 ) * as_matrix<4,4>( rv4, rv4, rv4, rv4 ) ) << " ";
    std::cout << "\n";
    std::cout << get<3,0>( as_matrix<4,4>( rv4, rv4, rv4, rv4 ) * as_matrix<4,4>( rv4, rv4, rv4, rv4 ) ) << " ";
    std::cout << get<3,1>( as_matrix<4,4>( rv4, rv4, rv4, rv4 ) * as_matrix<4,4>( rv4, rv4, rv4, rv4 ) ) << " ";
    std::cout << get<3,2>( as_matrix<4,4>( rv4, rv4, rv4, rv4 ) * as_matrix<4,4>( rv4, rv4, rv4, rv4 ) ) << " ";
    std::cout << get<3,3>( as_matrix<4,4>( rv4, rv4, rv4, rv4 ) * as_matrix<4,4>( rv4, rv4, rv4, rv4 ) ) << " ";
    std::cout << "\n";
}

#endif//GEOMETRIX_COMPOSE_MATRIX_TEST_HPP
