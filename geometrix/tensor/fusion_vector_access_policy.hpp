//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_FUSION_VECTOR_ACCESS_POLICY_HPP
#define GEOMETRIX_FUSION_VECTOR_ACCESS_POLICY_HPP

#include <geometrix/tensor/tensor_access_policy.hpp>

#include <boost/fusion/sequence.hpp>

namespace geometrix {

template <typename T>
struct fusion_vector_access_policy
{
    template <std::size_t Index>
    struct type_at
    {
        typedef typename remove_const_ref<T>::type tensor_type;
        typedef typename remove_const_ref
            <
                typename boost::fusion::result_of::at_c<tensor_type, Index>::type
            >::type type;
    };

    template <std::size_t Index>
    static typename type_at<Index>::type get( const T& collection ) 
    {
        typedef typename remove_const_ref<T>::type tensor_type;
        BOOST_CONCEPT_ASSERT((DimensionContainsIndexConcept<boost::fusion::result_of::size<tensor_type>,Index>));
        return boost::fusion::at_c<Index>( collection );
    }

    template <std::size_t Index>
    static void set( T& collection, typename boost::call_traits<typename type_at<Index>::type>::param_type v ) 
    {
        typedef typename remove_const_ref<T>::type tensor_type;
        BOOST_CONCEPT_ASSERT((DimensionContainsIndexConcept<boost::fusion::result_of::size<tensor_type>,Index>));
        boost::fusion::at_c<Index>( collection ) = v;
    }
};

template <typename T>
struct fusion_vector_member_function_access_policy
{
    template <std::size_t Index>
    struct type_at
    {
        typedef typename remove_const_ref<T>::type                                tensor_type;
        typedef typename boost::fusion::result_of::at_c<tensor_type, Index>::type proxy_type;
        typedef typename remove_const_ref<typename proxy_type::return_type>::type type;
    };

    template <std::size_t Index>
    static typename type_at<Index>::type get( const T& collection ) 
    {
        typedef typename remove_const_ref<T>::type tensor_type;
        BOOST_CONCEPT_ASSERT((DimensionContainsIndexConcept<boost::fusion::result_of::size<tensor_type>,Index>));
        return boost::fusion::at_c<Index>( collection );
    }

    template <std::size_t Index>
    static void set( T& collection, typename boost::call_traits<typename type_at<Index>::type>::param_type v ) 
    {
        typedef typename remove_const_ref<T>::type tensor_type;
        BOOST_CONCEPT_ASSERT((DimensionContainsIndexConcept<boost::fusion::result_of::size<tensor_type>,Index>));
        boost::fusion::at_c<Index>( collection ) = v;
    }
};

}//namespace geometrix;

#endif//GEOMETRIX_FUSION_VECTOR_ACCESS_POLICY_HPP
