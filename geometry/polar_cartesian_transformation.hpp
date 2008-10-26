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
    //! \brief A transformation type for transforms from the polar reference frame to a Cartesian coordinate frame. 
    //* This class currently supports transformations in 2D and 3D.
template < typename OriginNumericType, unsigned int OriginDimension, typename DestinationNumericType, unsigned int DestinationDimension>
struct reference_frame_transformation< polar_reference_frame< OriginNumericType, OriginDimension >,
                                       cartesian_reference_frame< DestinationNumericType, DestinationDimension > >
{
    typedef polar_reference_frame< OriginNumericType, OriginDimension >                   origin_frame;
    typedef cartesian_reference_frame< DestinationNumericType, DestinationDimension >     destination_frame;
    typedef typename reference_frame_traits< origin_frame >::affine_space_type            origin_affine_space_type;
    typedef typename reference_frame_traits< destination_frame >::affine_space_type       destination_affine_space_type;
    typedef typename affine_space_traits< origin_affine_space_type >::dimension_type      origin_space_dimension_type;
    typedef typename affine_space_traits< origin_affine_space_type >::numeric_type        origin_coordinate_type;
    typedef typename affine_space_traits< destination_affine_space_type >::dimension_type destination_space_dimension_type;
    typedef typename affine_space_traits< destination_affine_space_type >::numeric_type   destination_coordinate_type;

    template <unsigned int Dimension, typename From, typename To, typename Enable = void>
    struct transformer {};

    template <unsigned int Dimension, typename From, typename To>
    struct transformer<Dimension, From, To, typename boost::enable_if_c< use_indexed_access_type<From>::value == use_compile_time_access::value ||
                                                                         use_indexed_access_type<To>::value == use_compile_time_access::value >::type >
    {
        template <typename Coordinate, unsigned int D, unsigned int N>
        struct term_calculator
        {           
            template <typename To, typename From>
            term_calculator( To& to, const From& from, Coordinate& sum )
            {                
                to[N-1] = sum * math_functions<Coordinate>::cos( polar_access_traits<From>::get<N-1>( from ) );
                sum *= math_functions<Coordinate>::sin( polar_access_traits<From>::get<N-1>( from ) );
                term_calculator<Coordinate, D, N-1>( to, from, sum );
            }
        };

        template <typename Coordinate, unsigned int D>
        struct term_calculator<Coordinate, D, 2>
        {
            template <typename To, typename From>
            term_calculator( To& to, const From& from, Coordinate& sum )
            {
                to[1] = sum * math_functions<Coordinate>::sin( polar_access_traits<From>::get<1>( from ) );
                to[0] = sum * math_functions<Coordinate>::cos( polar_access_traits<From>::get<1>( from ) ); 
            }
        };

        inline static To transform( const From& p )
        {
            typedef boost::fusion::vector< const_indexed_access_fusion_adaptor<From>&, const_indexed_access_fusion_adaptor<From>& > sequences;            
            boost::array<destination_coordinate_type, Dimension> coordinates;
            typedef resolve_coordinate_sequence< From >::sequence_type from_type;
            origin_coordinate_type sum( polar_access_traits<From>::get<0>( p ) );
            term_calculator< origin_coordinate_type, Dimension, Dimension>( coordinates, p, sum );
            return construction_traits< To >::construct( coordinates );
        }
    };

    /*
    template<typename From, typename To>
    struct transformer<2, From, To>
    {            
        inline static To transform( const From& p )
        {
            boost::array<destination_coordinate_type, 2> coordinates;
            coordinates[0] = boost::numeric_cast< destination_coordinate_type >
            (
                polar_access_traits< From >::get<0>( p ) * math_functions< origin_coordinate_type >::cos( polar_access_traits< From >::get<1>( p ) )
            );

            coordinates[1] = boost::numeric_cast< destination_coordinate_type >
            (
                polar_access_traits< From >::get<0>( p ) * math_functions< origin_coordinate_type >::sin( polar_access_traits< From >::get<1>( p ) )
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
                polar_access_traits< From >::get<0>( p ) * math_functions< origin_coordinate_type >::cos( polar_access_traits< From >::get<1>( p ) ) * math_functions< origin_coordinate_type >::sin( polar_access_traits< From >::get<2>( p ) )
            );

            coordinates[1] = boost::numeric_cast< destination_coordinate_type >
            (
                polar_access_traits< From >::get<0>( p ) * math_functions< origin_coordinate_type >::sin( polar_access_traits< From >::get<1>( p ) ) * math_functions< origin_coordinate_type >::sin( polar_access_traits< From >::get<2>( p ) )
            );

            coordinates[2] = boost::numeric_cast< destination_coordinate_type >
            (
                polar_access_traits< From >::get<0>( p ) * math_functions< origin_coordinate_type >::cos( polar_access_traits< From >::get<2>( p ) )
            );

            return construction_traits< To >::construct( coordinates );
        }
    };
    */

    template <typename ToPoint, typename FromPoint>
    inline static reference_frame_tag< ToPoint, destination_frame > transform( const reference_frame_tag< FromPoint, origin_frame >& p )
    {
        return transformer
            < destination_space_dimension_type::value, 
              reference_frame_tag< FromPoint, origin_frame >,
              reference_frame_tag< ToPoint, destination_frame >
            >::transform( p );
    }
};

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_POLAR_CARTESIAN_TRANSFORMATION_HPP
