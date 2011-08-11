//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_POINT_TESTS_HPP
#define GEOMETRIX_POINT_TESTS_HPP


#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <geometrix/tensor/vector_traits.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/utility/utilities.hpp>
#include <geometrix/space/cartesian_access_traits.hpp>
#include <geometrix/space/polar_access_traits.hpp>
#include <geometrix/utility/member_function_fusion_adaptor.hpp>

BOOST_AUTO_TEST_CASE( TestPointOperators )
{
    using namespace geometrix;

    typedef point_double_2D point_2D;
    typedef point_double_3D point_3D;

    typedef cartesian_access_traits< point_2D > cartesian_access_2D;
    typedef cartesian_access_traits< point_3D > cartesian_access_3D;

    boost::function_requires< CartesianCoordinateAccessorConcept< cartesian_access_2D > >();
    boost::function_requires< CartesianCoordinateAccessorConcept< cartesian_access_3D > >();

    point_2D p( 1, 2 );

    //double z = p.get< -3 >();

    BOOST_CHECK( geometric_traits< point_2D >::dimension_type::value == 2 );
    BOOST_CHECK( geometric_traits< point_3D >::dimension_type::value == 3 );

    //! Shouldn't compile as the point is 2D.
    //p(Z);

    //! Shouldn't compile
    //typedef point_template<double,4> point_4d;
    //point_4d p4;

    typedef geometrix::point_double_2D point_2D;
    point_2D a( 1., 1. );
    point_2D b( 1., 1. );

    //Test polar access traits    
    typedef polar_access_traits< point_2D > polar_access_2D;    
    boost::function_requires< PolarCoordinateAccessorConcept< polar_access_2D > >();

    typedef reference_frame_adaptor< point_2D, polar_reference_frame_2D > polar_point2D;
    typedef reference_frame_adaptor< point_3D, polar_reference_frame_3D > polar_point3D;
    coordinate_type_of<polar_point2D, e_radius>::type r = get_r( a );
    coordinate_type_of<polar_point2D, e_theta>::type t = get_theta( a );

    typedef geometrix::point_double_3D point_3D;
    typedef polar_access_traits< point_3D > polar_access_3D;
    boost::function_requires< PolarCoordinateAccessorConcept< polar_access_3D > >();
    point_3D d( 1., 1., 1. );
    coordinate_type_of<polar_point2D, e_radius>::type r2 = get_r( d );
    coordinate_type_of<polar_point2D, e_theta>::type t2 = get_theta( d );
    coordinate_type_of<polar_point2D, e_phi>::type phi = get_phi( d );
}

#endif //GEOMETRIX_POINT_TESTS_HPP
