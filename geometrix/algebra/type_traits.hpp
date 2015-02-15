//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_LINEAR_ALGEBRA_TYPE_TRAITS_HPP
#define GEOMETRIX_LINEAR_ALGEBRA_TYPE_TRAITS_HPP

#include <geometrix/tensor/matrix_traits.hpp>
#include <geometrix/tensor/vector_traits.hpp>
#include <geometrix/tensor/scalar.hpp>
#include <geometrix/primitive/point_traits.hpp>

namespace geometrix {
    
    template <typename T, typename EnableIf=void>
    struct algebra_type_of
    {        
        BOOST_MPL_ASSERT_MSG
        (
            (false)
          , UNABLE_TO_DEDUCE_EXPRESSION_RESULT
          , (T)
        );
    };

    template <typename T>
    struct algebra_type_of<T, typename tensor_traits<typename remove_const_ref<T>::type>::rank_0 >
    {
        typedef scalar_tag type;
    };

    template <typename T>
    struct algebra_type_of<T, typename geometric_traits<typename remove_const_ref<T>::type>::is_vector >
    {
        typedef vector_tag type;
    };

    template <typename T>
    struct algebra_type_of<T, typename geometric_traits<typename remove_const_ref<T>::type>::is_point >
    {
        typedef point_tag type;
    };

    template <typename T>
    struct algebra_type_of<T, typename geometric_traits<typename remove_const_ref<T>::type>::is_matrix >
    {
        typedef matrix_tag type;
    };

}//namespace geometrix;

#endif//GEOMETRIX_LINEAR_ALGEBRA_TYPE_TRAITS_HPP
