//
//! Copyright © 2018
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_IS_NULL_HPP
#define GEOMETRIX_IS_NULL_HPP

#include <boost/fusion/include/all.hpp>
#include <geometrix/numeric/constants.hpp>

namespace geometrix {
       
    namespace detail{
        struct is_zero
        {
            template <typename T>
            bool operator()(const T& v)
            {
                return v == constants::zero<typename std::decay<T>::type>();
            }
        };
    }//! namespace detail;

    template <typename Vector>
    inline bool is_null(const Vector& v)
    {
        return boost::fusion::all(v, detail::is_zero()); 
    }
}//namespace geometrix;

#endif //GEOMETRIX_IS_NULL_HPP
