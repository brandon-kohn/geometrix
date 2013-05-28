//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_REFERENCE_FRAME_TESTS_HPP
#define GEOMETRIX_REFERENCE_FRAME_TESTS_HPP

#include <boost/test/unit_test.hpp>

#include <geometrix/primitive/point.hpp>
#include <geometrix/utility/utilities.hpp>

#include <geometrix/space/cartesian_reference_frame.hpp>
#include <geometrix/space/polar_reference_frame.hpp>
#include <geometrix/space/cartesian_polar_transformation.hpp>
#include <geometrix/space/polar_cartesian_transformation.hpp>

#include <geometrix/tensor/index_operator_vector_access_policy.hpp>
#include <geometrix/utility/member_function_fusion_adaptor.hpp>

#include <geometrix/tensor/numeric_sequence.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/algebra/algebra.hpp>

#include "tuple_kernal.hpp"
#include "vector_kernal.hpp"

typedef boost::array<double,3> array3;
typedef boost::array<double,2> array2;
GEOMETRIX_INDEX_OPERATOR_FUSION_SEQUENCE( array3, double, 3 );
GEOMETRIX_INDEX_OPERATOR_FUSION_SEQUENCE( array2, double, 2 );
GEOMETRIX_DEFINE_VECTOR_TRAITS( array3, (double), 3, double, neutral_reference_frame_3d, index_operator_vector_access_policy< array3 > );
GEOMETRIX_DEFINE_VECTOR_TRAITS( array2, (double), 2, double, neutral_reference_frame_3d, index_operator_vector_access_policy< array2 > );

BOOST_AUTO_TEST_CASE( TestCartesianReferenceFrame )
{
	using namespace geometrix;

	typedef point_double_2d                                         point_2d;
	typedef point_double_3d                                         point_3d;
    typedef cartesian_reference_frame_2d                            cartesian_frame_2d;
    typedef cartesian_reference_frame_3d                            cartesian_frame_3d;
    typedef reference_frame_adaptor< point_2d, cartesian_frame_2d > cartesian_point_2d;
    typedef reference_frame_adaptor< point_3d, cartesian_frame_3d > cartesian_point_3d;
    typedef polar_reference_frame_2d                                polar_frame_2d;
    typedef polar_reference_frame_3d                                polar_frame_3d;
    typedef reference_frame_adaptor< point_2d, polar_frame_2d >     polar_point_2d;
    typedef reference_frame_adaptor< point_3d, polar_frame_3d >     polar_point_3d;

    std::cout << sizeof( point_2d ) << std::endl;
    std::cout << sizeof( point_3d ) << std::endl;
    std::cout << sizeof( cartesian_point_2d ) << std::endl;
    std::cout << sizeof( cartesian_point_3d ) << std::endl;
    std::cout << sizeof( polar_point_2d ) << std::endl;
    std::cout << sizeof( polar_point_3d ) << std::endl;

//     BOOST_STATIC_ASSERT( is_point< cartesian_point_2d >::value );
//     BOOST_STATIC_ASSERT( is_point< polar_point_2d >::value );
// 
//     BOOST_STATIC_ASSERT( is_coordinate_sequence< cartesian_point_2d >::value );
//     BOOST_STATIC_ASSERT( is_coordinate_sequence< polar_point_2d >::value );
// 
//     BOOST_STATIC_ASSERT( is_numeric_sequence< cartesian_point_2d >::value );
//     BOOST_STATIC_ASSERT( is_numeric_sequence< polar_point_2d >::value );
// 
//     BOOST_STATIC_ASSERT( is_sequence< cartesian_point_2d >::value );
//     BOOST_STATIC_ASSERT( is_sequence< polar_point_2d >::value );
// 
//     BOOST_STATIC_ASSERT( !is_vector< cartesian_point_2d >::value );
//     BOOST_STATIC_ASSERT( !is_vector< polar_point_2d >::value );

    fraction_tolerance_comparison_policy<double> compare(1e-10);
    cartesian_point_2d cPoint( point_double_2d( 0., 1. ) );
    polar_point_2d pPoint( point_double_2d( 1., constants< double >::pi() / 2. ) );

    cartesian_point_2d cPoint2( pPoint );
    BOOST_CHECK( compare.equals( cPoint2.get<0>(), 0. ) );
    BOOST_CHECK( compare.equals( cPoint2.get<1>(), 1. ) );

    polar_point_2d pPoint2( cPoint );
    BOOST_CHECK( compare.equals( pPoint2.get<0>(), 1. ) );
    BOOST_CHECK( compare.equals( pPoint2.get<1>(), constants< double >::pi() / 2. ) );

    //! Construct from a neutral point.
    point_2d r2d (69.0, 69.0);
    cartesian_point_2d cPoint3( r2d );
    BOOST_CHECK( compare.equals( cPoint3.get<0>(), 69.0 ) );
    BOOST_CHECK( compare.equals( cPoint3.get<1>(), 69.0 ) );

    polar_point_2d pPoint3( r2d );
    BOOST_CHECK( compare.equals( pPoint3.get<0>(), 69.0 ) );
    BOOST_CHECK( compare.equals( pPoint3.get<1>(), 69.0 ) );

}

#endif //GEOMETRIX_REFERENCE_FRAME_TESTS_HPP
