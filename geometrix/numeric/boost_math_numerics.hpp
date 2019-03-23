//
//! Copyright © 2019
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include <geometrix/space/affine_space_traits.hpp>
#include <geometrix/tensor/tensor_access_policy.hpp>
#include <boost/multiprecision/cpp_dec_float.hpp>

namespace geometrix {

    template <typename ... Args>
    struct numeric_traits<boost::multiprecision::number<Args...>>
    {
        typedef boost::multiprecision::number<Args...>          numeric_type;
        typedef typename boost::is_float<numeric_type>::type    is_float;
        typedef typename boost::is_integral<numeric_type>::type is_integral;
        typedef void                                            is_numeric;
        static const unsigned int digits = std::numeric_limits<numeric_type>::digits10;

        BOOST_MPL_ASSERT_MSG
        (
            (!(is_float::value && is_integral::value))
            , NUMERIC_TRAITS_CANNOT_DEFINE_FLOAT_AND_INTEGRAL
            , (boost::multiprecision::number<Args...>)
        );
    };

	/*
    template <typename ... Args>
    struct construction_policy<boost::multiprecision::number<Args...>>
    {
        template <typename... T>
        static boost::multiprecision::number<Args...> construct(T&&... n) 
		{ 
			return boost::multiprecision::number<Args...>(std::forward<T>(n)...);
		}
    };
	*/
}//namespace geometrix;

