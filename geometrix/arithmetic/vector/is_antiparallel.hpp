//
//! Copyright © 2020
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_VECTOR_MATH_IS_ANTIPARALLEL_HPP
#define GEOMETRIX_VECTOR_MATH_IS_ANTIPARALLEL_HPP
#pragma once

#include <geometrix/tensor/vector.hpp>
#include <geometrix/numeric/number_comparison_policy.hpp>
#include <geometrix/numeric/constants.hpp>
#include <geometrix/algebra/dot_product.hpp>
#include <geometrix/arithmetic/arithmetic_promotion_policy.hpp>

namespace geometrix {
    
    template <typename Vector1, typename Vector2, typename NumberComparisonPolicy>
    inline bool is_antiparallel(const Vector1& u, const Vector2& v, const NumberComparisonPolicy& cmp)
    {
		auto dp = dot_product(normalize(u), normalize(v));
        return cmp.equals(dp, constants::negative_one<decltype(dp)>()); 
    }

}//namespace geometrix;

#endif //! GEOMETRIX_VECTOR_MATH_IS_ANTIPARALLEL_HPP
