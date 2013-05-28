//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_CARTESIAN_POLAR_TRANSFORMATION_HPP
#define GEOMETRIX_CARTESIAN_POLAR_TRANSFORMATION_HPP

#include <geometrix/space/cartesian_reference_frame.hpp>
#include <geometrix/space/polar_reference_frame.hpp>
#include <geometrix/space/reference_frame_transformation.hpp>
#include <geometrix/arithmetic/arithmetic.hpp>
#include <geometrix/arithmetic/arithmetic_promotion_policy.hpp>

namespace geometrix {  

    namespace cartesian_polar_transform { namespace detail 
    {
        template <typename Coordinate, unsigned int D, unsigned int I>
        struct term_calculator
        {           
            template <typename T, typename F>
            term_calculator( T& to, const F& from, Coordinate& sum )
            {
                to[I] = math::atan2( math::sqrt( sum ), get<I>( from ) );
                sum += get<I>( from ) * get<I>( from );
                term_calculator<Coordinate, D, I+1>( to, from, sum );
            }
        };

        template <typename Coordinate, unsigned int D>
        struct term_calculator<Coordinate, D, D>
        {
            template <typename T, typename F>
            term_calculator( T& to, const F& from, Coordinate& sum )
            {               
                //! end
                to[0] = math::sqrt( sum );
            }
        };

        template <typename Coordinate, unsigned int D>
        struct term_calculator<Coordinate, D, 1>
        {
            template <typename T, typename F>
            term_calculator( T& to, const F& from, Coordinate& sum )
            {
                to[1] = math::atan2( get<1>( from ), get<0>( from ) );
                sum += get<0>( from ) * get<0>( from ) +
                       get<1>( from ) * get<1>( from );
                term_calculator<Coordinate, D, 2>( to, from, sum );              
            }                     
        };

        template <unsigned int Dimension, typename To, typename From>
        inline To transform( const From& p )
        {
            typedef typename geometric_traits< From >::arithmetic_type arithmetic_type;
            boost::array< arithmetic_type, Dimension > coordinates;
            arithmetic_type sum(0);            
            term_calculator< arithmetic_type, Dimension, 1 >( coordinates, p, sum );
            return construct<To>( coordinates );
        }

        template <unsigned int Index, unsigned int Dimension, typename T>
        inline typename type_at< T, Index >::type transform_coordinate( const T& p )
        {
            typedef typename geometric_traits< T >::arithmetic_type arithmetic_type;
            boost::array<arithmetic_type, Dimension> coordinates;
            arithmetic_type sum(0);
            term_calculator< arithmetic_type, Dimension, 1 >( coordinates, p, sum );
            return arithmetic_promotion_policy<typename type_at< T, Index >::type>::demote( coordinates[Index] );
        }
    }}//! namespace cartesian_polar_transform::detail;
    
    //! \brief A transformation type for transforms from the Cartesian reference frame to a polar coordinate frame. 

    //! This class currently supports transformations in 2d and 3d.
    //! \ingroup CoordinateReferenceFrames
    template <unsigned int OriginDimension, unsigned int DestinationDimension>
    struct reference_frame_transformation< cartesian_reference_frame< OriginDimension >, 
                                           polar_reference_frame< DestinationDimension > >
    {
        typedef cartesian_reference_frame< OriginDimension >                                  origin_frame;
        typedef polar_reference_frame< DestinationDimension >                                 destination_frame;
        typedef typename reference_frame_traits< origin_frame >::space_type                   origin_affine_space_type;
        typedef typename reference_frame_traits< destination_frame >::space_type              destination_affine_space_type;
        typedef typename affine_space_traits< origin_affine_space_type >::dimension_type      origin_space_dimension_type;
        typedef typename affine_space_traits< destination_affine_space_type >::dimension_type destination_space_dimension_type;
        
        template <typename To, typename From>
        static reference_frame_adaptor< To, destination_frame > transform( const reference_frame_adaptor< From, origin_frame >& p )
        {
            return cartesian_polar_transform::detail::transform< destination_space_dimension_type::value, reference_frame_adaptor<To, destination_frame> >(p);        
        }

        template <unsigned int Index, typename T>
        static typename type_at< T, Index >::type transform_coordinate( const T& p )
        {
            return cartesian_polar_transform::detail::transform_coordinate<Index, destination_space_dimension_type::value>( p );        
        }
    };

}//namespace geometrix;

#endif //GEOMETRIX_CARTESIAN_POLAR_TRANSFORMATION_HPP
