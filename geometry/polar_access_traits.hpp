//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GENERATIVE_GEOMETRY_POLAR_ACCESS_TRAITS_HPP
#define GENERATIVE_GEOMETRY_POLAR_ACCESS_TRAITS_HPP
#pragma once

#include "polar_reference_frame.hpp"

namespace generative
{
namespace numeric
{
namespace geometry
{

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

//! \brief Macro to specialize the coordinate accessors for polar coords for polar point which model IndexedAccessConcept.
//! There is no real mechanism in place at the moment for actually constraining the type to be polar. This means
//! if the user defines a point type which is reference frame agnostic under both polar and other access types.. it will be 
//! up to the user to make certain the point passed to any given function is in the proper form.
//! Users who require compile-time tagging of point types should look at the reference_frame_tag type introduced in reference_frame_traits.
template <typename Sequence>
struct polar_access_traits 
{                                                                         
    typedef Sequence                                                                 sequence_type;
    typedef typename resolve_coordinate_sequence<sequence_type>::sequence_type       real_sequence_type;
    typedef typename resolve_reference_frame<sequence_type>::reference_frame_type    sequence_frame;
    typedef typename sequence_traits<real_sequence_type>::value_type                 value_type;
    typedef typename coordinate_sequence_traits<real_sequence_type>::coordinate_type coordinate_type;
    typedef typename coordinate_sequence_traits<real_sequence_type>::dimension_type  dimension_type;
    typedef polar_reference_frame< coordinate_type, dimension_type::value >          polar_frame;

    //! \brief run-time access method if the sequence supports it.
    static inline value_type get( const sequence_type& sequence, size_t index  ) 
    {   
        return indexed_access_traits< real_sequence_type >::get<Index>(
            static_cast< point< coordinate_type, dimension_type::value >&>(
                reference_frame_transformation
                <
                    sequence_frame,
                    polar_frame 
                >::transform< point< coordinate_type, dimension_type::value > >( sequence ) ), index );        
    }
    
    //! \brief compile time access if available for the sequence.
    template <unsigned int Index>
    static inline value_type get( const sequence_type& sequence ) 
    {
        return reference_frame_transformation
               <
                   sequence_frame,
                   polar_frame 
               >::transform_coordinate< Index >( sequence );
    }

};

//! \brief A concept definition that requires an access interface to support access to locations in a polar reference frame.
//! TODO: This concept used to enforce get_r,theta,phi and as such made some sense. Now that there is an frame agnostic index access.. it doesn't do a single thing.
template <typename AccessInterface>
struct PolarCoordinateAccessorConcept
{
    typedef typename AccessInterface::sequence_type            coordinate_sequence_type;
    typedef typename AccessInterface::coordinate_type          coordinate_type;

    BOOST_CLASS_REQUIRE( coordinate_sequence_type, generative::numeric::geometry, CoordinateSequenceConcept ); 

    void constraints()
    {
        dimensional_constraints< coordinate_sequence_type >();
    }

    //! older compilers require disambiguation
    template <int> struct disambiguation_tag { disambiguation_tag(int) {} };
    
    //! 2D access
    template <typename CoordinateSequence>
    typename boost::enable_if< 
        boost::is_same< 
            typename coordinate_sequence_traits< CoordinateSequence >::dimension_type,
            dimension_traits<2>
        >,
        void
    >::type dimensional_constraints( disambiguation_tag<0> = 0 )
    {
        coordinate_sequence_type* p = 0;
        coordinate_type r = AccessInterface::get<0>( *p );
        coordinate_type t = AccessInterface::get<1>( *p );

        *p = construction_traits< coordinate_sequence_type >::construct( r, t );
    }

    //! 3D access
    template <typename CoordinateSequence>
    typename boost::disable_if<
        boost::is_same< 
            typename coordinate_sequence_traits< CoordinateSequence >::dimension_type,
            dimension_traits<2> >,
            void
    >::type dimensional_constraints( disambiguation_tag<1> = 0 )
    {
        coordinate_sequence_type* p = 0;
        coordinate_type r = AccessInterface::get<0>( *p );
        coordinate_type t = AccessInterface::get<1>( *p );
        coordinate_type phi = AccessInterface::get<2>( *p );

        *p = construction_traits< coordinate_sequence_type >::construct( r, t, phi );
    }
};

//! \brief Convenience accessors for common r, theta, phi coords.
template <typename IndexedSequence>
typename polar_access_traits<IndexedSequence>::value_type get_r( const IndexedSequence& seq )
{
    return polar_access_traits< IndexedSequence >::get<0>( seq );
}
template <typename IndexedSequence>
typename polar_access_traits<IndexedSequence>::value_type get_theta( const IndexedSequence& seq )
{
    return polar_access_traits< IndexedSequence >::get<1>( seq );
}
template <typename IndexedSequence>
typename polar_access_traits<IndexedSequence>::value_type get_phi( const IndexedSequence& seq )
{
    return polar_access_traits< IndexedSequence >::get<2>( seq );
}

}}}//namespace generative::numeric::geometry

#endif //GENERATIVE_GEOMETRY_POLAR_ACCESS_TRAITS_HPP
