//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_POLAR_CARTESIAN_TRANSFORMATION_HPP
#define GEOMETRIX_POLAR_CARTESIAN_TRANSFORMATION_HPP

#include <geometrix/space/cartesian_reference_frame.hpp>
#include <geometrix/space/polar_reference_frame.hpp>
#include <geometrix/space/reference_frame_transformation.hpp>
#include <geometrix/arithmetic/arithmetic.hpp>

namespace geometrix {  

    namespace polar_cartesian_transform { namespace detail 
    {
        template <typename Coordinate, unsigned int D, unsigned int I>
        struct term_calculator
        {           
            template <typename To, typename From>
            term_calculator( To& to, const From& from, Coordinate& sum )
            {                
                to[I-1] = sum * math::cos( get<I-1>( from ) );
                sum *= math::sin( get<I-1>( from ) );
                term_calculator<Coordinate, D, I-1>( to, from, sum );
            }
        };

        template <typename Coordinate, unsigned int D>
        struct term_calculator<Coordinate, D, 2>
        {
            template <typename To, typename From>
            term_calculator( To& to, const From& from, Coordinate& sum )
            {
                to[1] = sum * math::sin( get<1>( from ) );
                to[0] = sum * math::cos( get<1>( from ) ); 
            }
        };

        template <unsigned int Dimension, typename To, typename From>
        inline To transform( const From& p )
        {
            typedef typename geometric_traits< From >::arithmetic_type arithmetic_type;
            boost::array< arithmetic_type, Dimension > coordinates;
            arithmetic_type sum( get<0>( p ) );            
            term_calculator< arithmetic_type, Dimension, Dimension >( coordinates, p, sum );
            return construct< To >( coordinates );
        }

        template <unsigned int Index, unsigned int Dimension, typename T>
        inline typename type_at< T, Index >::type transform_coordinate( const T& p )
        {
            typedef typename geometric_traits<T>::arithmetic_type arithmetic_type;
            boost::array<arithmetic_type, Dimension> coordinates;
            arithmetic_type sum( get<0>( p ) );
            term_calculator< arithmetic_type, Dimension, Dimension >( coordinates, p, sum );
            return arithmetic_promotion_policy< typename type_at< T, 0 >::type, arithmetic_type >::demote( coordinates[Index] );
        }
    }}//! namespace polar_cartesian_transform::detail;

    //! \brief A transformation type for transforms from the polar reference frame to a Cartesian coordinate frame. 
    //! This class currently supports transformations in 2d and 3d.
    template <unsigned int OriginDimension, unsigned int DestinationDimension>
    struct reference_frame_transformation< polar_reference_frame< OriginDimension >,
        cartesian_reference_frame< DestinationDimension > >
    {
        typedef polar_reference_frame< OriginDimension >                                      origin_frame;
        typedef cartesian_reference_frame< DestinationDimension >                             destination_frame;
        typedef typename reference_frame_traits< origin_frame >::space_type                   origin_affine_space_type;
        typedef typename reference_frame_traits< destination_frame >::space_type              destination_affine_space_type;
        typedef typename affine_space_traits< origin_affine_space_type >::dimension_type      origin_space_dimension_type;
        typedef typename affine_space_traits< destination_affine_space_type >::dimension_type destination_space_dimension_type;

        template <typename To, typename From>
        static reference_frame_adaptor< To, destination_frame > transform( const reference_frame_adaptor< From, origin_frame >& p )
        {
            return polar_cartesian_transform::detail::transform< destination_space_dimension_type::value, reference_frame_adaptor<To, destination_frame> >(p);        
        }

        template <unsigned int Index, typename T>
        static typename type_at< T, Index >::type transform_coordinate( const T& p )
        {
            return polar_cartesian_transform::detail::transform_coordinate<Index, destination_space_dimension_type::value>( p );        
        }
    };

}//namespace geometrix;

#endif //GEOMETRIX_POLAR_CARTESIAN_TRANSFORMATION_HPP
