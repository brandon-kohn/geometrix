//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_CARTESIAN_ACCESS_TRAITS_HPP
#define GEOMETRIX_CARTESIAN_ACCESS_TRAITS_HPP

#include <geometrix/space/cartesian_reference_frame.hpp>

#include <boost/concept_check.hpp>

namespace geometrix {

//! \brief accessor for Cartesian coordinates from coordinate sequences which model TensorConcept.
template <typename Sequence>
struct cartesian_access_traits
{                                                                         
    typedef Sequence                                                 sequence_type;
    typedef typename reference_frame_of<sequence_type>::type         sequence_frame;
    typedef typename geometric_traits<sequence_type>::dimension_type dimension_type;
    typedef cartesian_reference_frame<dimension_type::value>         cartesian_frame;

    //! \brief compile time access if available for the sequence.
    template <unsigned int Index>
    static typename type_at<sequence_type, Index>::type get_raw( const sequence_type& sequence ) 
    {
        return reference_frame_transformation
               <
                   sequence_frame
                 , cartesian_frame 
               >::transform_coordinate< Index >( sequence );
    }
    
    //! \brief compile time access if available for the sequence.
    template <unsigned int Index>
    static typename cartesian_frame::template coordinate_type_of<sequence_type, Index>::type get( const sequence_type& sequence ) 
    {   
        typedef typename cartesian_frame::template coordinate_type_of<sequence_type, Index>::type coordinate_type;
        typedef typename cartesian_frame::template unit_at<Index>::type                           unit_type;
        return coordinate_type( reference_frame_transformation
                                <
                                    sequence_frame
                                  , cartesian_frame 
                                >::transform_coordinate< Index >( sequence ) * unit_type() );
    }
};

//! \brief A concept definition that requires an access interface to support access to locations in a Cartesian reference frame.
//! TODO: This concept used to enforce get_x,y,z and as such made some sense. Now that there is an frame agnostic index access.. it doesn't do a single thing.
template <typename AccessInterface>
struct CartesianCoordinateAccessorConcept
{
    typedef typename AccessInterface::sequence_type   coordinate_sequence_type;
    
    BOOST_CLASS_REQUIRE( coordinate_sequence_type, geometrix, CoordinateSequenceConcept );
    
    void constraints() const
    {
        dimensional_constraints< coordinate_sequence_type >( geometric_traits< coordinate_sequence_type >::dimension_type() );
    }    

    //! 2d access
    template <typename CoordinateSequence>
    void dimensional_constraints( dimension<2> )
    {
        coordinate_sequence_type* p = 0;
        AccessInterface::get<0>( *p );
        AccessInterface::get<1>( *p );

        //factory construction
        *p = construction_policy< coordinate_sequence_type >::construct( 0, 0 );
    }

    //! 3d access
    template <typename CoordinateSequence>
    void dimensional_constraints( dimension<3> )
    {            
        coordinate_sequence_type* p = 0;
        AccessInterface::get<0>( *p );
        AccessInterface::get<1>( *p );
        AccessInterface::get<2>( *p );

        //factory accessor
        *p = construction_policy< coordinate_sequence_type >::construct( 0, 0, 0 );
    }
};

//! \brief Convenience accessors for common x, y, z coords.
template <typename Tensor>
typename typename cartesian_reference_frame< dimension_of<Tensor>::value >::template coordinate_type_of< Tensor, e_x >::type get_x( const Tensor& seq )
{
    return cartesian_access_traits< Tensor >::get<e_x>( seq );
}
template <typename Tensor>
typename typename cartesian_reference_frame< dimension_of<Tensor>::value >::template coordinate_type_of< Tensor, e_y >::type get_y( const Tensor& seq )
{
    return cartesian_access_traits< Tensor >::get<e_y>( seq );
}
template <typename Tensor>
typename typename cartesian_reference_frame< dimension_of<Tensor>::value >::template coordinate_type_of< Tensor, e_z >::type get_z( const Tensor& seq )
{
    return cartesian_access_traits< Tensor >::get<e_z>( seq );
}

}//namespace geometrix;

#endif //GEOMETRIX_CARTESIAN_ACCESS_TRAITS_HPP
