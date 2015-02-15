//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_LINEAR_ALGEBRA_UNARY_FUNCTION_MAGNITUDE_HPP
#define GEOMETRIX_LINEAR_ALGEBRA_UNARY_FUNCTION_MAGNITUDE_HPP

#include <geometrix/algebra/functions/unary_function.hpp>
#include <geometrix/arithmetic/vector/magnitude.hpp>

namespace geometrix {

    namespace tag
    {
        struct magnitude{};
    }
    
    //! Magnitude of a vector.
    template <typename T>
    struct un_fun
        <
            geometrix::tag::magnitude
          , T
          , typename geometric_traits<typename remove_const_ref<T>::type>::is_vector 
        > 
    {
        typedef void is_scalar;
        typedef void rank_0;
        typedef void is_homogeneous;
        
        struct context : boost::proto::callable_context< const context >
        {            
            typedef geometrix::tag::magnitude tag;
            typedef typename result_of::magnitude<T>::type result_type;

            result_type operator()(tag, const T& a) const
            {            
                return geometrix::magnitude(a);
            }
        };
    };
    
}//namespace geometrix;

#endif//GEOMETRIX_LINEAR_ALGEBRA_UNARY_FUNCTION_MAGNITUDE_HPP
