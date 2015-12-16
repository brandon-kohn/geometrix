//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_LINEAR_ALGEBRA_BINARY_FUNCTIONS_DOT_PRODUCT_HPP
#define GEOMETRIX_LINEAR_ALGEBRA_BINARY_FUNCTIONS_DOT_PRODUCT_HPP

#include <geometrix/algebra/functions/binary_function.hpp>
#include <geometrix/algebra/dot_product.hpp>

namespace geometrix {

    namespace tag
    {
        typedef boost::proto::tag::multiplies  dot_product;
     }//namespace tag;

    GEOMETRIX_LINEAR_ALGEBRA_BINARY_OP( tag::dot_product, is_vector, is_vector );
    GEOMETRIX_LINEAR_ALGEBRA_BINARY_OP( boost::proto::tag::multiplies, is_vector, is_matrix );
    GEOMETRIX_LINEAR_ALGEBRA_BINARY_OP( boost::proto::tag::multiplies, is_matrix, is_vector );
                
    //! Product of Row Vector with Matrix
    template <typename Left, typename Right>
    struct bin_fun
        < 
            boost::proto::tag::multiplies
          , Left
          , Right
          , typename geometric_traits<typename remove_const_ref<Left>::type>::is_vector
          , typename geometric_traits<typename remove_const_ref<Right>::type>::is_matrix 
        >
        : binary_diversity_base<Left,Right>
    {
        typedef void                                      rank_2;
        typedef void                                      is_matrix;
        typedef typename row_dimension_of<Right>::type    row_dimension;
        typedef typename column_dimension_of<Right>::type col_dimension;

        template <unsigned int Row, unsigned int Column, typename Callable = boost::proto::callable >
        struct context : boost::proto::callable_context< const context<Row, Column, Callable> >
        {            
            typedef boost::proto::tag::multiplies tag_t;
            typedef typename result_of::dot_product
                <
                    Left
                  , column<Right, Column>
                >::type result_type;

            result_type operator()(tag_t, const Left& l, const Right& r ) const
            {
                return dot_product( l, column<Right,Column>(r) );
            }
        };
    };

    //! Product of Matrix with Column Vector
    template <typename Left, typename Right>
    struct bin_fun
        < 
            boost::proto::tag::multiplies
          , Left
          , Right
          , typename geometric_traits<typename remove_const_ref<Left>::type>::is_matrix
          , typename geometric_traits<typename remove_const_ref<Right>::type>::is_vector 
        >
        : binary_diversity_base<Left,Right>
    {
        typedef void                                     rank_1;
        typedef void                                     is_vector;
        typedef typename dimension_of<Right>::type       dimension_type;
        typedef typename reference_frame_of<Right>::type reference_frame;//! Todo: This isn't properly calculated under transforms.
        typedef void                                     is_sequence;
        typedef void                                     is_numeric_sequence;
        typedef void                                     is_coordinate_sequence;

        template <unsigned int Index, typename Callable = boost::proto::callable >
        struct context : boost::proto::callable_context< const context<Index, Callable> >
        {            
            typedef boost::proto::tag::multiplies tag_t;
            typedef typename result_of::dot_product
                <
                    row<Left,Index>
                  , Right
                >::type result_type;

            result_type operator()(tag_t, const Left& l, const Right& r ) const
            {
                return dot_product( row<Left,Index>(l), r );
            }
        };
    };
    
    //! Dot Product of Vectors
    template <typename Left, typename Right>
    struct bin_fun
        <
            tag::dot_product
          , Left
          , Right
          , typename geometric_traits<typename remove_const_ref<Left>::type>::is_vector
          , typename geometric_traits<typename remove_const_ref<Right>::type>::is_vector 
        >
    {
        typedef void is_scalar;
        typedef void rank_0;
        typedef void is_homogeneous;
        
        struct context : boost::proto::callable_context< const context >
        {            
            typedef tag::dot_product tag_t;
            typedef typename result_of::dot_product<Left, Right>::type result_type;

            result_type operator()(tag_t, const Left& l, const Right& r ) const
            {
                return geometrix::dot_product( l, r );
            }
        };
    };
        
}//namespace geometrix;

#endif//GEOMETRIX_LINEAR_ALGEBRA_BINARY_FUNCTIONS_DOT_PRODUCT_HPP
