//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_CARTESIAN_ACCESS_TRAITS_HPP
#define _BOOST_GEOMETRY_CARTESIAN_ACCESS_TRAITS_HPP
#pragma once

#include "reference_frame_tag.hpp"
#include "indexed_access_traits.hpp"
#include "cartesian_reference_frame.hpp"
#include "reference_frame_transformation.hpp"

namespace boost
{
namespace numeric
{
namespace geometry
{
//! \brief accessor for cartesian coordinates from coordinate sequences which model IndexedAccessConcept.
template <typename Sequence>
struct cartesian_access_traits
{                                                                         
    typedef Sequence                                                                 sequence_type;
    typedef typename resolve_coordinate_sequence<Sequence>::sequence_type            real_sequence_type;
    typedef typename resolve_reference_frame<sequence_type>::reference_frame_type    sequence_frame;
    typedef typename sequence_traits<real_sequence_type>::const_reference            const_reference;
    typedef typename sequence_traits<real_sequence_type>::reference                  reference;
    typedef typename coordinate_sequence_traits<real_sequence_type>::coordinate_type coordinate_type;
    typedef typename sequence_traits<real_sequence_type>::dimension_type             dimension_type;
    typedef cartesian_reference_frame< coordinate_type, dimension_type::value >      cartesian_frame;

    //! \brief compile time access if available for the sequence.
    template <unsigned int Index>
    static inline const_reference get( const sequence_type& sequence ) 
    {   
        return indexed_access_traits< real_sequence_type >::get<Index>(
            static_cast<const real_sequence_type&>(
                reference_frame_transformation< 
                    sequence_frame, 
                    cartesian_frame >::transform<real_sequence_type>( sequence ) ) );
    }

    //! \brief run-time access method if the sequence supports it.
    static inline const_reference get( const sequence_type& sequence, size_t index  ) 
    {        
        return indexed_access_traits< real_sequence_type >::get<Index>(
            static_cast<const real_sequence_type&>(
                reference_frame_transformation<
                    sequence_frame,
                    cartesian_frame >::transform<real_sequence_type>( sequence ) ), index );
    }

    //! \brief compile time access if available for the sequence.
    template <unsigned int Index>
    static inline reference get( sequence_type& sequence ) 
    {
        return indexed_access_traits< real_sequence_type >::get<Index>(
            static_cast<real_sequence_type&>(
                reference_frame_transformation<
                    sequence_frame,
                    cartesian_frame >::transform<real_sequence_type>( sequence ) ) );
    }

    //! \brief run-time access method if the sequence supports it.
    static inline reference get( sequence_type& sequence, size_t index  ) 
    {        
        return indexed_access_traits< real_sequence_type >::get<Index>(
            static_cast<real_sequence_type&>(
                reference_frame_transformation<
                    sequence_frame,
                    cartesian_frame >::transform<real_sequence_type>( sequence ) ), index );
    }
};

}}}//namespace boost::numeric::geometry

#endif //_BOOST_GEOMETRY_CARTESIAN_ACCESS_TRAITS_HPP


