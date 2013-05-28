//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_NULL_ACCESS_POLICY_HPP
#define GEOMETRIX_NULL_ACCESS_POLICY_HPP


#include <geometrix/tensor/tensor_access_policy.hpp>

namespace geometrix {

//! Access policy for 0 order tensors.
template <typename T>
struct null_access_policy
{
    struct type_at
    {
        typedef T type;
    };

    static const T& get( const T& t ) 
    {
        return t;
    }

    static void set( T& t, const T& v) 
    {
        t=v;
    }
};

}//namespace geometrix;

#endif//GEOMETRIX_NULL_ACCESS_POLICY_HPP
