//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_LINEAR_ALGEBRA_BINARY_FUNCTIONS_DIVISION_HPP
#define GEOMETRIX_LINEAR_ALGEBRA_BINARY_FUNCTIONS_DIVISION_HPP

#include <geometrix/algebra/functions/binary_function.hpp>

namespace geometrix {
        
    GEOMETRIX_LINEAR_ALGEBRA_BINARY_OP( boost::proto::tag::divides, is_vector, is_scalar );
    GEOMETRIX_LINEAR_ALGEBRA_BINARY_OP( boost::proto::tag::divides, is_matrix, is_scalar );
    GEOMETRIX_LINEAR_ALGEBRA_BINARY_OP( boost::proto::tag::divides, is_scalar, is_scalar );
        
    //! Division of Scalar and Scalar
    template <typename Left, typename Right>
    struct bin_fun
        < 
            boost::proto::tag::divides
          , Left
          , Right
          , typename geometric_traits<typename remove_const_ref<Left>::type>::is_scalar
          , typename geometric_traits<typename remove_const_ref<Right>::type>::is_scalar 
        >
    {
        typedef void is_scalar;
        typedef void rank_0;
        typedef void is_homogeneous;

        struct context : boost::proto::callable_context< const context >
        {            
            typedef boost::proto::tag::divides tag_t;
            typedef typename result_of::divides
                    <
                        typename type_at<Left>::type
                      , typename type_at<Right>::type
                    >::type result_type;

            result_type operator()(tag_t, const Left& l, const Right& r ) const
            {
                return get( l ) / get( r );
            }
        };
    };
    
    //! Division of Vector by Scalar
    template <typename Left, typename Right>
    struct bin_fun
        <
            boost::proto::tag::divides
          , Left
          , Right
          , typename geometric_traits<typename remove_const_ref<Left>::type>::is_vector
          , typename geometric_traits<typename remove_const_ref<Right>::type>::is_scalar 
        >
        : diversity_base<Left>
    {
        typedef void                                    is_vector;
        typedef void                                    rank_1;
        typedef typename dimension_of<Left>::type       dimension_type;
        typedef typename reference_frame_of<Left>::type reference_frame;//! Todo: This isn't properly calculated under transforms.
        typedef void                                    is_sequence;
        typedef void                                    is_numeric_sequence;
        typedef void                                    is_coordinate_sequence;

        template <unsigned int Index, typename Callable = boost::proto::callable >
        struct context : boost::proto::callable_context< const context<Index, Callable> >
        {            
            typedef boost::proto::tag::divides tag_t;
            typedef typename result_of::divides
                    <
                        typename type_at<Left, Index>::type
                      , typename type_at<Right>::type
                    >::type result_type;

            result_type operator()(tag_t, const Left& l, const Right& r ) const
            {
                return get<Index>( l ) / get( r );            
            }
        };
    };
    
    //! Division of Matrix by Scalar
    template <typename Left, typename Right>
    struct bin_fun
        < 
            boost::proto::tag::divides
          , Left
          , Right
          , typename geometric_traits<typename remove_const_ref<Left>::type>::is_matrix
          , typename geometric_traits<typename remove_const_ref<Right>::type>::is_scalar 
        >
        : diversity_base<Left>
    {
        typedef void                                     is_matrix;
        typedef void                                     rank_2;
        typedef typename row_dimension_of<Left>::type    row_dimension;
        typedef typename column_dimension_of<Left>::type col_dimension;

        template <unsigned int Row, unsigned int Column, typename Callable = boost::proto::callable >
        struct context : boost::proto::callable_context< const context<Row, Column, Callable> >
        {            
            typedef boost::proto::tag::divides tag_t;
            typedef typename result_of::divides
                    <
                        typename type_at<Left, Row, Column>::type
                      , typename type_at<Right>::type
                    >::type result_type;

            result_type operator()(tag_t, const Left& l, const Right& r ) const
            {
                return get<Row, Column>( l ) / get( r );
            }
        };
    };
        
}//namespace geometrix;

#endif//GEOMETRIX_LINEAR_ALGEBRA_BINARY_FUNCTIONS_DIVISION_HPP
