//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_REFERENCE_FRAME_TRANSFORMATION_HPP
#define GEOMETRIX_REFERENCE_FRAME_TRANSFORMATION_HPP

#include <geometrix/space/reference_frame_traits.hpp>
#include <geometrix/space/neutral_reference_frame.hpp>
#include <geometrix/space/coordinate_sequence.hpp>
#include <geometrix/tensor/tensor_access_policy.hpp>

namespace geometrix {  
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
        typedef typename reference_frame_traits< OriginReferenceFrame >::space_type           origin_affine_space_type;
        typedef typename reference_frame_traits< DestinationReferenceFrame >::space_type      destination_affine_space_type;
        typedef typename affine_space_traits< origin_affine_space_type >::dimension_type      origin_space_dimension_type;
        typedef typename affine_space_traits< destination_affine_space_type >::dimension_type destination_space_dimension_type;       
        
    };

    //! \brief A null transformation for points in the same frame.
    template <typename ReferenceFrame>
    struct reference_frame_transformation< ReferenceFrame, ReferenceFrame >
    {           
        //! \brief Define null transform on p.
        template <typename CoordinateSequence1, typename CoordinateSequence2 >
        static CoordinateSequence1 transform( CoordinateSequence2& p ) { return construct<CoordinateSequence1>( p ); }

        template <unsigned int Index, typename FromPoint>
        static typename type_at< FromPoint, Index >::type transform_coordinate( const FromPoint& p )
        {
            return get< Index >( p );
        }

        template <typename FromPoint>
        static typename type_at< FromPoint, 0 >::type transform_coordinate( const FromPoint& p, std::size_t index )
        {
            return get( p, index );
        }
    };

    //! \brief A null transformation for points in the same frame.
    template <typename ReferenceFrame, unsigned int D>
    struct reference_frame_transformation< ReferenceFrame, neutral_reference_frame<D> >
    {           
        //! \brief Define null transform on p.
        template <typename CoordinateSequence1, typename CoordinateSequence2 >
        static CoordinateSequence1 transform( CoordinateSequence2& p ) { return construct<CoordinateSequence1>( p ); }

        template <unsigned int Index, typename FromPoint>
        static typename type_at< FromPoint, Index >::type transform_coordinate( const FromPoint& p )
        {
            return get< Index >( p );
        }

        template <typename FromPoint>
        static typename type_at< FromPoint, 0 >::type transform_coordinate( const FromPoint& p, std::size_t index )
        {
            return get( p, index );
        }
    };

    //! \brief A null transformation for points in the same frame.
    template <typename ReferenceFrame, unsigned int D>
    struct reference_frame_transformation< neutral_reference_frame<D>, ReferenceFrame >
    {           
        //! \brief Define null transform on p.
        template <typename CoordinateSequence1, typename CoordinateSequence2 >
        static CoordinateSequence1 transform( CoordinateSequence2& p ) { return construct<CoordinateSequence1>( p ); }

        template <unsigned int Index, typename FromPoint>
        static typename type_at< FromPoint, Index >::type transform_coordinate( const FromPoint& p )
        {
            return get< Index >( p );
        }

        template <typename FromPoint>
        static typename type_at< FromPoint, 0 >::type transform_coordinate( const FromPoint& p, std::size_t index )
        {
            return get( p, index );
        }
    };
    
}//namespace geometrix;

#endif //GEOMETRIX_REFERENCE_FRAME_TRANSFORMATION_HPP
