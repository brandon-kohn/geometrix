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
#include <geometrix/algorithm/linear_components_intersection.hpp>

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

	//! Calculate whether the segment p1->p2 intersects p3-p4 and how that intersection is oriented.
    template <typename Point1, typename Point2, typename Point3, typename Point4, typename NumberComparisonPolicy>
    inline intersection_type oriented_intersection(const Point1& p1, const Point2& p2, const Point3& p3, const Point4& p4, const NumberComparisonPolicy& cmp)
    {
        auto p1OrientationLine34 = get_orientation(p3, p4, p1, cmp);
        auto p2OrientationLine34 = get_orientation(p3, p4, p2, cmp);

		if (p1OrientationLine34 != oriented_collinear && p2OrientationLine34 != oriented_collinear)
		{
			auto p3OrientationLine12 = get_orientation(p1, p2, p3, cmp);
			auto p4OrientationLine12 = get_orientation(p1, p2, p4, cmp);

			if (p1OrientationLine34 >= oriented_collinear && p2OrientationLine34 <= oriented_collinear)
			{
				if (p3OrientationLine12 * p4OrientationLine12 <= oriented_collinear)
					return e_intersect_left_right;
			}
			else if (p1OrientationLine34 <= oriented_collinear && p2OrientationLine34 >= oriented_collinear)
			{
				if (p3OrientationLine12 * p4OrientationLine12 <= oriented_collinear)
					return e_intersect_right_left;
			}
		}
		else 
		{
			//! At this point, the points are collinear and thus either overlap or intersect soley at an endpoint.
            bool p3isBetweenp1p2 = is_between( p1, p2, p3, false, cmp );
            bool p4isBetweenp1p2 = is_between( p1, p2, p4, false, cmp );

            if ( p3isBetweenp1p2 && p4isBetweenp1p2 )
                return e_overlapping;

            bool p1isBetweenp3p4 = is_between( p3, p4, p1, false, cmp );
            bool p2isBetweenp3p4 = is_between( p3, p4, p2, false, cmp );

            if ( p1isBetweenp3p4 && p2isBetweenp3p4 )
                return e_overlapping;

            if( p3isBetweenp1p2 && p2isBetweenp3p4 )
                return e_overlapping;

            if( p3isBetweenp1p2 && p1isBetweenp3p4 )
                return e_overlapping;

            if( p4isBetweenp1p2 && p2isBetweenp3p4 )
                return e_overlapping;

            if( p4isBetweenp1p2 && p1isBetweenp3p4 )
                return e_overlapping;

            bool originEqualsp3 = numeric_sequence_equals( p1, p3, cmp );
            bool destinationEqualsp4 = numeric_sequence_equals( p2, p4, cmp );
            bool originEqualsp4 = numeric_sequence_equals( p1, p4, cmp );
            bool destinationEqualsp3 = numeric_sequence_equals( p2, p3, cmp );
            if( (originEqualsp3 && destinationEqualsp4) || (originEqualsp4 && destinationEqualsp3) )
                return e_overlapping;

            if(originEqualsp3)
                return e_endpoint;

            if(originEqualsp4)
                return e_endpoint;

            if(destinationEqualsp3)
                return e_endpoint;

            if(destinationEqualsp4)
                return e_endpoint;
		}

        return e_non_crossing;
    }
}//! namespace geometrix;

#endif//GEOMETRIX_ALGORITHM_ORIENTATION_HPP
