//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include <geometrix/algorithm/orientation_enum.hpp>

namespace geometrix {

	namespace detail {
		template <typename X0, typename X1, typename NumberComparisonPolicy>
		inline orientation_type orientation(X0&& x0, X1&& x1, const NumberComparisonPolicy& cmp)
		{
			if(cmp.less_than(x0, x1))
				return oriented_right;
			else if(cmp.greater_than(x0,x1))
				return oriented_left;
			else
				return oriented_collinear;
		}
	}
    
    //! Orientation test to check the orientation of A relative to B. - NOTE: This is the reverse of get_orientation.
    //! @precondition A and B are vectors which share a common origin.
    template <typename Vector1, typename Vector2, typename NumberComparisonPolicy>
    inline orientation_type vector_vector_orientation(const Vector1& A, const Vector2& B, const NumberComparisonPolicy& cmp)
    {
        BOOST_CONCEPT_ASSERT((Vector2DConcept<Vector1>));
        BOOST_CONCEPT_ASSERT((Vector2DConcept<Vector2>));
        BOOST_CONCEPT_ASSERT((NumberComparisonPolicyConcept<NumberComparisonPolicy>));
        return detail::orientation(get<1>(A) * get<0>(B), get<0>(A) * get<1>(B), cmp);
    }

    //! Orientation test to check the orientation of B relative to A.
    //! @precondition A and B are vectors which share a common origin.
    template <typename Vector1, typename Vector2, typename NumberComparisonPolicy>
    inline orientation_type get_orientation(const Vector1& A, const Vector2& B, const NumberComparisonPolicy& cmp)
    {
        BOOST_CONCEPT_ASSERT((Vector2DConcept<Vector1>));
        BOOST_CONCEPT_ASSERT((Vector2DConcept<Vector2>));
        BOOST_CONCEPT_ASSERT((NumberComparisonPolicyConcept<NumberComparisonPolicy>));
        return detail::orientation(get<0>(A) * get<1>(B), get<1>(A) * get<0>(B), cmp);
    }

}//! namespace geometrix;

