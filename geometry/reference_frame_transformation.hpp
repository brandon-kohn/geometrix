//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_REFERENCE_FRAME_TRANSFORMATION_HPP
#define _BOOST_GEOMETRY_REFERENCE_FRAME_TRANSFORMATION_HPP
#pragma once

#include "reference_frame_traits.hpp"

namespace boost
{
namespace numeric
{
namespace geometry
{  
    //! Define the base traits of a frame of reference. This requires the definitions of an affine space.
    template <typename OriginReferenceFrame, typename DestinationReferenceFrame>
    struct reference_frame_transformation
    {
        typedef OriginReferenceFrame                                                          origin_frame;
        typedef DestinationReferenceFrame                                                     destination_frame;
        typedef typename reference_frame_traits< OriginReferenceFrame >                       origin_affine_space_type;
        typedef typename reference_frame_traits< DestinationReferenceFrame >                  destination_affine_space_type;
        typedef typename affine_space_traits< origin_affine_space_type >::dimension_type      origin_space_dimension_type;
        typedef typename affine_space_traits< origin_affine_space_type >::point_type          origin_point_type;
        typedef typename affine_space_traits< origin_affine_space_type >::vector_type         origin_vector_type;
        typedef typename affine_space_traits< destination_affine_space_type >::dimension_type destination_space_dimension_type;
        typedef typename affine_space_traits< destination_affine_space_type >::point_type     destination_point_type;
        typedef typename affine_space_traits< destination_affine_space_type >::vector_type    destination_vector_type;
        
        //! Define some transform on p.
        destination_point_type transform( const origin_point_type& p )
        {            
            return p;
        }

        BOOST_MPL_ASSERT_MSG( 
		  ( false )
		, REFERENCE_FRAME_TRANSFORM_NOT_DEFINED
		, (reference_frame_transformation<OriginReferenceFrame,DestinationReferenceFrame>) );

    };

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_REFERENCE_FRAME_TRANSFORMATION_HPP
