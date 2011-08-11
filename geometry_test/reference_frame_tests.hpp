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
#include <boost/test/floating_point_comparison.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/utility/utilities.hpp>
#include <geometrix/space/cartesian_reference_frame.hpp>
#include <geometrix/space/polar_reference_frame.hpp>
#include <geometrix/space/cartesian_polar_transformation.hpp>
#include <geometrix/space/polar_cartesian_transformation.hpp>

BOOST_AUTO_TEST_CASE( TestCartesianReferenceFrame )
{
	using namespace geometrix;

	typedef point_double_2D                                     point_2D;
	typedef point_double_3D                                     point_3D;
    typedef cartesian_reference_frame_2D                        cartesian_frame_2D;
    typedef cartesian_reference_frame_3D                        cartesian_frame_3D;
    typedef reference_frame_adaptor< point_2D, cartesian_frame_2D > cartesian_point_2D;
    typedef reference_frame_adaptor< point_3D, cartesian_frame_3D > cartesian_point_3D;
    typedef polar_reference_frame_2D                            polar_frame_2D;
    typedef polar_reference_frame_3D                            polar_frame_3D;
    typedef reference_frame_adaptor< point_2D, polar_frame_2D >     polar_point_2D;
    typedef reference_frame_adaptor< point_3D, polar_frame_3D >     polar_point_3D;

    std::cout << sizeof( point_2D ) << std::endl;
    std::cout << sizeof( point_3D ) << std::endl;
    std::cout << sizeof( cartesian_point_2D ) << std::endl;
    std::cout << sizeof( cartesian_point_3D ) << std::endl;
    std::cout << sizeof( polar_point_2D ) << std::endl;
    std::cout << sizeof( polar_point_3D ) << std::endl;

    BOOST_CHECK( is_point< cartesian_point_2D >::value );
    BOOST_CHECK( is_point< polar_point_2D >::value );

    BOOST_CHECK( is_coordinate_sequence< cartesian_point_2D >::value );
    BOOST_CHECK( is_coordinate_sequence< polar_point_2D >::value );

    BOOST_CHECK( is_numeric_sequence< cartesian_point_2D >::value );
    BOOST_CHECK( is_numeric_sequence< polar_point_2D >::value );

    BOOST_CHECK( is_sequence< cartesian_point_2D >::value );
    BOOST_CHECK( is_sequence< polar_point_2D >::value );

    BOOST_CHECK( !is_vector< cartesian_point_2D >::value );
    BOOST_CHECK( !is_vector< polar_point_2D >::value );

    fraction_tolerance_comparison_policy<double> compare(1e-10);
    cartesian_point_2D cPoint( point_double_2D( 0., 1. ) );
    polar_point_2D pPoint( point_double_2D( 1., constants< double >::pi() / 2. ) );

    cartesian_point_2D cPoint2( pPoint );
    BOOST_CHECK( compare.equals( cPoint2.get<0>(), 0. ) );
    BOOST_CHECK( compare.equals( cPoint2.get<1>(), 1. ) );

    polar_point_2D pPoint2( cPoint );
    BOOST_CHECK( compare.equals( pPoint2.get<0>(), 1. ) );
    BOOST_CHECK( compare.equals( pPoint2.get<1>(), constants< double >::pi() / 2. ) );

    //! Construct from a neutral point.
    point_2D r2D (69.0, 69.0);
    cartesian_point_2D cPoint3( r2D );
    BOOST_CHECK( compare.equals( cPoint3.get<0>(), 69.0 ) );
    BOOST_CHECK( compare.equals( cPoint3.get<1>(), 69.0 ) );

    polar_point_2D pPoint3( r2D );
    BOOST_CHECK( compare.equals( pPoint3.get<0>(), 69.0 ) );
    BOOST_CHECK( compare.equals( pPoint3.get<1>(), 69.0 ) );

}

#endif //GEOMETRIX_REFERENCE_FRAME_TESTS_HPP
