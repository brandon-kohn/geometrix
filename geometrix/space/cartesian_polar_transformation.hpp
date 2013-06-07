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
#include <geometrix/tensor/fusion_tensor_sequence_adaptor.hpp>

namespace geometrix {  

    namespace cartesian_polar_transform { namespace detail 
    {
        template <typename To, typename EnableIf=void>
        struct transform_state
        {};

        template <typename To>
        struct transform_state<To, typename geometric_traits<To>::is_homogeneous>
        {
            typedef boost::array<typename type_at<To,0>::type, dimension_of<To>::value> type;            
        };

        template <typename To>
        struct transform_state<To, typename geometric_traits<To>::is_heterogeneous>
            : boost::fusion::result_of::as_vector< typename geometric_traits<To>::storage_types >
        {
        };

        template <unsigned int D, unsigned int I>
        struct term_calculator
        {           
            template <typename F, typename Coordinate, typename State>
            static void apply(const F& from, const Coordinate& sum, State& state)
            {
                boost::fusion::at_c<I>(state) = math::atan2( math::sqrt( sum ), get<I>( from ) );
                term_calculator<D, I+1>::template apply(from, sum + get<I>( from ) * get<I>( from ), state);
            }
        };

        template <unsigned int D>
        struct term_calculator<D, D>
        {
            template <typename F, typename Coordinate, typename State>
            static void apply(const F& from, const Coordinate& sum, State& state )
            {
                boost::fusion::at_c<0>(state) = math::sqrt( sum );
            }
        };

        template <unsigned int D>
        struct term_calculator<D, 1>
        {
            template <typename F, typename Coordinate, typename State>
            static void apply(const F& from, const Coordinate& sum, State& state)
            {
                boost::fusion::at_c<1>(state) = math::atan2( get<1>( from ), get<0>( from ) );
                term_calculator<D, 2>::template apply(from, sum + get<0>( from ) * get<0>( from ) + get<1>( from ) * get<1>( from ), state );
            }                     
        };
        
        template <unsigned int Dimension, typename To, typename From>
        inline To transform( const From& p )
        {
            typename transform_state<To>::type state;
            term_calculator<Dimension, 1>::apply( p, 0, state );
            To result = construct<To>(state);
            return result;
        }

        template <unsigned int Index, unsigned int Dimension, typename T>
        inline typename type_at< T, Index >::type transform_coordinate( const T& p )
        {
            typename transform_state<T>::type state;
            term_calculator<Dimension, 1>::apply( p, 0, state );
            return boost::fusion::at_c<Index>(state);
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
