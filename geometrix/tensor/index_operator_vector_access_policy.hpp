//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_INDEX_OPERATOR_ACCESS_POLICY_HPP
#define GEOMETRIX_INDEX_OPERATOR_ACCESS_POLICY_HPP

#include <geometrix/tensor/tensor_access_policy.hpp>
#include <boost/typeof/typeof.hpp>

namespace geometrix {

template <typename T>
struct index_operator_vector_access_policy
{
    typedef void run_time_access;

    template <std::size_t Index>
    struct type_at
    {
        typedef typename remove_const_ref<T>::type tensor_type;
        //typedef BOOST_TYPEOF_TPL( ((tensor_type*)0)->operator[](Index) ) type;
		typedef typename remove_const_ref<decltype( ((tensor_type*)0)->operator[](Index) )>::type type;
    };

    //! \brief run-time access method if the collection supports it.
    static typename type_at<0>::type get( const T& collection, std::size_t index  ) 
    {
        return collection[ index ];
    }

    //! \brief run-time access method if the collection supports it.
    static void set( T& collection, std::size_t index, typename boost::call_traits<typename type_at<0>::type>::param_type v  ) 
    {
        collection[ index ] = v;
    }

    //! \brief compile time access if available for the collection.
    template <std::size_t Index>
    static typename type_at<Index>::type get( const T& collection ) 
    {
        return collection[ Index ];
    }

    //! \brief compile time access if available for the collection.
    template <std::size_t Index>
	static void set( T& collection, typename boost::call_traits<typename type_at<0>::type>::param_type v )
    {
        collection[ Index ] = v;
    }
};

}// namespace geometrix;

#endif//GEOMETRIX_INDEX_OPERATOR_ACCESS_POLICY_HPP
