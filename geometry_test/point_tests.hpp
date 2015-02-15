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


#include <boost/test/included/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/typeof/typeof.hpp>
#include <geometrix/tensor/vector_traits.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/utility/utilities.hpp>
#include <geometrix/space/cartesian_access_traits.hpp>
#include <geometrix/space/polar_access_traits.hpp>
#include <geometrix/utility/member_function_fusion_adaptor.hpp>

BOOST_AUTO_TEST_CASE( TestPointOperators )
{
    using namespace geometrix;

    typedef point_double_2d point_2d;
    typedef point_double_3d point_3d;

    typedef cartesian_access_traits< point_2d > cartesian_access_2d;
    typedef cartesian_access_traits< point_3d > cartesian_access_3d;

    BOOST_CONCEPT_ASSERT((CartesianCoordinateAccessorConcept< cartesian_access_2d >));
    BOOST_CONCEPT_ASSERT((CartesianCoordinateAccessorConcept< cartesian_access_3d >));

    point_2d p( 1, 2 );

    //double z = p.get< -3 >();

    BOOST_CHECK( geometric_traits< point_2d >::dimension_type::value == 2 );
    BOOST_CHECK( geometric_traits< point_3d >::dimension_type::value == 3 );

    //! Shouldn't compile as the point is 2d.
    //p(Z);

    //! Shouldn't compile
    //typedef point_template<double,4> point_4d;
    //point_4d p4;

    typedef geometrix::point_double_2d point_2d;
    point_2d a( 1., 1. );
    point_2d b( 1., 1. );

    //Test polar access traits    
    typedef polar_access_traits< point_2d > polar_access_2d;    
    BOOST_CONCEPT_ASSERT((PolarCoordinateAccessorConcept< polar_access_2d >));

    typedef reference_frame_adaptor< point_2d, polar_reference_frame_2d > polar_point2d;
    typedef reference_frame_adaptor< point_3d, polar_reference_frame_3d > polar_point3d;
    BOOST_AUTO(r, get_r( a ));
    BOOST_AUTO(t, get_theta( a ));

    typedef geometrix::point_double_3d point_3d;
    typedef polar_access_traits< point_3d > polar_access_3d;
    BOOST_CONCEPT_ASSERT((PolarCoordinateAccessorConcept< polar_access_3d >));
    point_3d d( 1., 1., 1. );
    BOOST_AUTO(r2, get_r( d ));
    BOOST_AUTO(t2, get_theta( d ));
    BOOST_AUTO(phi, get_phi( d ));
}

#endif //GEOMETRIX_POINT_TESTS_HPP
