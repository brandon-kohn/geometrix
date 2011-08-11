//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_FUNCTION_TRAITS_HPP
#define GEOMETRIX_FUNCTION_TRAITS_HPP

namespace geometrix {
    
//! Default function traits struct. 
//! NOTE: must be specialized for user functions.
template <typename Function, typename EnableIf=void>
struct function_traits
{};

//! \brief Tag to check if a type is a unary function.
template <typename Function, typename Enable = void>
struct is_unary_function : boost::false_type{};

template <typename Function>
struct is_unary_function<Function, typename function_traits<Function>::is_unary_function> : boost::true_type{};

}//namespace geometrix;

#endif //GEOMETRIX_FUNCTION_TRAITS_HPP
