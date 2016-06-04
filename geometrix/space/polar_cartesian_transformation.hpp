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
#include <geometrix/tensor/fusion_tensor_sequence_adaptor.hpp>

namespace geometrix {  

    namespace polar_cartesian_transform { namespace detail 
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

        template <std::size_t D, std::size_t I>
        struct term_calculator 
        {           
            template <typename From, typename Coordinate, typename State>
            static void apply(const From& from, const Coordinate& sum, State& state)                
            {      
				using std::sin;
				using std::cos;
                term_calculator<D, I-1>::template apply( from, sum * sin( get<I-1>( from ) ), state );
                boost::fusion::at_c<I-1>(state) = sum * cos( get<I-1>( from ) );
            }
        };

        template <std::size_t D>
        struct term_calculator<D, 2>
        {
            template <typename From, typename Coordinate, typename State>
            static void apply(const From& from, const Coordinate& sum, State& state)
            {
				using std::sin;
				using std::cos;
                boost::fusion::at_c<1>(state) = sum * sin( get<1>( from ) );
                boost::fusion::at_c<0>(state) = sum * cos( get<1>( from ) );
            }            
        };
    }}//! namespace polar_cartesian_transform::detail;

    //! \brief A transformation type for transforms from the polar reference frame to a Cartesian reference frame. 
    //! This class currently supports transformations in 2d and 3d.
    template <std::size_t OriginDimension, std::size_t DestinationDimension>
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
            using namespace polar_cartesian_transform::detail;
            typedef typename transform_state<To>::type state_type;
            state_type state;
            term_calculator<destination_space_dimension_type::value, destination_space_dimension_type::value>::apply( p, get<0>( p ), state );
            To result = construct<To>(state);
            return reference_frame_adaptor<To, destination_frame>( result );
        }

        template <std::size_t Index, typename T>
        static typename type_at< T, Index >::type transform_coordinate( const T& p )
        {
            using namespace polar_cartesian_transform::detail;
#if defined(_MSC_VER) && _MSC_VER < 1600
            typedef boost::array<typename type_at<T,Index>::type, destination_space_dimension_type::value> state_type;
#else
            typedef typename transform_state<T>::type state_type;
#endif
            state_type state;
            term_calculator<destination_space_dimension_type::value, destination_space_dimension_type::value>::apply( p, get<0>( p ), state );
            return boost::fusion::at_c<Index>(state);
        }
    };

}//namespace geometrix;

#endif //GEOMETRIX_POLAR_CARTESIAN_TRANSFORMATION_HPP
