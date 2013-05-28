//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_LINEAR_ALGEBRA_FUNCTIONS_TRANSPOSE_HPP
#define GEOMETRIX_LINEAR_ALGEBRA_FUNCTIONS_TRANSPOSE_HPP

#include <geometrix/algebra/functions/unary_function.hpp>

namespace geometrix { namespace algebra {

    namespace tag
    {
        struct determinant{};
    }
        
    //! Determinant of a Matrix.
    template <typename T>
    struct un_fun
        <
            algebra::tag::determinant
          , T
          , typename geometric_traits<typename remove_const_ref<T>::type>::is_matrix 
        > 
    {
        typedef void is_scalar;
        typedef void rank_0;
        typedef void is_homogeneous;

        struct context : boost::proto::callable_context< const context >
        {            
            typedef algebra::tag::determinant tag;
        
            typedef typename result_of::determinant<T>::type result_type;

            result_type operator()(tag, const T& a) const
            {            
                return geometrix::determinant( a );
            }
        };
    };
        
}//namespace algebra;
}//namespace geometrix;

#endif//GEOMETRIX_LINEAR_ALGEBRA_FUNCTIONS_TRANSPOSE_HPP
