//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_LINEAR_ALGEBRA_TIME_TESTS_HPP
#define GEOMETRIX_LINEAR_ALGEBRA_TIME_TESTS_HPP


#include <boost/test/included/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/fusion/include/boost_tuple.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#include <boost/cstdint.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/lu.hpp>

#include <geometrix/tensor/numeric_sequence.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/algebra/algebra.hpp>

#include "tuple_kernal.hpp"
#include "vector_kernal.hpp"
#include "scope_timer.hpp"
/*
template<class matrix_T>
double b_determinant(boost::numeric::ublas::matrix_expression<matrix_T> const& mat_r)
{
  double det = 1.0;

  matrix_T mLu(mat_r() );
  boost::numeric::ublas::permutation_matrix<std::size_t> pivots(mat_r().size1() );

  int is_singular = boost::numeric::ublas::lu_factorize(mLu, pivots);

  if (!is_singular)
  {
    for (std::size_t i=0; i < pivots.size(); ++i)
    {
      if (pivots(i) != i)
        det *= -1.0;

      det *= mLu(i,i);
    }
  }
  else
    det = 0.0;

  return det;
} 

BOOST_AUTO_TEST_CASE( MatrixVsUBlasTimeTests )
{
    using namespace geometrix;
    using namespace geometrix::algebra;
    matrix<double,2,2> m = { 0, 1,
                            2, 3 };

    matrix<double,2,3> m2 = { 0, 1, 2,
                             3, 4, 5 };

    matrix<double,3,3> m3 = { 0, 1, 2,
                             3, 4, 5,
                             6, 7, 8 };
    
    matrix<double,3,3> m4 = { 1, 1, 2,
                             3, 2, 5,
                             6, 7, 3 };
    
    boost::int64_t runs = 1000000;
         
    boost::numeric::ublas::matrix<double> vb (2, 3);
    {
        using namespace boost::numeric::ublas;
        boost::numeric::ublas::matrix<double> mb2 (2, 3), mb3 (3, 3);
        for (unsigned i = 0; i < 2; ++ i)
            for (unsigned j = 0; j < 3; ++ j)
                mb2(i, j) = m2[i][j];

        for (unsigned i = 0; i < 3; ++ i)
            for (unsigned j = 0; j < 3; ++ j)
                mb3(i, j) = m3[i][j];
        
        
        double sum = 0;
        {
            MEASURE_SCOPE_TIME( "UBlasMatrixMultiplication" );
            for( boost::int64_t i=0;i < runs; ++i )
                vb = boost::numeric::ublas::prod(mb2, mb3);
        }
    }

    //! Measure matrix multiplication.
    matrix<double,2,3> v;
    {
        double sum = 0;
        MEASURE_SCOPE_TIME( "CompileTimeMatrixMultiplication" );
        for( boost::int64_t i=0;i < runs; ++i )
        {
            v <<= m2 * m3;
        }
    }
   
    for (unsigned i = 0; i < 2; ++ i)
            for (unsigned j = 0; j < 3; ++ j)
                BOOST_CHECK( vb(i, j) == v[i][j] );

   
    double sumb = 0;
    {
        using namespace boost::numeric::ublas;
        boost::numeric::ublas::matrix<double> mb4 (3, 3);
        
        for (unsigned i = 0; i < 3; ++ i)
            for (unsigned j = 0; j < 3; ++ j)
                mb4(i, j) = m4[i][j];
        
        
        
        {
            MEASURE_SCOPE_TIME( "UBlasDet" );
            for( boost::int64_t i=0;i < runs; ++i )
                sumb += b_determinant( mb4 );
        }
    }

    double sum = 0;
    //! Measure matrix det.
    {        
        double d;
        MEASURE_SCOPE_TIME( "GeometrixDet" );
        for( boost::int64_t i=0;i < runs; ++i )
        {
            d <<= det(m4);
            sum += d;
        }
    }

    BOOST_CHECK_CLOSE( sum, sumb, 1e-6 );
   
    
    //! Measure the compile-time access.
//     {
//         point_3 p( 69.0, 69.0, 69.0 );
//         double sum = 0;
//         MEASURE_SCOPE_TIME( "CompileTimeAccess_GGPoint" );
// 
//         for( boost::int64_t i=0;i < runs; ++i )
//         {
//             sum += get<0>( p );
//             sum += get<1>( p );
//             sum += get<2>( p );
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
//             sum += get<0>( p );
//             sum += get<1>( p );
//             sum += get<2>( p );
//         }
//     }
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
*/
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/adapted/boost_array.hpp>
BOOST_GEOMETRY_REGISTER_BOOST_ARRAY_CS(boost::geometry::cs::cartesian)

#include <geometrix/algorithm/euclidean_distance.hpp>
#include <geometrix/tensor/index_operator_vector_access_policy.hpp>
#include <geometrix/primitive/segment.hpp>

typedef boost::array<double,3> array3;
typedef boost::array<double,2> array2;
GEOMETRIX_INDEX_OPERATOR_FUSION_SEQUENCE( array3, double, 3 );
GEOMETRIX_INDEX_OPERATOR_FUSION_SEQUENCE( array2, double, 2 );

template <>
struct geometrix::construction_policy< array3 >
{    
    static boost::array<double,3> construct( const double& x, const double& y, const double& z )
    {
        array3 a = {x,y,z};
        return a;
    }

    template <typename NumericSequence>
    static boost::array<double,3> construct( const NumericSequence& args )
    {
        array3 a = { geometrix::get<0>( args ),
                     geometrix::get<1>( args ),
                     geometrix::get<2>( args ) };
        return a;
    }
};

template <>
struct geometrix::construction_policy< array2 >
{    
    static array2 construct( const double& x, const double& y )
    {
        array2 a = {x,y};
        return a;
    }

    template <typename NumericSequence>
    static array2 construct( const NumericSequence& args )
    {
        array2 a = { geometrix::get<0>( args ),
                     geometrix::get<1>( args ) };
        return a;
    }
};

GEOMETRIX_DEFINE_VECTOR_TRAITS( array3, (double), 3, double, neutral_reference_frame_3d, index_operator_vector_access_policy< array3 > );
GEOMETRIX_DEFINE_VECTOR_TRAITS( array2, (double), 2, double, neutral_reference_frame_3d, index_operator_vector_access_policy< array2 > );

BOOST_AUTO_TEST_CASE( GeometryTimeTests )
{
    using namespace geometrix;
    using namespace geometrix::algebra;
    
    boost::int64_t runs = 100000000;

    boost::array<double,3> a = { 1., 1., 1. };
    boost::array<double,3> b = { 3., 3., 3. };
    /*  
    double sum1 = 0;
    {
        MEASURE_SCOPE_TIME( "P2PGeometry" );
        for( boost::int64_t i=0;i < runs; ++i )
            sum1 += boost::geometry::distance(a,b);
    }
  
    double sum2 = 0;
    {
        MEASURE_SCOPE_TIME( "P2PGeometrix" );
        for( boost::int64_t i=0;i < runs; ++i )
            sum2 += point_point_distance(a,b);
    }
*/
    boost::array<double,3> c = { 5., 5., 5. };
    boost::array<double,3> d = { 10., 10., 10. };
    boost::geometry::model::segment<array3> s1(a,b), s2(c,d);
    /*
    double sum1 = 0;
    {
        MEASURE_SCOPE_TIME( "P2PGeometry" );
        for( boost::int64_t i=0;i < runs; ++i )
            sum1 += boost::geometry::distance(s1,s2);
    }
    */

    array3 a1 = { 0., 1., 1. };
    array3 a2 = { 0., 2., 1. };

    array3 b1 = { -1., 2.5, 1. };
    array3 b2 = { 1., 2.5, 1. };

    segment<array3> gs1(a1,a2), gs2(b1,b2);
    double sum2 = 0;
    {
        //MEASURE_SCOPE_TIME( "P2PGeometrix" );
        //for( boost::int64_t i=0;i < runs; ++i )
            sum2 += segment_segment_distance(gs1,gs2);
    }

    array2 a3 = { 0., 1. };
    array2 a4 = { 0., 2. };

    array2 b3 = { -1., 2.5 };
    array2 b4 = { 1., 2.5 };

    segment<array2> gs3(a3,a4), gs4(b3,b4);
    double sum3 = 0;
    {
        //MEASURE_SCOPE_TIME( "P2PGeometrix" );
        //for( boost::int64_t i=0;i < runs; ++i )
            sum3 += segment_segment_distance(gs3,gs4);
    }

    //BOOST_CHECK_CLOSE(sum1,sum2,1e-10);
}
#endif //GEOMETRIX_LINEAR_ALGEBRA_TIME_TESTS_HPP
