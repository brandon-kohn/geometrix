//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_VECTOR_MATH_BISECT_HPP
#define GEOMETRIX_VECTOR_MATH_BISECT_HPP
#pragma once

#include <geometrix/arithmetic/vector/is_antiparallel.hpp>
#include <geometrix/arithmetic/vector/perp.hpp>
#include <geometrix/algorithm/orientation.hpp>
#include <geometrix/utility/assert.hpp>

namespace geometrix {
    
    namespace result_of
    {
        template <typename Vector1, typename Vector2, typename EnableIf = void>
        struct bisect 
        {
            using dimensionless_t = typename dimensionless_type_of<Vector1>::type;
            using type = vector<dimensionless_t, dimension_of<Vector1>::value>;
        };
    }//! namespace result_of;

	//! Find the bisect between two vectors whose winding (rotational) progression is counter-clockwise from lo to hi.
    template <typename Vector1, typename Vector2>
    inline typename result_of::bisect<Vector1, Vector2>::type bisect(const Vector1& lo, const Vector2& hi)
    {
        using dimensionless_t = typename dimensionless_type_of<Vector1>::type;
		auto nlo = normalize(lo);
		auto nhi = normalize(hi);
		auto dp = dot_product(nlo, nhi);
		if (dp > constants::negative_one<decltype(dp)>() && dp < constants::one<decltype(dp)>()) 
		{
			auto one_half = constants::one_half<dimensionless_t>();
			auto orientation = get_orientation(lo, hi, direct_comparison_policy());
			if(orientation != oriented_right)
				return normalize(one_half * (lo + hi));
			return normalize(one_half * (-lo - hi));
		}
		else if (dp == constants::negative_one<decltype(dp)>())
			return right_normal(nhi);

		return -hi;
    }
	
	//! Find the bisect between two vectors whose winding (rotational) progression is counter-clockwise from lo to hi.
	//! \precondition - lo and hi must not be parallel or anti-parallel (the angle between lo,hi is not a multiple of pi.)
    template <typename Vector1, typename Vector2>
    inline typename result_of::bisect<Vector1, Vector2>::type bisect_not_antiparallel(const Vector1& lo, const Vector2& hi)
    {
		GEOMETRIX_ASSERT(!is_antiparallel(lo, hi, direct_comparison_policy()));
        using dimensionless_t = typename dimensionless_type_of<Vector1>::type;
        auto one_half = constants::one_half<dimensionless_t>();
		auto orientation = get_orientation(lo, hi, direct_comparison_policy());
		if(orientation != oriented_right)
	        return normalize(one_half * (lo + hi));
		else
	        return normalize(one_half * (-lo - hi));
    }

}//! namespace geometrix;

#endif //GEOMETRIX_VECTOR_MATH_BISECT_HPP
