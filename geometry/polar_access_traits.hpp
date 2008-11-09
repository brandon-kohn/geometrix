//
//! Copyright © 2008
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_POLAR_ACCESS_TRAITS_HPP
#define _BOOST_GEOMETRY_POLAR_ACCESS_TRAITS_HPP
#pragma once

#include "indexed_access_traits.hpp"
#include "polar_reference_frame.hpp"

namespace boost
{
namespace numeric
{
namespace geometry
{

//! Access traits for numeric sequences in a polar reference frame.
//! NOTE: must be specialized for user types.
//template <typename T>
//struct polar_access_traits
//{
//    typedef T numeric_sequence_type;
//
//    BOOST_MPL_ASSERT_MSG( 
//		  ( false )
//		, POLAR_ACCESS_TRAITS_NOT_DEFINED
//		, (T) );	
//
//};

//! \brief Macro to specialize the coordinate accessors for polar coords for polar point which model IndexedAccessConcept.
//* There is no real mechanism in place at the moment for actually constraining the type to be polar. This means
//* if the user defines a point type which is reference frame agnostic under both polar and other access types.. it will be 
//* up to the user to make certain the point passed to any given function is in the proper form.
//* Users who require compile-time tagging of point types should look at the reference_frame_tag type introduced in reference_frame_traits.
template <typename Sequence>
struct polar_access_traits 
{                                                                         
    typedef Sequence                                                                 sequence_type;
    typedef typename resolve_coordinate_sequence<sequence_type>::sequence_type       real_sequence_type;
    typedef typename resolve_reference_frame<sequence_type>::reference_frame_type    sequence_frame;
    typedef typename sequence_traits<real_sequence_type>::const_reference            const_reference;
    typedef typename sequence_traits<real_sequence_type>::reference                  reference;
    typedef typename coordinate_sequence_traits<real_sequence_type>::coordinate_type coordinate_type;
    typedef typename coordinate_sequence_traits<real_sequence_type>::dimension_type  dimension_type;
    typedef polar_reference_frame< coordinate_type, dimension_type::value >          polar_frame;
    
    //! \brief compile time access if available for the sequence.
    template <unsigned int Index>
    static inline const_reference get( const sequence_type& sequence ) 
    {                
        return indexed_access_traits< real_sequence_type >::get<Index>(
            static_cast<const real_sequence_type&>(
                reference_frame_transformation< 
                    sequence_frame, 
                    polar_frame >::transform<real_sequence_type>( sequence ) ) );
    }

    //! \brief run-time access method if the sequence supports it.
    static inline const_reference get( const sequence_type& sequence, size_t index  ) 
    {   
        return indexed_access_traits< real_sequence_type >::get<Index>(
            static_cast<const real_sequence_type&>(
                reference_frame_transformation<
                    sequence_frame,
                    polar_frame >::transform<real_sequence_type>( sequence ) ), index );
    }

    //! \brief compile time access if available for the sequence.
    template <unsigned int Index>
    static inline reference get( sequence_type& sequence ) 
    {
        return indexed_access_traits< real_sequence_type >::get<Index>(
            static_cast<real_sequence_type&>(
                reference_frame_transformation<
                    sequence_frame,
                    polar_frame >::transform<real_sequence_type>( sequence ) ) );
    }

    //! \brief run-time access method if the sequence supports it.
    static inline reference get( sequence_type& sequence, size_t index  ) 
    {        
        return indexed_access_traits< real_sequence_type >::get<Index>(
            static_cast<real_sequence_type&>(
                reference_frame_transformation<
                    sequence_frame,
                    polar_frame >::transform<real_sequence_type>( sequence ) ), index );
    }
};

}}}//namespace boost::numeric::geometry

#endif //_BOOST_GEOMETRY_POLAR_ACCESS_TRAITS_HPP
