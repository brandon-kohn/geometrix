//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GENERATIVE_GEOMETRY_POINT_EXAMPLE_HPP
#define GENERATIVE_GEOMETRY_POINT_EXAMPLE_HPP
#pragma once

#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include "../geometry/sequence_traits.hpp"
#include "../geometry/point.hpp"
#include "../geometry/utilities.hpp"
#include "../geometry/cartesian_access_traits.hpp"
#include "../geometry/polar_access_traits.hpp"
#include "../geometry/detail/member_function_fusion_adaptor.hpp"

using namespace generative::numeric::geometry;

// A simple point structure to model a 3D point with type double.
struct point3D
{
    point3D( double x, double y, double z )
    {
        coords[0] = x;
        coords[1] = y;
        coords[2] = z;
    }

    double get_x() const { return coords[0]; }
    double get_y() const { return coords[1]; }
    double get_z() const { return coords[2]; }

    double coords[3];    
};

// Adapt the struct into a fusion sequence to provide a compile time access interface.
GENERATIVE_GEOMETRY_MEMBER_FUNCTION_FUSION_SEQUENCE
(
    point3D, 
    (double, get_x)
    (double, get_y)
    (double, get_z)
);

// Register the simple struct into a GGA enabled point type with 
// a Cartesian reference frame and a preference for compile time access semantics.
GENERATIVE_GEOMETRY_DEFINE_USER_POINT_TRAITS
(
     point3D,                             // The real type
     double,                              // The underlying coordinate type
     3,                                   // The dimensionality of the point type
     cartesian_reference_frame_double_3d, // The default reference frame
     require_compile_time_access_policy   // The preferred index access policy
);

// Define the policy for constructing a point3D from raw coordinates and numeric sequence types.
// This construction policy allows the user to specialize the way their object
// must be created.
template <>
struct generative::numeric::geometry::construction_traits< point3D >
{    
    // Construct from individual coordinates.
    static inline point3D construct( const double& x, const double& y, const double& z )
    {
        return point3D( x, y, z );
    }

    // Construct from an instance that models a numeric_sequence.
    template <typename NumericSequence>
    static inline point3D construct( const NumericSequence& args )
    {
        using namespace generative::numeric::geometry;
        boost::function_requires< NumericSequenceConcept< NumericSequence > >();

        return construct( indexed_access_traits<NumericSequence>::get<0>( args ),
                          indexed_access_traits<NumericSequence>::get<1>( args ),
                          indexed_access_traits<NumericSequence>::get<2>( args ) );
    }
};

BOOST_AUTO_TEST_CASE( PointExample )
{
    // Algorithms in GGA use construction_traits to build objects.
    point3D p( construction_traits< point3D >::construct( 1.0, 2.0, 3.0 ) );
    
    // Type system has registered the type characteristics for use in library algorithms.
    BOOST_ASSERT( point_traits< point3D >::dimension_type::value == 3 );
    BOOST_ASSERT( (boost::is_same< point_traits< point3D >::coordinate_type, double >::value) );
    BOOST_ASSERT( is_point< point3D >::value );

    // Access the magnitude value of the point in each dimension.
    BOOST_ASSERT( indexed_access_traits< point3D >::get<0>( p ) == 1.0 );
    BOOST_ASSERT( indexed_access_traits< point3D >::get<1>( p ) == 2.0 );
    BOOST_ASSERT( indexed_access_traits< point3D >::get<2>( p ) == 3.0 );
    
    // Access Cartesian frame coordinates.
    BOOST_ASSERT( cartesian_access_traits< point3D >::get<0>( p ) == 1.0 );
    BOOST_ASSERT( cartesian_access_traits< point3D >::get<1>( p ) == 2.0 );
    BOOST_ASSERT( cartesian_access_traits< point3D >::get<2>( p ) == 3.0 );
    
    // Access the values in polar coordinates
    fraction_tolerance_comparison_policy<double> compare( 1.0e-10 ); //Create a comparison functor to compare within 1e-10 tolerance.
    BOOST_ASSERT( compare.equals( polar_access_traits< point3D >::get<0>( p ), sqrt( 14.0 ) ) );
    BOOST_ASSERT( compare.equals( polar_access_traits< point3D >::get<1>( p ), atan2( 2.0, 1.0 ) ) );
    BOOST_ASSERT( compare.equals( polar_access_traits< point3D >::get<2>( p ), acos( 3.0 / sqrt( 14.0 ) ) ) );   

    //! Check the reverse transformations
    typedef reference_frame_tag< point3D, polar_reference_frame_double_3d > polar_point3D;
    polar_point3D p2( construction_traits< point3D >::construct( polar_access_traits< point3D >::get<0>( p ),
                                                                 polar_access_traits< point3D >::get<1>( p ),
                                                                 polar_access_traits< point3D >::get<2>( p ) ) );

    BOOST_ASSERT( compare.equals( cartesian_access_traits< polar_point3D >::get<0>( p2 ), 1.0 ) );
    BOOST_ASSERT( compare.equals( cartesian_access_traits< polar_point3D >::get<1>( p2 ), 2.0 ) );
    BOOST_ASSERT( compare.equals( cartesian_access_traits< polar_point3D >::get<2>( p2 ), 3.0 ) );   
}

#endif //GENERATIVE_GEOMETRY_POINT_EXAMPLE_HPP
