//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GENERATIVE_GEOMETRY_POINT_TESTS_HPP
#define GENERATIVE_GEOMETRY_POINT_TESTS_HPP
#pragma once

#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include "../geometry/sequence_traits.hpp"
#include "../geometry/point.hpp"
#include "../geometry/utilities.hpp"
#include "../geometry/cartesian_access_traits.hpp"
#include "../geometry/polar_access_traits.hpp"
#include "../geometry/detail/member_function_fusion_adaptor.hpp"

struct pointXYZ
{
    double coords[3];

    template <unsigned int Index>
    double&          get() { return coords[Index]; }
    template <unsigned int Index>
    const double&    get() const { return coords[Index]; }
};

GENERATIVE_GEOMETRY_DEFINE_USER_POINT_TRAITS( pointXYZ,
                                double,
                                3,
                                generative::numeric::geometry::cartesian_reference_frame_double_3d,
                                generative::numeric::geometry::require_compile_time_access_policy );

struct point2XYZ
{    
    double&          get_x() { return coords[0]; }
    double&          get_y() { return coords[1]; }
    double&          get_z() { return coords[2]; }

    const double&    get_x() const { return coords[0]; }
    const double&    get_y() const { return coords[1]; }
    const double&    get_z() const { return coords[2]; }

    double coords[3];
};

GENERATIVE_GEOMETRY_MEMBER_FUNCTION_FUSION_SEQUENCE( point2XYZ, 
                                               (double, get_x)
                                               (double, get_y)
                                               (double, get_z)
                                               );

BOOST_AUTO_TEST_CASE( TestPointOperators )
{
    using namespace generative::numeric::geometry;

    typedef point_double_2d point_2d;
    typedef point_double_3d point_3d;

    typedef cartesian_access_traits< point_2d > cartesian_access_2d;
    typedef cartesian_access_traits< point_3d > cartesian_access_3d;

    boost::function_requires< CartesianCoordinateAccessorConcept< cartesian_access_2d > >();
    boost::function_requires< CartesianCoordinateAccessorConcept< cartesian_access_3d > >();

    point_2d p( 1, 2 );

    //double z = p.get< -3 >();

    BOOST_CHECK( point_traits< point_2d >::dimension_type::value == 2 );
    BOOST_CHECK( point_traits< point_3d >::dimension_type::value == 3 );

    //! Shouldn't compile as the point is 2D.
    //p(Z);

    //! Shouldn't compile
    //typedef point_template<double,4> point_4d;
    //point_4d p4;

    typedef generative::numeric::geometry::point_double_2d point_2d;
    point_2d a( 1., 1. );
    point_2d b( 1., 1. );

    //Test polar access traits    
    typedef polar_access_traits< point_2d > polar_access_2d;    
    boost::function_requires< PolarCoordinateAccessorConcept< polar_access_2d > >();

    double r = polar_access_2d::get<0>( a );
    double t = polar_access_2d::get<1>( a );

    typedef generative::numeric::geometry::point_double_3d point_3d;
    typedef polar_access_traits< point_3d > polar_access_3d;
    boost::function_requires< PolarCoordinateAccessorConcept< polar_access_3d > >();
    point_3d d( 1., 1., 1. );
    double r2 = polar_access_3d::get<0>( d );
    double t2 = polar_access_3d::get<1>( d );
    double phi = polar_access_3d::get<2>( d );
}

#endif //GENERATIVE_GEOMETRY_POINT_TESTS_HPP
