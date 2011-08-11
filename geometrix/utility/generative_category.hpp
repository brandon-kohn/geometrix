//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_GENERATIVE_CATEGORY_HPP
#define GEOMETRIX_GENERATIVE_CATEGORY_HPP

#include <boost/utility.hpp>
#include <boost/type_traits.hpp>

namespace geometrix {

    template <typename T, typename EnableIf=void>
    struct is_compile_time : boost::false_type{};
    template <typename T>
    struct is_compile_time<T, typename T::compile_time_category > : boost::true_type
    {};

    template <typename T, typename EnableIf=void>
    struct is_run_time : boost::false_type{};
    template <typename T>
    struct is_run_time<T, typename T::run_time_category > : boost::true_type
    {};

}//namespace geometrix;

#endif//GEOMETRIX_ACCESS_CATEGORY_HPP
