//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_LINEAR_ALGEBRA_UNARY_FUNCTION_NORMALIZE_HPP
#define GEOMETRIX_LINEAR_ALGEBRA_UNARY_FUNCTION_NORMALIZE_HPP

#include <geometrix/algebra/functions/unary_function.hpp>
#include <geometrix/arithmetic/vector/normalize_at_index.hpp>

namespace geometrix { namespace algebra {

    namespace tag
    {
        struct normalize{};
    }
    
    //! Normalize a vector.
    template <typename T>
    struct unary_fn
        <
            algebra::tag::normalize
          , T
          , typename geometric_traits<typename remove_const_ref<T>::type>::is_vector 
        > 
        : uniformity_base<T>
    {
        typedef void                           is_vector;
        typedef void                           rank_1;
        typedef typename dimension_of<T>::type dimension_type;
        typedef void                           is_sequence;
        typedef void                           is_numeric_sequence;
        typedef void                           is_coordinate_sequence;

        template <unsigned int Index>
        struct context : boost::proto::callable_context< const context<Index> >
        {            
            typedef algebra::tag::normalize tag;
            
            typedef typename result_of::normal_at_index<Index, T>::type result_type;

            result_type operator()(tag, const T& a) const
            {            
                return normal_at_index<Index>( a ); 
            }
        };    
    };
    
}//namespace algebra;
}//namespace geometrix;

#endif//GEOMETRIX_LINEAR_ALGEBRA_UNARY_FUNCTION_NORMALIZE_HPP
