//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_MATH_FUNCTIONS_HPP
#define GEOMETRIX_MATH_FUNCTIONS_HPP

#include <cmath>
#include <utility>

#include <geometrix/numeric/numeric_traits.hpp>
#include <geometrix/numeric/function_traits.hpp>
#include <geometrix/utility/assert.hpp>
#include <geometrix/tensor/tensor_access_policy.hpp>
#include <geometrix/arithmetic/arithmetic_promotion_policy.hpp>

#include <boost/typeof/typeof.hpp>
#include <boost/utility/result_of.hpp>

namespace geometrix {

    namespace result_of
    {
        template <typename LHS, typename RHS, typename LeftType = void, typename RightType = void>
        struct multiplies;

        template <typename LHS, typename RHS, typename LeftType = void, typename RightType = void>
        struct plus;

        template <typename LHS, typename RHS, typename LeftType = void, typename RightType = void>
        struct minus;

        template <typename LHS, typename RHS, typename LeftType = void, typename RightType = void>
        struct divides;
    }

    template <typename LHS, typename RHS, typename LeftType = void, typename RightType = void>
    struct multiplies;

    template <typename LHS, typename RHS>
    inline typename result_of::multiplies<LHS, RHS>::type do_multiplies(const LHS& lhs, const RHS& rhs)
    {
        return multiplies<LHS,RHS>()(lhs, rhs);
    }

    template <typename LHS, typename RHS, typename LeftType = void, typename RightType = void>
    struct plus;

    template <typename LHS, typename RHS>
    inline typename result_of::plus<LHS, RHS>::type do_plus(const LHS& lhs, const RHS& rhs)
    {
        return plus<LHS,RHS>()(lhs, rhs);
    }

    template <typename LHS, typename RHS, typename LeftType = void, typename RightType = void>
    struct minus;

    template <typename LHS, typename RHS>
    inline typename result_of::minus<LHS, RHS>::type do_minus(const LHS& lhs, const RHS& rhs)
    {
        return minus<LHS,RHS>()(lhs, rhs);
    }

    template <typename LHS, typename RHS, typename LeftType = void, typename RightType = void>
    struct divides;

    template <typename LHS, typename RHS>
    inline typename result_of::divides<LHS, RHS>::type do_divides(const LHS& lhs, const RHS& rhs)
    {
        return divides<LHS,RHS>()(lhs, rhs);
    }

    //! Compile time calculation of N^P (integral).
    template< int N, unsigned int P >
    struct power_c
    {
        static const boost::int64_t value = N * power_c<N, P - 1>::value;
    };

    template < int N >
    struct power_c<N, 0>
    {
        static const boost::int64_t value = 1;
    };

    template <typename T>
    inline T integral_pow(T radix, T exponent)
    {
        if (exponent < T(0))
            return T(0);

        T p(1);
        while (true)
        {
            if (exponent == T(0))
                break;

            p *= radix;
            --exponent;
        }

        return p;
    }

    template <typename T>
    inline decltype(std::declval<T>() * std::declval<T>()) square(T v) { return v*v; }

}//namespace geometrix;

#endif //GEOMETRIX_MATH_FUNCTIONS_HPP
