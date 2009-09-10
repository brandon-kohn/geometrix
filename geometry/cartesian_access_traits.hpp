//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GENERATIVE_GEOMETRY_CARTESIAN_ACCESS_TRAITS_HPP
#define GENERATIVE_GEOMETRY_CARTESIAN_ACCESS_TRAITS_HPP
#pragma once

#include "cartesian_reference_frame.hpp"
#include <boost/concept_check.hpp>

namespace generative
{
namespace numeric
{
namespace geometry
{
//! \brief accessor for Cartesian coordinates from coordinate sequences which model IndexedAccessConcept.
template <typename Sequence>
struct cartesian_access_traits
{                                                                         
    typedef Sequence                                                                 sequence_type;
    typedef typename resolve_coordinate_sequence<Sequence>::sequence_type            real_sequence_type;
    typedef typename resolve_reference_frame<sequence_type>::reference_frame_type    sequence_frame;
    typedef typename sequence_traits<real_sequence_type>::value_type                 value_type;
    typedef typename coordinate_sequence_traits<real_sequence_type>::coordinate_type coordinate_type;
    typedef typename sequence_traits<real_sequence_type>::dimension_type             dimension_type;
    typedef cartesian_reference_frame< coordinate_type, dimension_type::value >      cartesian_frame;

    //! \brief compile time access if available for the sequence.
    template <unsigned int Index>
    static value_type get( const sequence_type& sequence ) 
    {   
        return reference_frame_transformation
               <
                   sequence_frame,
                   cartesian_frame 
               >::transform_coordinate< Index >( sequence );
    }

    //! \brief run-time access method if the sequence supports it.
    static value_type get( const sequence_type& sequence, size_t index  ) 
    {        
        return reference_frame_transformation
               <
                   sequence_frame,
                   cartesian_frame 
               >::transform_coordinate( sequence, index );
    }

};

//! \brief A concept definition that requires an access interface to support access to locations in a Cartesian reference frame.
//! TODO: This concept used to enforce get_x,y,z and as such made some sense. Now that there is an frame agnostic index access.. it doesn't do a single thing.
template <typename AccessInterface>
struct CartesianCoordinateAccessorConcept
{
    typedef typename AccessInterface::sequence_type   coordinate_sequence_type;
    typedef typename AccessInterface::coordinate_type coordinate_type;

    BOOST_CLASS_REQUIRE( coordinate_sequence_type, generative::numeric::geometry, CoordinateSequenceConcept );
    
    void constraints()
    {
        dimensional_constraints< coordinate_sequence_type >();
    }    

    //! older compilers require disambiguation
    template <int> struct disambiguation_tag { disambiguation_tag(int) {} };

    //! 2D access
    template <typename CoordinateSequence>
    typename boost::enable_if< boost::is_same< typename coordinate_sequence_traits< CoordinateSequence >::dimension_type, dimension_traits<2> >, void >::type dimensional_constraints( disambiguation_tag<0> = 0 )
    {
        coordinate_sequence_type* p = 0;
        coordinate_type x = AccessInterface::get<0>( *p );
        coordinate_type y = AccessInterface::get<1>( *p );

        //factory accessor
        *p = construction_traits< coordinate_sequence_type >::construct( x, y );
    }

    //! 3D access
    template <typename CoordinateSequence>
    typename boost::disable_if< boost::is_same< typename coordinate_sequence_traits< CoordinateSequence >::dimension_type, dimension_traits<2> >, void >::type dimensional_constraints( disambiguation_tag<1> = 0 )
    {            
        coordinate_sequence_type* p = 0;
        coordinate_type x = AccessInterface::get<0>( *p );
        coordinate_type y = AccessInterface::get<1>( *p );
        coordinate_type z = AccessInterface::get<2>( *p );

        //factory accessor
        *p = construction_traits< coordinate_sequence_type >::construct( x, y, y );
    }
};

//! \brief Convenience accessors for common x, y, z coords.
template <typename IndexedSequence>
typename cartesian_access_traits<IndexedSequence>::value_type get_x( const IndexedSequence& seq )
{
    return cartesian_access_traits< IndexedSequence >::get<0>( seq );
}
template <typename IndexedSequence>
typename cartesian_access_traits<IndexedSequence>::value_type get_y( const IndexedSequence& seq )
{
    return cartesian_access_traits< IndexedSequence >::get<1>( seq );
}
template <typename IndexedSequence>
typename cartesian_access_traits<IndexedSequence>::value_type get_z( const IndexedSequence& seq )
{
    return cartesian_access_traits< IndexedSequence >::get<2>( seq );
}

}}}//namespace generative::numeric::geometry

#endif //GENERATIVE_GEOMETRY_CARTESIAN_ACCESS_TRAITS_HPP
