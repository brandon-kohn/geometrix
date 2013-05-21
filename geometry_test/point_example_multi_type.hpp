//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_POINT_EXAMPLE_MULTI_TYPE_HPP
#define GEOMETRIX_POINT_EXAMPLE_MULTI_TYPE_HPP

#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <geometrix/tensor/vector_traits.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/utility/utilities.hpp>
#include <geometrix/space/cartesian_access_traits.hpp>
#include <geometrix/space/polar_access_traits.hpp>
#include <geometrix/space/cartesian_polar_transformation.hpp>
#include <geometrix/space/polar_cartesian_transformation.hpp>
#include <geometrix/utility/member_function_fusion_adaptor.hpp>

using namespace geometrix;

// A simple point structure to model a 3d point with type double.
struct CompileTimeCartesianPointM3d
{
    CompileTimeCartesianPointM3d( float x, int y, short z )
        : x( x )
        , y( y )
        , z( z )
    {}

    float  get_x() const { return x; }
    int    get_y() const { return y; }
    short  get_z() const { return z; }

    void   set_x( const float& v ){ x=v; }
    void   set_y( const int& v ){ y=v; }
    void   set_z( const short& v ){ z=v; }

    float  x;
    int    y;
    short  z;
};

// Adapt the struct into a fusion sequence to provide a compile time access interface.
GEOMETRIX_MEMBER_FUNCTION_FUSION_SEQUENCE
(
    CompileTimeCartesianPointM3d, 
    (float, float, get_x, set_x)
    (int, int, get_y, set_y)
    (short, short, get_z, set_z)
);

// Register the simple struct into a GGA enabled point type with 
// a Cartesian reference frame and a preference for compile time access semantics.
GEOMETRIX_DEFINE_POINT_TRAITS
(
    CompileTimeCartesianPointM3d,                      // The real type
    (float)(int)(short),                               // The underlying coordinate types
    3,                                                 // The tensor_order of the point type
    double,                                            // The arithmetic type to use in calculations
    cartesian_reference_frame_3d,                      // The default reference frame
    fusion_vector_member_function_access_policy<CompileTimeCartesianPointM3d> // The preferred index access policy
);

// Define the policy for constructing a CompileTimeCartesianPointM3d from raw coordinates and numeric sequence types.
// This construction policy allows the user to specialize the way their object
// must be created. This macro can be used on types with default constructors for each coordinate.
// For example:
// \code
// struct Point3d
// {
//     Point3d( float x, int y, short z )
//       : x( x )
//       , y( y )
//       , z( z )
//     {}
//
//     float x;
//     int   y;
//     short z;
// };
// \endcode
GEOMETRIX_DEFINE_NUMERIC_SEQUENCE_CONSTRUCTION_POLICY( CompileTimeCartesianPointM3d, 3 );

template <typename Point>
void TestCompileTimePointM()
{
    // Algorithms in GGA use construction_policy to build objects.
    Point p( construct<Point>( 1.0f, 2, 3 ) );

    // Type system has registered the type characteristics for use in library algorithms.
    BOOST_CHECK( geometric_traits< Point >::dimension_type::value == 3 );
    BOOST_CHECK( (boost::is_same< typename type_at< Point, 0 >::type, float >::value) );
    BOOST_CHECK( (boost::is_same< typename type_at< Point, 1 >::type, int >::value) );
    BOOST_CHECK( (boost::is_same< typename type_at< Point, 2 >::type, short >::value) );
    BOOST_CHECK( is_heterogeneous< Point >::value );
    BOOST_CHECK( !is_homogeneous< Point >::value );
    BOOST_CHECK( is_point< Point >::value );

    // Access the magnitude value of the point in each dimension.
    BOOST_CHECK( get<0>( p ) == 1.0f );
    BOOST_CHECK( get<1>( p ) == 2 );
    BOOST_CHECK( get<2>( p ) == 3 );

    // Access Cartesian frame coordinates.
    BOOST_CHECK( cartesian_access_traits< Point >::get<0>( p ) == 1.0f * boost::units::si::length() );
    BOOST_CHECK( cartesian_access_traits< Point >::get<1>( p ) == 2 * boost::units::si::length() );
    BOOST_CHECK( cartesian_access_traits< Point >::get<2>( p ) == 3 * boost::units::si::length() );

    // Access the values in polar coordinates (loses precision...)
    fraction_tolerance_comparison_policy<float> compare( 1.0e-5f ); //Create a comparison functor to compare within 1e-10 tolerance.
    direct_comparison_policy dcompare;
    BOOST_CHECK( compare.equals( polar_access_traits< Point >::get<0>( p ), boost::numeric_cast< type_at< Point, 0 >::type >( sqrt( 14.0 ) ) * boost::units::si::length() ) );
    BOOST_CHECK( dcompare.equals( polar_access_traits< Point >::get<1>( p ), boost::numeric_cast< type_at< Point, 1 >::type >( atan2( 2.0, 1.0 ) ) * boost::units::si::plane_angle() ) );
    BOOST_CHECK( dcompare.equals( polar_access_traits< Point >::get<2>( p ), boost::numeric_cast< type_at< Point, 2 >::type >( acos( 3.0 / sqrt( 14.0 ) ) ) * boost::units::si::plane_angle() ) );   
}

BOOST_AUTO_TEST_CASE( TestMultiTypePoints )
{
    TestCompileTimePointM< CompileTimeCartesianPointM3d >();
}

#endif //GEOMETRIX_POINT_EXAMPLE_MULTI_TYPE_HPP
