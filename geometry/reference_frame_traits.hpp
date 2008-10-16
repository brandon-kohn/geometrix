//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_REFERENCE_FRAME_TRAITS_HPP
#define _BOOST_GEOMETRY_REFERENCE_FRAME_TRAITS_HPP
#pragma once

#include "affine_space_traits.hpp"

namespace boost
{
namespace numeric
{
namespace geometry
{  
    //! Reference frame must define a tagging type templated for all geometry primitives.
    template <typename Geometry, typename ReferenceFrame>
    struct reference_frame_tag : public Geometry
    {
        typedef Geometry        geometry_type;
        typedef ReferenceFrame  reference_frame_type;
        
        //! construct from the raw geometry.
        explicit reference_frame_tag( const Geometry& geometry )
            : geometry_type( geometry )
        {}

        //! construct from a copy - duh! :D
        explicit reference_frame_tag( const reference_frame_tag<geometry_type, reference_frame_type>& geometry )
            : geometry_type( geometry )
        {}

        //! automatic transform?
        template <typename OtherGeometry, typename OtherReferenceFrame>
        explicit reference_frame_tag( const reference_frame_tag< OtherGeometry, OtherReferenceFrame >& geometry )
            : geometry_type( reference_frame_transform< OtherReferenceFrame, reference_frame_type >( geometry ) )
        {}
    };

    //! Define the base traits of a frame of reference. 
    template <typename ReferenceFrame>
    struct reference_frame_traits
    {
        //! Reference frame belongs to some affine space.
        typedef typename ReferenceFrame::AffineSpace                           affine_space_type;
        typedef typename affine_space_traits< affine_space_type >::point_type  point_type;
        typedef typename affine_space_traits< affine_space_type >::vector_type vector_type;
        typedef typename ReferenceFrame::reference_frame_type                  reference_frame_type;//! self reference for tagging primitives.

        //! Reference frame must define an origin.
        inline static point_type get_origin() { static point_type origin; return origin; }

        BOOST_MPL_ASSERT_MSG( 
		  ( false )
		, REFERENCE_FRAME_TRAITS_NOT_DEFINED
		, (ReferenceFrame) );

    };

    //! Attempt to make a reference frame tag points and vectors in some way such that they are then known to be in that frame. How? Adaptor wrapper?


}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_REFERENCE_FRAME_TRAITS_HPP
