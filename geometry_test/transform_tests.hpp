//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_TRANSFORM_TESTS_HPP
#define GEOMETRIX_TRANSFORM_TESTS_HPP

#include <boost/test/included/unit_test.hpp>
#include <geometrix/numeric/number_comparison_policy.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/utility/utilities.hpp>
#include <geometrix/algebra/algebra.hpp>
#include <geometrix/arithmetic/arithmetic_promotion_policy.hpp>
#include <geometrix/tensor/fusion_matrix.hpp>
#include <geometrix/tensor/fusion_vector.hpp>
#include <geometrix/space/cartesian_reference_frame.hpp>

#include <iostream>

BOOST_AUTO_TEST_CASE( TestTransforms )
{
    using namespace geometrix;
    
    BOOST_STATIC_ASSERT(( boost::mpl::equal<result_of::multiplies<int,double>::type,double>::value ));
    BOOST_STATIC_ASSERT(( boost::mpl::equal<result_of::divides<int,double>::type,double>::value ));
    BOOST_STATIC_ASSERT(( boost::mpl::equal<result_of::plus<int,double>::type,double>::value ));
    BOOST_STATIC_ASSERT(( boost::mpl::equal<result_of::minus<int,double>::type,double>::value ));

    BOOST_STATIC_ASSERT(( boost::mpl::equal<result_of::multiplies<double, int>::type,double>::value ));
    BOOST_STATIC_ASSERT(( boost::mpl::equal<result_of::divides<double, int>::type,double>::value ));
    BOOST_STATIC_ASSERT(( boost::mpl::equal<result_of::plus<double, int>::type,double>::value ));
    BOOST_STATIC_ASSERT(( boost::mpl::equal<result_of::minus<double, int>::type,double>::value ));
    
    typedef boost::fusion::vector<int, double, char> xtypes;
    typedef boost::fusion::vector<int, char, float> xtypes2;
    typedef result_of::cross_product<xtypes, xtypes2>::type ctypes1;
    std::cout << typeid(ctypes1).name() << std::endl;
    BOOST_STATIC_ASSERT(( boost::mpl::equal<ctypes1, boost::mpl::vector<double, float, double> >::value ));

    typedef boost::mpl::transform_view
                    <
                        boost::mpl::zip_view< boost::mpl::vector<xtypes,xtypes2> >
                      , boost::mpl::unpack_args< result_of::minus<boost::mpl::_1,boost::mpl::_2> >
                    > mtypes;
    BOOST_STATIC_ASSERT(( boost::mpl::equal<mtypes, boost::mpl::vector<int, double, float> >::value ));

    typedef result_of::matrix_product< matrix<int,4,3>, matrix<double,3,4> >::type m2t;
    //std::cout << typeid(m2t).name() << std::endl;

    typedef result_of::determinant< matrix<int,4,4> >::type m3t;
    //std::cout << typeid(m3t).name() << std::endl << std::endl;

    typedef result_of::minus< vector_int_2d, vector_int_2d >::type minusv;
    std::cout << typeid(minusv).name() << std::endl << std::endl;

    typedef result_of::divides< vector_int_2d, double >::type mdd;
    std::cout << typeid(mdd).name() << std::endl << std::endl;
    std::cout << typeid(boost::mpl::at_c<mdd,0>::type).name() << std::endl;

    using namespace geometrix::algebra;
    vector_int_2d a(4, 6);

    vector_double_2d r; 
    r <<=a + a/2.0;

    //BOOST_STATIC_ASSERT(( boost::is_same<boost::fusion::result_of::at_c<mdd,0>::type, double>::value ));
    
}

GEOMETRIX_FUSION_MATRIX
(
    test_matrix
  , ((int, float, double))
    ((char, bool, short))
  , 2
  , 3
)

GEOMETRIX_FUSION_MATRIX
(
    test_matrix2
  , ((int, float))
    ((char, bool))
    ((long, short))
  , 3
  , 2
)

//#pragma message( BOOST_PP_STRINGIZE( ( GEOMETRIX_FUSION_MATRIX( test_matrix, ((int, float, double))((char, bool, short)), 2, 3, double ) ) ) )
//#pragma message( BOOST_PP_STRINGIZE( ( GEOMETRIX_FUSION_MATRIX( test_matrix2, ((int, float))((char, bool))((long, short)), 3, 2, double ) ) ) )

GEOMETRIX_FUSION_POD_MATRIX( test, ((int, float))((char, bool))((long, short)), 3, 2 );

GEOMETRIX_FUSION_POD_MATRIX
(
    function_tensor
  , ((geometrix::algebra::abs, geometrix::algebra::sin))
    ((geometrix::algebra::cos, geometrix::algebra::tan))
  , 2
  , 2
);

BOOST_AUTO_TEST_CASE( TestFusionMatrix )
{
    using namespace geometrix;
    using namespace geometrix::result_of;
    using namespace geometrix::algebra;

    typedef result_of::matrix_product< test_matrix, test_matrix2 >::type m2t;
    std::cout << typeid(m2t).name() << std::endl;

    test m = { {0, 1}, 
               {2, 3},
               {4, 5} };

    std::cout << get<0,0>(m) << std::endl;
    std::cout << get<0,1>(m) << std::endl;
    std::cout << get<1,0>(m) << std::endl;
    std::cout << get<1,1>(m) << std::endl;
    std::cout << get<2,0>(m) << std::endl;
    std::cout << get<2,1>(m) << std::endl;
    set<2,1>(m, 7);
    //std::cout << get<2,1>(m) << std::endl;

    function_tensor ft;

    vector_double_2d v( -69., 22./7. );
    v <<= ft * v;
    
    matrix<double,2,2> m2 = {{ {-69., 22./7.  },
                               {  0., 22./28. } }};

    m2 <<= m2 * ft;
}

GEOMETRIX_FUSION_POD_VECTOR
(
    test_vector
  , (double)(char)(int)
  , geometrix::cartesian_reference_frame_3d
);

typedef geometrix::cartesian_reference_frame_3d::coordinate_type_at<double,0>::type x_coord;
typedef geometrix::cartesian_reference_frame_3d::coordinate_type_at<double,1>::type y_coord;
typedef geometrix::cartesian_reference_frame_3d::coordinate_type_at<double,2>::type z_coord;

GEOMETRIX_FUSION_POD_VECTOR
(
    coordinate_vector
  , (x_coord)
    (y_coord)
    (z_coord)
  , geometrix::cartesian_reference_frame_3d
);

typedef boost::units::quantity<boost::units::unit<boost::units::area_dimension,boost::units::si::system>, double> area_c;

GEOMETRIX_FUSION_POD_VECTOR
(
    pseudovector
  , (area_c)
    (area_c)
    (area_c)
  , geometrix::cartesian_reference_frame_3d
);

//#pragma message( BOOST_PP_STRINGIZE( ( GEOMETRIX_FUSION_POD_VECTOR(test_vector, (double)(char)(int), double, cartesian_reference_frame_3d) ) ) )

BOOST_AUTO_TEST_CASE( TestFusionVector )
{
    using namespace geometrix;
    using namespace geometrix::result_of;
    using namespace geometrix::algebra;
    typedef result_of::cross_product<test_vector, test_vector>::type m2t;
    std::cout << typeid(m2t).name() << std::endl;
    test_vector m = { 0, 1, 2 };

    std::cout << get<0>(m) << std::endl;
    std::cout << get<1>(m) << std::endl;
    std::cout << get<2>(m) << std::endl;
    set<0>(m, 7);
    std::cout << get<0>(m) << std::endl;

    coordinate_vector v = { 0 * cartesian_reference_frame_3d::basis<0>::unit_type()
                          , 1 * cartesian_reference_frame_3d::basis<1>::unit_type()
                          , 2 * cartesian_reference_frame_3d::basis<2>::unit_type() };

    BOOST_STATIC_ASSERT(( is_heterogeneous<coordinate_vector>::value ));
    typedef result_of::dot_product< coordinate_vector, coordinate_vector >::type coordinate_dot;
    std::cout << typeid(coordinate_dot).name() << std::endl;
    typedef type_at<BOOST_TYPEOF(v*v)>::type eval_type;
    std::cout << std::endl;
    std::cout << typeid(eval_type).name() << std::endl;
    coordinate_dot l;l <<= v * v;

    typedef result_of::cross_product< coordinate_vector, coordinate_vector >::type coordinate_cross;
    pseudovector vp;vp <<= v ^ v;
    
}

BOOST_AUTO_TEST_CASE( TestGeneralRotation )
{
    using namespace geometrix;
    using namespace geometrix::result_of;
    using namespace geometrix::algebra;

    vector_double_3d u(0., 0., 1.);
    vector_double_3d v(1., 0., 0.);
    point_double_3d p( 1., 0., 0. );
    double theta = geometrix::constants<double>::pi()/2.;
    p <<= expr_cast<point_double_3d>( std::cos(theta)*v + (1.0 - std::cos(theta))*(v*u)*u + std::sin(theta)*(u^v) );
    theta = geometrix::constants<double>::pi();
    p <<= expr_cast<point_double_3d>( std::cos(theta)*v + (1.0 - std::cos(theta))*(v*u)*u + std::sin(theta)*(u^v) );
    theta = 3.* geometrix::constants<double>::pi() / 2.;
    p <<= expr_cast<point_double_3d>( std::cos(theta)*v + (1.0 - std::cos(theta))*(v*u)*u + std::sin(theta)*(u^v) );
    
    vector_double_4d vp(1., 0., 0., 0.);
    vector_double_4d q(0., 0., 0., 1.);q <<= std::cos(theta/2.) * q + std::sin( theta / 2. ) * vp;
    vector_double_4d q_(0., 0., 0., 1.);q_ <<= std::cos(theta/2.) * q_ - std::sin( theta / 2. ) * vp;

    p <<= expr_cast<point_double_3d>( q*vp*q_ );

}

#endif //GEOMETRIX_TRANSFORM_TESTS_HPP
