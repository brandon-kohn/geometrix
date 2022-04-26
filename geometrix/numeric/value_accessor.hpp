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
#include <type_traits>

namespace geometrix{

    template <typename T, typename EnableIf=void>
    struct value_accessor;

    template <typename T>
    struct value_accessor<T, typename std::enable_if<std::is_arithemtic<T>::value>::type>
    {
        BOOST_STATIC_CONSTEXPR apply(T&& v) -> auto
        {
            return std::forward<T>(v);
        }
    };

    template <typename T>
    inline BOOST_CONSTEXPR access_value(T&& v) -> auto
    {
        return value_accessor<typename std::decay<T>::type>::apply(v);
    }

}//! namespace geometrix;

