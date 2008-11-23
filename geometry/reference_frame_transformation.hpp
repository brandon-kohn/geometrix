//
//! Copyright © 2008
//! Brandon Kohn
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
        BOOST_MPL_ASSERT_MSG( 
		      ( false )
		    , REFERENCE_FRAME_TRANSFORM_NOT_DEFINED
		    , (reference_frame_transformation<OriginReferenceFrame,DestinationReferenceFrame>) );

        typedef OriginReferenceFrame                                                          origin_frame;
        typedef DestinationReferenceFrame                                                     destination_frame;
        typedef typename reference_frame_traits< OriginReferenceFrame >                       origin_affine_space_type;
        typedef typename reference_frame_traits< DestinationReferenceFrame >                  destination_affine_space_type;
        typedef typename affine_space_traits< origin_affine_space_type >::dimension_type      origin_space_dimension_type;
        typedef typename affine_space_traits< destination_affine_space_type >::dimension_type destination_space_dimension_type;       
        
    };

    //! \brief A null transformation for points in the same frame.
    template <typename ReferenceFrame>
    struct reference_frame_transformation< ReferenceFrame, ReferenceFrame >
    {           
        //! \brief Define null transform on p.
        template <typename CoordinateSequence>
        static const CoordinateSequence& transform( const CoordinateSequence& p ) { return p; }

        template <typename CoordinateSequence>
        static CoordinateSequence& transform( CoordinateSequence& p ) { return p; }
    };

    //! \brief A null transformation for points in the same frame.
    template <typename ReferenceFrame, typename T, unsigned int D>
    struct reference_frame_transformation< ReferenceFrame, neutral_reference_frame<T,D> >
    {           
        //! \brief Define null transform on p.
        template <typename CoordinateSequence>
        static const CoordinateSequence& transform( const CoordinateSequence& p ) { return p; }

        template <typename CoordinateSequence>
        static CoordinateSequence& transform( CoordinateSequence& p ) { return p; }
    };

    //! \brief A null transformation for points in the same frame.
    template <typename ReferenceFrame, typename T, unsigned int D>
    struct reference_frame_transformation< neutral_reference_frame<T,D>, ReferenceFrame >
    {           
        //! \brief Define null transform on p.
        template <typename CoordinateSequence>
        static const CoordinateSequence& transform( const CoordinateSequence& p ) { return p; }

        template <typename CoordinateSequence>
        static CoordinateSequence& transform( CoordinateSequence& p ) { return p; }
    };
    
}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_REFERENCE_FRAME_TRANSFORMATION_HPP
