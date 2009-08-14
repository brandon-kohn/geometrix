//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GENERATIVE_GEOMETRY_REFERENCE_FRAME_TESTS_HPP
#define GENERATIVE_GEOMETRY_REFERENCE_FRAME_TESTS_HPP
#pragma once

#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include "../geometry/point.hpp"
#include "../geometry/utilities.hpp"
#include "../geometry/cartesian_reference_frame.hpp"
#include "../geometry/polar_reference_frame.hpp"
#include "../geometry/cartesian_polar_transformation.hpp"
#include "../geometry/polar_cartesian_transformation.hpp"

BOOST_AUTO_TEST_CASE( TestCartesianReferenceFrame )
{
	using namespace generative::numeric::geometry;

	typedef point_double_2d                                     point_2d;
	typedef point_double_3d                                     point_3d;
    typedef cartesian_reference_frame_double_2d                 cartesian_frame_2d;
    typedef cartesian_reference_frame_double_3d                 cartesian_frame_3d;
    typedef reference_frame_tag< point_2d, cartesian_frame_2d > cartesian_point_2d;
    typedef reference_frame_tag< point_3d, cartesian_frame_3d > cartesian_point_3d;
    typedef polar_reference_frame_double_2d                     polar_frame_2d;
    typedef polar_reference_frame_double_3d                     polar_frame_3d;
    typedef reference_frame_tag< point_2d, polar_frame_2d >     polar_point_2d;
    typedef reference_frame_tag< point_3d, polar_frame_3d >     polar_point_3d;

    std::cout << sizeof( point_2d ) << std::endl;
    std::cout << sizeof( point_3d ) << std::endl;
    std::cout << sizeof( cartesian_point_2d ) << std::endl;
    std::cout << sizeof( cartesian_point_3d ) << std::endl;
    std::cout << sizeof( polar_point_2d ) << std::endl;
    std::cout << sizeof( polar_point_3d ) << std::endl;

    BOOST_CHECK( is_point< cartesian_point_2d >::value );
    BOOST_CHECK( is_point< polar_point_2d >::value );

    BOOST_CHECK( is_coordinate_sequence< cartesian_point_2d >::value );
    BOOST_CHECK( is_coordinate_sequence< polar_point_2d >::value );

    BOOST_CHECK( is_numeric_sequence< cartesian_point_2d >::value );
    BOOST_CHECK( is_numeric_sequence< polar_point_2d >::value );

    BOOST_CHECK( is_sequence< cartesian_point_2d >::value );
    BOOST_CHECK( is_sequence< polar_point_2d >::value );

    BOOST_CHECK( !is_vector< cartesian_point_2d >::value );
    BOOST_CHECK( !is_vector< polar_point_2d >::value );

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
    point_2d r2D (69.0, 69.0);
    cartesian_point_2d cPoint3( r2D );
    BOOST_CHECK( compare.equals( cPoint3.get<0>(), 69.0 ) );
    BOOST_CHECK( compare.equals( cPoint3.get<1>(), 69.0 ) );

    polar_point_2d pPoint3( r2D );
    BOOST_CHECK( compare.equals( pPoint3.get<0>(), 69.0 ) );
    BOOST_CHECK( compare.equals( pPoint3.get<1>(), 69.0 ) );

}

#endif //GENERATIVE_GEOMETRY_REFERENCE_FRAME_TESTS_HPP
