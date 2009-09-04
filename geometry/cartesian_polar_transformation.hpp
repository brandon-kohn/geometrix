//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GENERATIVE_GEOMETRY_CARTESIAN_POLAR_TRANSFORMATION_HPP
#define GENERATIVE_GEOMETRY_CARTESIAN_POLAR_TRANSFORMATION_HPP
#pragma once

#include "cartesian_reference_frame.hpp"
#include "polar_reference_frame.hpp"
#include "reference_frame_transformation.hpp"
#include "math_functions.hpp"

namespace generative
{
namespace numeric
{
namespace geometry
{  

//! \brief A transformation type for transforms from the Cartesian reference frame to a polar coordinate frame. 

//! This class currently supports transformations in 2D and 3D.
//! \ingroup CoordinateReferenceFrames
template < typename OriginNumericType, unsigned int OriginDimension, typename DestinationNumericType, unsigned int DestinationDimension>
struct reference_frame_transformation< cartesian_reference_frame< OriginNumericType, OriginDimension >, 
                                       polar_reference_frame< DestinationNumericType, DestinationDimension > >
{
    typedef cartesian_reference_frame< OriginNumericType, OriginDimension >               origin_frame;
    typedef polar_reference_frame< DestinationNumericType, DestinationDimension >         destination_frame;
    typedef typename reference_frame_traits< origin_frame >::affine_space_type            origin_affine_space_type;
    typedef typename reference_frame_traits< destination_frame >::affine_space_type       destination_affine_space_type;
    typedef typename affine_space_traits< origin_affine_space_type >::dimension_type      origin_space_dimension_type;
    typedef typename affine_space_traits< origin_affine_space_type >::numeric_type        origin_coordinate_type;
    typedef typename affine_space_traits< destination_affine_space_type >::dimension_type destination_space_dimension_type;
    typedef typename affine_space_traits< destination_affine_space_type >::numeric_type   destination_coordinate_type;

    template <unsigned int Dimension, typename From, typename To, typename Enable = void>
    struct transformer {};

    template <unsigned int Dimension, typename From, typename To>
    struct transformer<Dimension, From, To, typename boost::enable_if_c< should_use_compile_time_access2<From,To>::type::value >::type >
    {


        template <typename Coordinate, unsigned int D, unsigned int N>
        struct term_calculator
        {           
            template <typename To, typename From>
            term_calculator( To& to, const From& from, Coordinate& sum )
            {
                to[N] = math_functions<Coordinate>::atan2( math_functions<Coordinate>::sqrt( sum ), cartesian_access_traits<From>::get<N>( from ) );
                sum += cartesian_access_traits<From>::get<N>( from ) * cartesian_access_traits<From>::get<N>( from );
                term_calculator<Coordinate, D, N+1>( to, from, sum );
            }
        };

        template <typename Coordinate, unsigned int D>
        struct term_calculator<Coordinate, D, D>
        {
            template <typename To, typename From>
            term_calculator( To& to, const From& from, Coordinate& sum )
            {               
                //! end
                to[0] = math_functions<Coordinate>::sqrt( sum );
            }
        };

        template <typename Coordinate, unsigned int D>
        struct term_calculator<Coordinate, D, 1>
        {
            template <typename To, typename From>
            term_calculator( To& to, const From& from, Coordinate& sum )
            {
                to[1] = math_functions<Coordinate>::atan2( cartesian_access_traits<From>::get<1>( from ), cartesian_access_traits<From>::get<0>( from ) );
                sum += cartesian_access_traits<From>::get<0>( from ) * cartesian_access_traits<From>::get<0>( from ) +
                       cartesian_access_traits<From>::get<1>( from ) * cartesian_access_traits<From>::get<1>( from );
                term_calculator<Coordinate, D, 2>( to, from, sum );              
            }                     
        };

        inline static To transform( const From& p )
        {
            typedef boost::fusion::vector< From&, From& > sequences;            
            boost::array<destination_coordinate_type, Dimension> coordinates;
            typedef resolve_coordinate_sequence< From >::sequence_type from_type;
            origin_coordinate_type sum(0);
            term_calculator< origin_coordinate_type, Dimension, 1 >( coordinates, p, sum );
            return construction_traits< To >::construct( coordinates );
        }

        template <unsigned int Index, typename T>
        inline static destination_coordinate_type transform_coordinate( const T& p )
        {
            boost::array<destination_coordinate_type, Dimension> coordinates;
            origin_coordinate_type sum(0);
            term_calculator< origin_coordinate_type, Dimension, 1 >( coordinates, p, sum );
            return coordinates[Index];
        }
    };

    template <unsigned int Dimension, typename From, typename To>
    struct transformer<Dimension, From, To, typename boost::enable_if_c< should_use_run_time_access2<From,To>::type::value >::type >
    {
        template <typename Coordinate, unsigned int Dimension>
        struct term_calculator
        {           
            template <typename To, typename From>
            term_calculator( To& to, const From& from, Coordinate& sum )
            {
                to[Dimension-1] = math_functions<Coordinate>::atan2( cartesian_access_traits<From>::get( from, Dimension-1 ), cartesian_access_traits<From>::get( from, Dimension - 2 ) );
                sum = cartesian_access_traits<From>::get<D-1>( from ) * cartesian_access_traits<From>::get<D-1>( from );
                for( unsigned int i=Dimension-1; i > 1; --i )
                {
                    sum += cartesian_access_traits<From>::get( from, i-1 ) * cartesian_access_traits<From>::get( from, i-1 );
                    to[i-1] = math_functions<Coordinate>::atan2( sum, cartesian_access_traits<From>::get( from, i-1 ) );                    
                }
                sum += cartesian_access_traits<From>::get( from, 0 ) * cartesian_access_traits<From>::get( from, 0 );
                to[0] = math_functions<Coordinate>::sqrt( sum );
            }
        };

        //! Specialize the low dimension cases.
        template <typename Coordinate>
        struct term_calculator< Coordinate, 3 >
        {           
            template <typename To, typename From>
            term_calculator( To& to, const From& from, Coordinate& sum )
            {
                to[1] = math_functions<Coordinate>::atan2( cartesian_access_traits<From>::get( from, 1 ), cartesian_access_traits<From>::get( from, 0 ) );

                sum = cartesian_access_traits<From>::get( from, 0 ) * cartesian_access_traits<From>::get( from, 0 ) +
                      cartesian_access_traits<From>::get( from, 1 ) * cartesian_access_traits<From>::get( from, 1 );
                to[2] = math_functions<Coordinate>::atan2( math_functions<Coordinate>::sqrt( sum ), cartesian_access_traits<From>::get( from, 2 ) );
                
                sum += cartesian_access_traits<From>::get( from, 2 ) * cartesian_access_traits<From>::get( from, 2 );
                to[0] = math_functions<Coordinate>::sqrt( sum );
            }
        };

        template <typename Coordinate>
        struct term_calculator< Coordinate, 2 >
        {           
            template <typename To, typename From>
            term_calculator( To& to, const From& from, Coordinate& sum )
            {
                to[1] = math_functions<Coordinate>::atan2( cartesian_access_traits<From>::get( from, 1 ), cartesian_access_traits<From>::get( from, 0 ) );

                sum = cartesian_access_traits<From>::get( from, 0 ) * cartesian_access_traits<From>::get( from, 0 ) +
                      cartesian_access_traits<From>::get( from, 1 ) * cartesian_access_traits<From>::get( from, 1 ) +
                      cartesian_access_traits<From>::get( from, 2 ) * cartesian_access_traits<From>::get( from, 2 );

                to[0] = math_functions<Coordinate>::sqrt( sum );
            }
        };

        template <typename T>
        inline static To transform( const T& p )
        {
            boost::array<destination_coordinate_type, Dimension> coordinates;
            origin_coordinate_type sum(0);
            term_calculator< origin_coordinate_type, Dimension >( coordinates, p, sum );
            return construction_traits< To >::construct( coordinates );
        }

        template <typename T>
        inline static destination_coordinate_type transform_coordinate( const T& p, std::size_t index )
        {
            boost::array<destination_coordinate_type, Dimension> coordinates;
            origin_coordinate_type sum(0);
            term_calculator< origin_coordinate_type, Dimension, Dimension>( coordinates, p, sum );
            return coordinates[index];
        }
    };

    template <typename ToPoint, typename FromPoint>
    inline static reference_frame_tag< ToPoint, destination_frame > transform( const reference_frame_tag< FromPoint, origin_frame >& p )
    {
        return transformer
               <
                   destination_space_dimension_type::value, 
                   reference_frame_tag< FromPoint, origin_frame >,
                   reference_frame_tag< ToPoint, destination_frame >
               >::transform( p );
    }

    template <unsigned int Index, typename CoordinateSequence>
    inline static destination_coordinate_type transform_coordinate( const CoordinateSequence& p )
    {
        return transformer
               <
                   destination_space_dimension_type::value, 
                   CoordinateSequence,
                   CoordinateSequence
               >::transform_coordinate<Index>( p );
    }

    template <typename FromPoint>
    inline static destination_coordinate_type transform_coordinate( const FromPoint& p, std::size_t index )
    {
        return transformer
            <
            destination_space_dimension_type::value, 
            reference_frame_tag< FromPoint, origin_frame >,
            reference_frame_tag< FromPoint, destination_frame >
            >::transform_coordinate<Index>( p, index );
    }
};

}}}//namespace generative::numeric::geometry;

#endif //GENERATIVE_GEOMETRY_CARTESIAN_POLAR_TRANSFORMATION_HPP
