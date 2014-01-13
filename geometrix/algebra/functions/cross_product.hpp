//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_LINEAR_ALGEBRA_BINARY_FUNCTIONS_CROSS_PRODUCT_HPP
#define GEOMETRIX_LINEAR_ALGEBRA_BINARY_FUNCTIONS_CROSS_PRODUCT_HPP

#include <geometrix/algebra/functions/binary_function.hpp>

namespace geometrix { namespace algebra {

    namespace tag
    {
        typedef boost::proto::tag::bitwise_xor cross_product;        
    }//namespace tag;

    GEOMETRIX_LINEAR_ALGEBRA_BINARY_OP( tag::cross_product, is_vector, is_vector );
                
    //! Cross Product of Vectors
    template <typename Left, typename Right>
    struct binary_fn
        <
            tag::cross_product
          , Left
          , Right
          , typename geometric_traits<typename remove_const_ref<Left>::type>::is_vector
          , typename geometric_traits<typename remove_const_ref<Right>::type>::is_vector 
        >
        : binary_uniformity_base<Left,Right>
    {
        typedef void                                     is_vector;
        typedef void                                     rank_1;
        typedef typename dimension_of<Right>::type       dimension_type;
        typedef typename reference_frame_of<Right>::type reference_frame;//! Todo: This isn't properly calculated under transforms.
        typedef void                                     is_sequence;
        
        template <unsigned int Index, typename Callable = boost::proto::callable >
        struct context : boost::proto::callable_context< const context<Index, Callable> >
        {
            typedef typename result_of::cross_product_at_index<Left, Right, Index>::type result_type;

            result_type operator()(tag::cross_product, const Left& l, const Right& r ) const
            {
                return cross_product_at_index<Index>( l, r );
            }
        };
    };

}//namespace algebra;
}//namespace geometrix;

#endif//GEOMETRIX_LINEAR_ALGEBRA_BINARY_FUNCTIONS_CROSS_PRODUCT_HPP
