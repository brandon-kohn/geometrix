//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_POLAR_CARTESIAN_TRANSFORMATION_HPP
#define _BOOST_GEOMETRY_POLAR_CARTESIAN_TRANSFORMATION_HPP
#pragma once

#include "cartesian_reference_frame.hpp"
#include "polar_reference_frame.hpp"
#include "reference_frame_transformation.hpp"
#include "math_functions.hpp"

namespace boost
{
namespace numeric
{
namespace geometry
{  
    //! Define the base traits of a frame of reference. This requires the definitions of an affine space.
    template < typename OriginAffineSpace, typename DestinationAffineSpace >
    struct reference_frame_transformation< polar_reference_frame< OriginAffineSpace >, cartesian_reference_frame< DestinationAffineSpace > >
    {
        typedef cartesian_reference_frame< OriginAffineSpace >                                origin_frame;
        typedef polar_reference_frame< DestinationAffineSpace >                               destination_frame;
        typedef typename reference_frame_traits< origin_frame >::affine_space_type            origin_affine_space_type;
        typedef typename reference_frame_traits< destination_frame >::affine_space_type       destination_affine_space_type;
        typedef typename affine_space_traits< origin_affine_space_type >::dimension_type      origin_space_dimension_type;
        typedef typename affine_space_traits< origin_affine_space_type >::point_type          origin_point_type;
        typedef typename affine_space_traits< origin_affine_space_type >::vector_type         origin_vector_type;
        typedef typename affine_space_traits< origin_affine_space_type >::numeric_type        origin_coordinate_type;
        typedef typename affine_space_traits< destination_affine_space_type >::dimension_type destination_space_dimension_type;
        typedef typename affine_space_traits< destination_affine_space_type >::point_type     destination_point_type;
        typedef typename affine_space_traits< destination_affine_space_type >::vector_type    destination_vector_type;
        typedef typename affine_space_traits< destination_affine_space_type >::numeric_type   destination_coordinate_type;

        template <unsigned int Dimension, typename From, typename To>
        struct transformer {};

        template<typename From, typename To>
        struct transformer<2, From, To>
        {            
            inline static To transform( const From& p )
            {
                boost::array<destination_coordinate_type, 2> coordinates;
                coordinates[0] = boost::numeric_cast< destination_coordinate_type >
                (
                    indexed_access_traits< From >::get<0>( p ) * math_functions< origin_coordinate_type >::cos( indexed_access_traits< From >::get<1>( p ) )
                );

                coordinates[1] = boost::numeric_cast< destination_coordinate_type >
                (
                    indexed_access_traits< From >::get<0>( p ) * math_functions< origin_coordinate_type >::sin( indexed_access_traits< From >::get<1>( p ) )
                );

                return construction_traits< To >::construct( coordinates );
            }
        };

        template<typename From, typename To>
        struct transformer<3, From, To>
        {                        
            inline static To transform( const From& p )
            {
                boost::array<destination_coordinate_type, 3> coordinates;
                coordinates[0] = boost::numeric_cast< destination_coordinate_type >
                (
                    indexed_access_traits< From >::get<0>( p ) * math_functions< origin_coordinate_type >::sin( indexed_access_traits< From >::get<1>( p ) ) * math_functions< origin_coordinate_type >::cos( indexed_access_traits< From >::get<2>( p ) )
                );

                coordinates[1] = boost::numeric_cast< destination_coordinate_type >
                (
                    indexed_access_traits< From >::get<0>( p ) * math_functions< origin_coordinate_type >::sin( indexed_access_traits< From >::get<1>( p ) ) * math_functions< origin_coordinate_type >::sin( indexed_access_traits< From >::get<2>( p ) )
                );

                coordinate[3] = boost::numeric_cast< destination_coordinate_type >
                (
                    indexed_access_traits< From >::get<0>( p ) * math_functions< origin_coordinate_type >::cos( indexed_access_traits< From >::get<1>( p ) )
                );

                return construction_traits< To >::construct( coordinates );
            }
        };

        inline static destination_point_type transform( const origin_point_type& p )
        {
            return transformer< destination_space_dimension_type::value, origin_point_type, destination_point_type >::transform( p );
        }
    };

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_POLAR_CARTESIAN_TRANSFORMATION_HPP
