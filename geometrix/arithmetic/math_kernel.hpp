//
//! Copyright © 2022
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include <boost/config.hpp>
#include <boost/units/cmath.hpp>
#include <geometrix/utility/tagged_quantity_cmath.hpp>

namespace geometrix {

    struct std_math_kernel
    {
        template<typename T>
        static auto exp(T&& q) -> auto
        {
            using std::exp;
            return exp(std::forward<T>(q));
        }

        template<typename T>
        static auto log(T&& q) -> auto
        {
            using std::log;
            return log(std::forward<T>(q));
        }

        template<typename T>
        static auto log10(T&& q) -> auto
        {
            using std::log10;
            return log10(std::forward<T>(q));
        }

        template <typename T>
        static auto sqrt(T&& q) -> auto
        {
            using std::sqrt;
            return sqrt(std::forward<T>(q));
        }

        template<typename T>
        static auto cos(T&& q) -> auto
        {
            using std::cos;
            return cos(std::forward<T>(q));
        }

        template<typename T>
        static auto sin(T&& q) -> auto
        {
            using std::sin;
            return sin(std::forward<T>(q));
        }

        template<typename T>
        static auto tan(T&& q) -> auto
        {
            using std::tan;
            return tan(std::forward<T>(q));
        }

        template<typename T>
        static auto acos(T&& q) -> auto
        {
            using std::acos;
            return acos(std::forward<T>(q));
        }

        template<typename T>
        static auto asin(T&& q) -> auto
        {
            using std::asin;
            return asin(std::forward<T>(q));
        }

        template<typename T>
        static auto atan(T&& q) -> auto
        {
            using std::atan;
            return atan(std::forward<T>(q));
        }

        template<typename T, typename U>
        static auto pow(T&& q1, U&& q2) -> auto
        {
            using std::pow;
            return pow(std::forward<T>(q1), std::forward<U>(q2));
        }
        
        template<typename T, typename U>
        static auto atan2(T&& y, U&& x) -> auto
        {
            using std::atan2;
            return atan2(std::forward<T>(y), std::forward<U>(x));
        }
    };

}//! namespace geometrix;

