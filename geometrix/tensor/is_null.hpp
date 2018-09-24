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
#include <geometrix/numeric/number_comparison_policy.hpp>

namespace geometrix {
       
    namespace detail{
        template <typename NumberComparisonPolicy>
        struct is_zero : NumberComparisonPolicy
        {
            is_zero() = default;
            is_zero(const NumberComparisonPolicy& cmp)
                : NumberComparisonPolicy(cmp)
            {}

            template <typename T>
            bool operator()(const T& v)
            {
                return NumberComparisonPolicy::equals(v, constants::zero<typename std::decay<T>::type>());
            }
        };
    }//! namespace detail;

    template <typename Vector>
    inline bool is_null(const Vector& v)
    {
        return boost::fusion::all(v, detail::is_zero<direct_comparison_policy>()); 
    }

    //! Bounding box test between two segments.
    template <typename Vector, typename NumberComparisonPolicy>
    inline bool is_null(const Vector& v, const NumberComparisonPolicy& cmp)
    {
        return boost::fusion::all(v, detail::is_zero<NumberComparisonPolicy>(cmp)); 
    }
}//namespace geometrix;

#endif //GEOMETRIX_IS_NULL_HPP

