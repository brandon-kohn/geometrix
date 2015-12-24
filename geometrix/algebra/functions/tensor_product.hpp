//
//! Copyright © 2008-2013
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_LINEAR_ALGEBRA_BINARY_FUNCTIONS_TENSOR_PRODUCTS_HPP
#define GEOMETRIX_LINEAR_ALGEBRA_BINARY_FUNCTIONS_TENSOR_PRODUCTS_HPP

#include <geometrix/algebra/functions/binary_function.hpp>
#include <geometrix/arithmetic/tensor_arithmetic.hpp>

namespace geometrix {

    namespace tag
    {
        typedef boost::proto::tag::bitwise_and tensor_product;
    }//namespace tag;

    GEOMETRIX_LINEAR_ALGEBRA_BINARY_OP( tag::tensor_product, is_vector, is_vector );
    GEOMETRIX_LINEAR_ALGEBRA_BINARY_OP( tag::tensor_product, is_vector, is_matrix );
    GEOMETRIX_LINEAR_ALGEBRA_BINARY_OP( tag::tensor_product, is_matrix, is_vector );
    GEOMETRIX_LINEAR_ALGEBRA_BINARY_OP( tag::tensor_product, is_matrix, is_matrix );
        
    //! Tensor Product of Vectors
    template <typename Left, typename Right>
    struct bin_fun
        <
            tag::tensor_product
          , Left
          , Right
          , typename geometric_traits<typename remove_const_ref<Left>::type>::is_vector
          , typename geometric_traits<typename remove_const_ref<Right>::type>::is_vector 
        >
        : binary_diversity_base<Left,Right>
    {
        typedef void                                                          is_matrix;
        typedef void                                                          rank_2;
        typedef typename result_of::tensor_product<Left,Right>::row_dimension row_dimension;
        typedef typename result_of::tensor_product<Left,Right>::col_dimension col_dimension;
        
		template <std::size_t Row, std::size_t Column>
        struct context : boost::proto::callable_context< const context<Row,Column> >
        {            
            typedef typename result_of::tensor_product_element<Left, Right, Row, Column>::type result_type;

            result_type operator()(tag::tensor_product, const Left& l, const Right& r ) const
            {
                return geometrix::tensor_product_element<Row,Column>( l, r );
            }
        };
    };

    //! Tensor Product of Vector and Matrix
    template <typename Left, typename Right>
    struct bin_fun
        <
            tag::tensor_product
          , Left
          , Right
          , typename geometric_traits<typename remove_const_ref<Left>::type>::is_matrix
          , typename geometric_traits<typename remove_const_ref<Right>::type>::is_vector 
        >
        : binary_diversity_base<Left,Right>
    {
        typedef void                                                                        is_matrix;
        typedef void                                                                        rank_2;
        typedef typename result_of::tensor_product<Left, row_vector<Right> >::row_dimension row_dimension;
        typedef typename result_of::tensor_product<Left, row_vector<Right> >::col_dimension col_dimension;
        
		template <std::size_t Row, std::size_t Column>
        struct context : boost::proto::callable_context< const context<Row,Column> >
        {            
            typedef typename result_of::tensor_product_element<Left, Right, Row, Column>::type result_type;

            result_type operator()(tag::tensor_product, const Left& l, const Right& r ) const
            {
                return geometrix::tensor_product_element<Row,Column>( l, r );
            }
        };
    };

    //! Tensor Product of Matrix and Vector
    template <typename Left, typename Right>
    struct bin_fun
        <
            tag::tensor_product
          , Left
          , Right
          , typename geometric_traits<typename remove_const_ref<Left>::type>::is_vector
          , typename geometric_traits<typename remove_const_ref<Right>::type>::is_matrix 
        >
        : binary_diversity_base<Left,Right>
    {
        typedef void                                                                          is_matrix;
        typedef void                                                                          rank_2;
        typedef typename result_of::tensor_product<column_vector<Left>, Right>::row_dimension row_dimension;
        typedef typename result_of::tensor_product<column_vector<Left>, Right>::col_dimension col_dimension;
        
		template <std::size_t Row, std::size_t Column>
        struct context : boost::proto::callable_context< const context<Row,Column> >
        {            
            typedef typename result_of::tensor_product_element<Left, Right, Row, Column>::type result_type;

            result_type operator()(tag::tensor_product, const Left& l, const Right& r ) const
            {
                return geometrix::tensor_product_element<Row,Column>( l, r );
            }
        };
    };
    
    //! Tensor Product of Matrix and Vector
    template <typename Left, typename Right>
    struct bin_fun
        <
            tag::tensor_product
          , Left
          , Right
          , typename geometric_traits<typename remove_const_ref<Left>::type>::is_matrix
          , typename geometric_traits<typename remove_const_ref<Right>::type>::is_matrix 
        >
        : binary_diversity_base<Left,Right>
    {
        typedef void                                                           is_matrix;
        typedef void                                                           rank_2;
        typedef typename result_of::tensor_product<Left, Right>::row_dimension row_dimension;
        typedef typename result_of::tensor_product<Left, Right>::col_dimension col_dimension;
        
		template <std::size_t Row, std::size_t Column>
        struct context : boost::proto::callable_context< const context<Row,Column> >
        {            
            typedef typename result_of::tensor_product_element<Left, Right, Row, Column>::type result_type;

            result_type operator()(tag::tensor_product, const Left& l, const Right& r ) const
            {
                return geometrix::tensor_product_element<Row,Column>( l, r );
            }
        };
    };

}//namespace geometrix;

#endif//GEOMETRIX_LINEAR_ALGEBRA_BINARY_FUNCTIONS_TENSOR_PRODUCTS_HPP
