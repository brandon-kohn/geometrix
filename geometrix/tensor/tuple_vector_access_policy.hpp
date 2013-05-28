//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_TUPLE_ACCESS_POLICY_HPP
#define GEOMETRIX_TUPLE_ACCESS_POLICY_HPP


#include <geometrix/tensor/tensor_access_policy.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/typeof/typeof.hpp>

namespace geometrix {

template <typename T>
struct tuple_vector_access_policy
{
    typedef typename remove_const_ref<T>::type tensor_type;

    template <unsigned int Index>
    struct type_at
    {        
        typedef typename boost::tuples::element<Index,tensor_type>::type type;
    };

    //! \brief compile time access if available for the vector.
    template <unsigned int Index>
    static typename type_at<Index>::type get( const T& collection ) 
    {
        return collection.get<Index>();
    }

    //! \brief compile time access if available for the vector.
    template <unsigned int Index>
    static void set( T& collection, const typename type_at<Index>::type& v ) 
    {
        collection.get<Index>() = v;
    }
};

}//namespace geometrix;

#endif//GEOMETRIX_TUPLE_ACCESS_POLICY_HPP
