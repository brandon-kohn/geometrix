//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_POINT_EXAMPLE_HPP
#define GEOMETRIX_POINT_EXAMPLE_HPP

#include <boost/test/included/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/units/physical_dimensions.hpp>
#include <boost/units/dimensionless_quantity.hpp>
#include <boost/units/io.hpp>
#include <boost/fusion/include/adapt_adt.hpp>

#include <geometrix/tensor/vector_traits.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/utility/utilities.hpp>
#include <geometrix/space/cartesian_access_traits.hpp>
#include <geometrix/space/polar_access_traits.hpp>
#include <geometrix/space/cartesian_polar_transformation.hpp>
#include <geometrix/space/polar_cartesian_transformation.hpp>
#include <geometrix/utility/member_function_fusion_adaptor.hpp>
#include <geometrix/tensor/fusion_vector_access_policy.hpp>

using namespace geometrix;

// A simple point structure to model a 3d point with type double.
struct CartesianPoint3D
{
    CartesianPoint3D( double x=0., double y=0., double z=0. )
    {
        coords[0] = x;
        coords[1] = y;
        coords[2] = z;
    }

    CartesianPoint3D( const CartesianPoint3D& p )        
    {
        coords[0] = p.coords[0];
        coords[1] = p.coords[1];
        coords[2] = p.coords[2];
        static int c = 0;
        std::cout << "Copying CartesianPoint3D: " << ++c << std::endl;
    }
        
    double& get_x() { return coords[0]; }
    double& get_y() { return coords[1]; }
    double& get_z() { return coords[2]; }

    const double& get_x() const { return coords[0]; }
    const double& get_y() const { return coords[1]; }
    const double& get_z() const { return coords[2]; }

    void set_x( const double& v ) { coords[0] = v; }
    void set_y( const double& v ) { coords[1] = v; }
    void set_z( const double& v ) { coords[2] = v; }

    double coords[3];    
};

// Adapt the struct into a fusion sequence to provide a compile time access interface.
GEOMETRIX_MEMBER_FUNCTION_FUSION_SEQUENCE
(
    CartesianPoint3D,
    (double&, const double&, get_x, set_x)
    (double&, const double&, get_y, set_y)
    (double&, const double&, get_z, set_z)
);

// Register the simple struct into a GGA enabled point type with 
// a Cartesian reference frame and a preference for compile time access semantics.
GEOMETRIX_DEFINE_POINT_TRAITS
(
     CartesianPoint3D,                      // The real type
     (double),                                         // The underlying coordinate type
     3,                                                // The tensor_order of the point type
	 double,                                           // Dimensionless type for calculations
     double,                                           // The arithmetic type to use in calculations
     cartesian_reference_frame_3d,                     // The default reference frame
     fusion_vector_member_function_access_policy<CartesianPoint3D> // The preferred index access policy
);

// Define the policy for constructing a CartesianPoint3D from raw coordinates and numeric sequence types.
// This construction policy allows the user to specialize the way their object
// must be created. This macro can be used on types with default constructors for each coordinate.
// For example:
// \code
// struct Point3d
// {
//     Point3d( double x, double y, double z )
//     {
//         coords[0] = x;
//         coords[1] = y;
//         coords[2] = z;
//     }
//
//     double coords[3];    
// };
// \endcode
//GEOMETRIX_DEFINE_NUMERIC_SEQUENCE_CONSTRUCTION_POLICY( CartesianPoint3D, 3 );
namespace geometrix 
{
    template <>
    struct construction_policy< CartesianPoint3D > 
    {
        template < typename A0 , typename A1 , typename A2 > 
        static CartesianPoint3D construct( const A0& a0
                                         , const A1& a1 
                                         , const A2& a2 ) 
        { return CartesianPoint3D( a0 , a1 , a2 ); } 
        
        
        template <typename NumericSequence>
        static CartesianPoint3D construct( const NumericSequence& args ) 
        {
            return CartesianPoint3D( geometrix::get<0>( args ) , geometrix::get<1>( args ) , geometrix::get<2>( args ) ); 
        }        
    }; 
} 

template <typename Point>
void TestPoint()
{
    // Algorithms in geometrix use construction_policy to build objects.
    Point p( construct<Point>( 1.0, 2.0, 3.0 ) );
    
    // Type system has registered the type characteristics for use in library algorithms.
    GEOMETRIX_STATIC_ASSERT( geometric_traits< Point >::dimension_type::value == 3 );
    GEOMETRIX_STATIC_ASSERT( (boost::is_same< typename type_at< Point, 0 >::type, double >::value) );
    GEOMETRIX_STATIC_ASSERT( is_point< Point >::value );

    // Access the magnitude value of the point in each dimension.
    BOOST_CHECK( get<0>( p ) == 1.0 );
    BOOST_CHECK( get<1>( p ) == 2.0 );
    BOOST_CHECK( get<2>( p ) == 3.0 );
    
    // Access Cartesian frame coordinates.
    BOOST_CHECK( get_x( p ) == 1.0 * boost::units::si::length() );
    BOOST_CHECK( get_y( p ) == 2.0 * boost::units::si::length() );
    BOOST_CHECK( get_z( p ) == 3.0 * boost::units::si::length() );
    
    // Access the values in polar coordinates
    fraction_tolerance_comparison_policy<double> compare( 1.0e-10 ); //Create a comparison functor to compare within 1e-10 tolerance.
    BOOST_CHECK( compare.equals( get_r( p ), sqrt( 14.0 ) * boost::units::si::length() ) );
    BOOST_CHECK( compare.equals( get_theta( p ), atan2( 2.0, 1.0 ) * boost::units::si::plane_angle() ) );
    BOOST_CHECK( compare.equals( get_phi( p ), acos( 3.0 / sqrt( 14.0 ) ) * boost::units::si::plane_angle() ) );   

    // Check the reverse transformations

    // Use the reference frame tag type to mark a point type as being transformed into another frame. 
    typedef reference_frame_adaptor< Point, polar_reference_frame_3d > polar_point3d;

    // Construct a point with the polar coordinates.
    polar_point3d p2( construct<Point>( boost::units::quantity_cast<double>( get_r( p ) ),
                                        boost::units::quantity_cast<double>( get_theta( p ) ),
                                        boost::units::quantity_cast<double>( get_phi( p ) ) ) );

    // Check that the polar point values are correctly set.
    BOOST_AUTO( p2r, get_r(p2));
    BOOST_CHECK( compare.equals( p2r, sqrt( 14.0 ) * boost::units::si::length() ) );
    BOOST_CHECK( compare.equals( get_theta( p2 ), atan2( 2.0, 1.0 ) * boost::units::si::plane_angle() ) );
    BOOST_CHECK( compare.equals( get_phi( p2 ), acos( 3.0 / sqrt( 14.0 ) ) * boost::units::si::plane_angle() ) );   

    // Check that they conversions are still the same back in Cartesian space.
    BOOST_CHECK( compare.equals( get_x( p2 ), 1.0 * boost::units::si::length() ) );
    BOOST_CHECK( compare.equals( get_y( p2 ), 2.0 * boost::units::si::length() ) );
    BOOST_CHECK( compare.equals( get_z( p2 ), 3.0 * boost::units::si::length() ) );    

    std::cout << get_x(p) << std::endl;
    std::cout << get_theta(p) << std::endl;    

    //! Test intersection calculation
    
    double t = 0.5;
    Point gp;
    gp <<= p + t * (p-p);
}

BOOST_AUTO_TEST_CASE( TestPoints )
{
    TestPoint< CartesianPoint3D >();
}

#endif //GEOMETRIX_POINT_EXAMPLE_HPP
