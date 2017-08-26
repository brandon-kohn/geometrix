//
//! Copyright © 2017
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_LINEAR_ALGEBRA_FUNCTIONS_TRACE_HPP
#define GEOMETRIX_LINEAR_ALGEBRA_FUNCTIONS_TRACE_HPP

#include <geometrix/algebra/functions/unary_function.hpp>
#include <geometrix/arithmetic/matrix/trace.hpp>

namespace geometrix {

    namespace tag
    {
        struct trace{};
    }
        
    //! Determinant of a Matrix.
    template <typename T>
    struct un_fun
        <
            geometrix::tag::trace
          , T
          , typename geometric_traits<typename remove_const_ref<T>::type>::is_matrix
        > 
    {
        typedef void is_scalar;
        typedef void rank_0;
        typedef void is_homogeneous;

        struct context : boost::proto::callable_context< const context >
        {
            typedef geometrix::tag::trace tag;
        
            typedef typename result_of::trace<T>::type result_type;

            result_type operator()(tag, const T& a) const
            {
                return geometrix::trace( a );
            }
        };
    };
}//namespace geometrix;

#endif//GEOMETRIX_LINEAR_ALGEBRA_FUNCTIONS_TRACE_HPP
