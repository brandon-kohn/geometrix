//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_LINEAR_ALGEBRA_UNARY_FUNCTION_INVERSE_HPP
#define GEOMETRIX_LINEAR_ALGEBRA_UNARY_FUNCTION_INVERSE_HPP

#include <geometrix/algebra/functions/unary_function.hpp>

namespace geometrix {

    namespace tag
    {
        struct inverse{};
    }
        
    //! Inverse of a Matrix.
    template <typename T>
    struct un_fun
        <
            geometrix::tag::inverse
          , T
          , typename geometric_traits<typename remove_const_ref<T>::type>::is_matrix 
        > 
        : diversity_base<T>
    {
        typedef void                                  is_matrix;
        typedef void                                  rank_2;
        typedef typename row_dimension_of<T>::type    row_dimension;
        typedef typename column_dimension_of<T>::type col_dimension;

        template <unsigned int Row, unsigned int Column>
        struct context : boost::proto::callable_context< const context<Row, Column> >
        {            
            typedef geometrix::tag::inverse tag;
        
            typedef typename result_of::determinant<T>::type result_type;

            result_type operator()(tag, const T& a) const
            {            
                return geometrix::inverse_at_index<Row, Column>( a );
            }
        };
    };
        
}//namespace geometrix;

#endif//GEOMETRIX_LINEAR_ALGEBRA_UNARY_FUNCTION_INVERSE_HPP
