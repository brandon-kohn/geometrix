//
//! Copyright © 2022
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include <boost/units/quantity.hpp>
#include <boost/units/cmath.hpp>
#include <geometrix/utility/tagged_quantity.hpp>
#include <geometrix/utility/tagged_quantity_cmath.hpp>
#include <type_traits>

namespace geometrix {

    struct abs_tag{};
    struct ceil_tag{};
    struct floor_tag{};
    struct exp_tag{};
    struct log_tag{};
    struct log10_tag{};
    struct sqrt_tag{};
    struct cos_tag{};
    struct sin_tag{};
    struct tan_tag{};
    struct acos_tag{};
    struct asin_tag{};
    struct atan_tag{};
    struct hypot_tag{};
    struct pow_tag{};
    struct atan2_tag{};
    
    template <typename Tag, typename T1, typename T2=void, typename T3=void, typename T4=void, typename EnableIF=void>
    struct math_function_traits;
    
    template <typename Tag, typename T>
    struct math_function_traits<Tag, T, typename std::enable_if<std::is_floating_point<T>::value>::type>
    {
        using result_type = typename std::decay<T>::type;
    };

    template <typename Tag, typename T>
    struct math_function_traits<Tag, T, typename std::enable_if<std::is_integral<T>::value>::type>
    {
        using result_type = double;
    };

}//! namespace geometrix;

