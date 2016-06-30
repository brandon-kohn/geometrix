//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_ALGORITHM_ORIENTATION_HPP
#define GEOMETRIX_ALGORITHM_ORIENTATION_HPP

#include <geometrix/algorithm/orientation_enum.hpp>
#include <geometrix/arithmetic/vector.hpp>
#include <geometrix/algebra/expression.hpp>
#include <geometrix/tensor/numeric_sequence_compare.hpp>

namespace geometrix {
    
    //! Orientation test to check the orientation of B relative to A.
    //! @precondition A and B are vectors which share a common origin.
    template <typename Vector1, typename Vector2, typename NumberComparisonPolicy>
    inline orientation_type get_orientation( const Vector1& A, const Vector2& B, const NumberComparisonPolicy& cmp )
    {
        BOOST_CONCEPT_ASSERT((Vector2DConcept<Vector1>));
        BOOST_CONCEPT_ASSERT((Vector2DConcept<Vector2>));
        BOOST_CONCEPT_ASSERT((NumberComparisonPolicyConcept<NumberComparisonPolicy>));

        auto cross0 = get<0>(A) * get<1>(B);
        auto cross1 = get<1>(A) * get<0>(B);
        if( cmp.less_than( cross0, cross1 ) )
            return oriented_right;
        else if( cmp.greater_than( cross0, cross1 ) )
            return oriented_left;
        else
            return oriented_collinear;
    }

    //! Orientation test to check if point C is left, collinear, or right of the line formed by A-B.
    template <typename Point1, typename Point2, typename Point3, typename NumberComparisonPolicy>
    inline orientation_type get_orientation( const Point1& A, const Point2& B, const Point3& C, const NumberComparisonPolicy& cmp )
    {
        return get_orientation(B-A, C-A, cmp);
    }
    
}//! namespace geometrix;

#endif//GEOMETRIX_ALGORITHM_ORIENTATION_HPP
