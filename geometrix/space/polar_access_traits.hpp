//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_POLAR_ACCESS_TRAITS_HPP
#define GEOMETRIX_POLAR_ACCESS_TRAITS_HPP

#include <geometrix/space/polar_reference_frame.hpp>

namespace geometrix {

//! Access traits for numeric sequences in a polar reference frame.
//! NOTE: must be specialized for user types.
// template <typename T>
// struct polar_access_traits
// {
//     typedef T numeric_sequence_type;
// 
//     BOOST_MPL_ASSERT_MSG( 
// 		  ( false )
// 		, POLAR_ACCESS_TRAITS_NOT_DEFINED
// 		, (T) );	
// 
// };

//! \brief Macro to specialize the coordinate accessors for polar coords for polar point which model TensorConcept.
//! There is no real mechanism in place at the moment for actually constraining the type to be polar. This means
//! if the user defines a point type which is reference frame agnostic under both polar and other access types.. it will be 
//! up to the user to make certain the point passed to any given function is in the proper form.
//! Users who require compile-time tagging of point types should look at the reference_frame_adaptor type introduced in reference_frame_traits.
template <typename Sequence>
struct polar_access_traits 
{   
    typedef Sequence                                            sequence_type;
    typedef typename reference_frame_of<Sequence>::type         sequence_frame;
    typedef typename geometric_traits<Sequence>::dimension_type dimension_type;
    typedef polar_reference_frame< dimension_type::value >      polar_frame;

    //! \brief compile time access if available for the sequence.
    template <unsigned int Index>
    static typename type_at<Sequence, Index>::type get_raw( const Sequence& sequence ) 
    {
        return reference_frame_transformation
               <
                   sequence_frame
                 , polar_frame 
               >::transform_coordinate< Index >( sequence );
    }

    //! \brief compile time access if available for the sequence.
    template <unsigned int Index>
    static typename polar_frame::coordinate_type_of<Sequence, Index>::type get( const Sequence& sequence ) 
    {
        typedef typename polar_frame::coordinate_type_of<Sequence, Index>::type coordinate_type;
        typedef typename polar_frame::unit_at<Index>::type                      unit_type;
        return coordinate_type( reference_frame_transformation
                                <
                                    sequence_frame
                                  , polar_frame 
                                >::transform_coordinate< Index >( sequence ) * unit_type() );
    }
};

//! \brief A concept definition that requires an access interface to support access to locations in a polar reference frame.
//! TODO: This concept used to enforce get_r,theta,phi and as such made some sense. Now that there is an frame agnostic index access.. it doesn't do a single thing.
template <typename AccessInterface>
struct PolarCoordinateAccessorConcept
{
    typedef typename AccessInterface::sequence_type coordinate_sequence_type;
    
    BOOST_CLASS_REQUIRE( coordinate_sequence_type, geometrix, CoordinateSequenceConcept ); 

    void constraints() const
    {
        dimensional_constraints<coordinate_sequence_type>( typename geometric_traits< coordinate_sequence_type >::dimension_type() );
    }

    //! 2D access
    template <typename CoordinateSequence>
    void dimensional_constraints( dimension<2> )
    {
        coordinate_sequence_type* p = 0;
        AccessInterface::get<0>( *p );
        AccessInterface::get<1>( *p );

        *p = construction_policy< coordinate_sequence_type >::construct( 0, 0 );
    }

    //! 3D access
    template <typename CoordinateSequence>
    void dimensional_constraints( dimension<3> )
    {
        coordinate_sequence_type* p = 0;
        AccessInterface::get<0>( *p );
        AccessInterface::get<1>( *p );
        AccessInterface::get<2>( *p );

        *p = construction_policy< coordinate_sequence_type >::construct( 0, 0, 0 );
    }
};

//! \brief Convenience accessors for common r, theta, phi coords.
template <typename Tensor>
typename polar_reference_frame< dimension_of<Tensor>::value >::coordinate_type_of<Tensor, e_radius >::type get_r( const Tensor& seq )
{
    return polar_access_traits< Tensor >::get<e_radius>( seq );
}
template <typename Tensor>
typename polar_reference_frame< dimension_of<Tensor>::value >::coordinate_type_of<Tensor, e_theta>::type get_theta( const Tensor& seq )
{
    return polar_access_traits< Tensor >::get<e_theta>( seq );
}
template <typename Tensor>
typename polar_reference_frame< dimension_of<Tensor>::value >::coordinate_type_of<Tensor, e_phi>::type get_phi( const Tensor& seq )
{
    return polar_access_traits< Tensor >::get<e_phi>( seq );
}

}//namespace geometrix;

#endif //GEOMETRIX_POLAR_ACCESS_TRAITS_HPP
