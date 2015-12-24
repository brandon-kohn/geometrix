//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_LINEAR_ALGEBRA_TERMINALS_HPP
#define GEOMETRIX_LINEAR_ALGEBRA_TERMINALS_HPP
#pragma once

#include <geometrix/geometric_traits.hpp>
#include <geometrix/algebra/type_traits.hpp>

#include <boost/proto/core.hpp>
#include <boost/proto/debug.hpp>
#include <boost/proto/context.hpp>
#include <boost/proto/transform.hpp>
#include <boost/utility/result_of.hpp>

namespace geometrix {

    template <typename T, typename EnableIf = void>
    struct is_linear_algebra_terminal : boost::mpl::false_ 
    {};

    #define GEOMETRIX_LINEAR_ALGEBRA_TERMINAL( Trait, Type )       \
    template <typename T>                                          \
    struct is_linear_algebra_terminal                              \
    <                                                              \
        T                                                          \
      , typename Trait< typename remove_const_ref<T>::type >::Type \
    > : boost::mpl::true_                                          \
    {};                                                            \
    /***/

    GEOMETRIX_LINEAR_ALGEBRA_TERMINAL( geometric_traits, is_vector );
    GEOMETRIX_LINEAR_ALGEBRA_TERMINAL( tensor_traits, rank_0 );//scalars
    GEOMETRIX_LINEAR_ALGEBRA_TERMINAL( geometric_traits, is_point );
    GEOMETRIX_LINEAR_ALGEBRA_TERMINAL( geometric_traits, is_matrix );

    template < typename T, typename LinearAlgebraType = void >
    struct term_ 
    {
        BOOST_MPL_ASSERT_MSG
        (
            (false)
          , INVALID_LINEAR_ALGEBRA_TERMINAL_TYPE
          , (term_<T, LinearAlgebraType>)
        );
    };

    template <typename T>
    struct term_
        <
            T
          , typename geometric_traits<typename remove_const_ref<T>::type>::is_matrix 
        > 
        : geometric_traits<typename remove_const_ref<T>::type>
        , tensor_traits<typename remove_const_ref<T>::type>
    {
        template <std::size_t Row, std::size_t Column>
        struct context : boost::proto::callable_context< const context<Row, Column> >
        {            
            typedef typename geometrix::type_at<T, Row, Column>::type result_type;
            
            result_type operator()(boost::proto::tag::terminal, const T& a) const
            {            
                return geometrix::get<Row, Column>(a);
            }
        };
    };

    template <typename T>
    struct term_
        <
            T
          , typename geometric_traits<typename remove_const_ref<T>::type>::is_vector 
        > 
        : geometric_traits<typename remove_const_ref<T>::type>
        , tensor_traits<typename remove_const_ref<T>::type>
    {
        template <std::size_t Index>
        struct context : boost::proto::callable_context< const context<Index> >
        {            
            typedef typename geometrix::type_at<T, Index>::type result_type;
            
            result_type operator()(boost::proto::tag::terminal, const T& a) const
            {            
                return geometrix::get<Index>(a);
            }
        };
    };

    template <typename T>
    struct term_
        <
            T
          , typename geometric_traits<typename remove_const_ref<T>::type>::is_point 
        >
        : geometric_traits<typename remove_const_ref<T>::type>
        , tensor_traits<typename remove_const_ref<T>::type>
    {        
        template <std::size_t Index>
        struct context : boost::proto::callable_context< const context<Index> >
        {            
            typedef typename geometrix::type_at<T, Index>::type result_type;
            
            result_type operator()(boost::proto::tag::terminal, const T& a) const
            {            
                return geometrix::get<Index>(a);
            }
        };
    };

    template <typename T>
    struct term_
        <
            T
          , typename geometric_traits<typename remove_const_ref<T>::type>::is_scalar 
        > 
        : geometric_traits<typename remove_const_ref<T>::type>
        , tensor_traits<typename remove_const_ref<T>::type>
    {
        struct context : boost::proto::callable_context< const context >
        {            
            typedef T result_type;
            
            result_type operator()(boost::proto::tag::terminal, const T& a) const
            {            
                return a;
            }
        };
    };

}//namespace geometrix;

#endif//GEOMETRIX_LINEAR_ALGEBRA_TERMINALS_HPP
