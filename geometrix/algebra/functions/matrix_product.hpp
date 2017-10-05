//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_LINEAR_ALGEBRA_BINARY_FUNCTIONS_MATRIX_PRODUCT_HPP
#define GEOMETRIX_LINEAR_ALGEBRA_BINARY_FUNCTIONS_MATRIX_PRODUCT_HPP

#include <geometrix/algebra/functions/binary_function.hpp>

namespace geometrix {

    namespace tag
    {
        typedef boost::proto::tag::multiplies  matrix_product;
    }//namespace tag;

    GEOMETRIX_LINEAR_ALGEBRA_BINARY_OP( tag::matrix_product, is_matrix, is_matrix );
       
    //! Product of Matrices
    template <typename Left, typename Right>
    struct bin_fun
        <
            tag::matrix_product
          , Left
          , Right
          , typename geometric_traits<typename remove_const_ref<Left>::type>::is_matrix
          , typename geometric_traits<typename remove_const_ref<Right>::type>::is_matrix 
        >
        : binary_diversity_base<Left,Right>
    {
        using left_type = typename std::decay<Left>::type;
        using right_type = typename std::decay<Right>::type;
        typedef void                                           is_matrix;
        typedef void                                           rank_2;
        typedef typename row_dimension_of<left_type>::type     row_dimension;
        typedef typename column_dimension_of<right_type>::type col_dimension;

        template <std::size_t Row, std::size_t Column, typename Callable = boost::proto::callable >
        struct context : boost::proto::callable_context< const context<Row, Column, Callable> >
        {            
            typedef tag::matrix_product tag_t;
            typedef typename result_of::dot_product
                <
                    row<left_type,Row>
                  , column<right_type,Column> 
                >::type result_type;
            
            result_type operator()(tag_t, const left_type& l, const right_type& r ) const
            {
                return matrix_product_element<Row, Column>( l, r );
            }
        };
    };

}//namespace geometrix;

#endif//GEOMETRIX_LINEAR_ALGEBRA_BINARY_FUNCTIONS_MATRIX_PRODUCT_HPP
