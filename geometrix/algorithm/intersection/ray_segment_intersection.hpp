//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_ALGORITHM_RAY_SEGMENT_INTERSECTION_HPP
#define GEOMETRIX_ALGORITHM_RAY_SEGMENT_INTERSECTION_HPP

#include <geometrix/algorithm/linear_components_intersection.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/algebra/dot_product.hpp>
#include <geometrix/algebra/exterior_product.hpp>
#include <geometrix/algebra/expression.hpp>
#include <geometrix/utility/utilities.hpp>

/////////////////////////////////////////////////////////////////////////////
//
// NAMESPACE
//
/////////////////////////////////////////////////////////////////////////////
namespace geometrix {

//! \brief Compute whether the ray defined by A->v intersects the specified segment (A,B).
template <typename Point, typename UnitVector, typename PointA, typename PointB, typename Length, typename PointX, typename NumberComparisonPolicy>
inline intersection_type ray_segment_intersection( const Point& O, const UnitVector& v, const PointA& A, const PointB& B, Length& t, PointX* xPoints, const NumberComparisonPolicy& cmp )
{       
    BOOST_CONCEPT_ASSERT((Point2DConcept<Point>));
    BOOST_CONCEPT_ASSERT((Vector2DConcept<UnitVector>));
    BOOST_CONCEPT_ASSERT((Point2DConcept<PointA>));
    BOOST_CONCEPT_ASSERT((Point2DConcept<PointB>));
    BOOST_CONCEPT_ASSERT((Point2DConcept<PointX>));
    intersection_type iType = e_invalid_intersection;

	using length_t = typename select_arithmetic_type_from_sequences<Point, PointA>::type;
	using area_t = decltype(std::declval<length_t>() * std::declval<length_t>());
	using dimensionless_t = decltype(std::declval<length_t>() / std::declval<length_t>());
	using vector_t = vector<length_t, 2>;
	using unit_vector_t = vector<dimensionless_t, 2>;
	
	vector_t v1 = O - A;
	vector_t v2 = B - A;
	auto v3 = left_normal<unit_vector_t>(v);

	auto denom = scalar_projection(v2, v3);

    //If denom is zero then ray and segment are parallel.
    if( cmp.equals( denom, constants::zero<length_t>() ) )
    {
		if (get_orientation(A, B, O, cmp) != oriented_collinear)
			return e_non_crossing;

		vector_t vOB = B - O;
		auto OA = scalar_projection(-v1, v);
		auto OB = scalar_projection(vOB, v);

		//! Check if both are ahead on the ray.
		if (cmp.less_than(OA, constants::zero<length_t>()))
		{
			if (cmp.less_than(OB, constants::zero<length_t>()))
				return e_non_crossing;

			t = constants::zero<length_t>();
			if (xPoints)
			{
				assign(xPoints[0], O);
				assign(xPoints[1], B);
			}
		}
		else if( cmp.less_than(OB, constants::zero<length_t>()) )
		{
			t = constants::zero<length_t>();
			if (xPoints)
			{
				assign(xPoints[0], O);
				assign(xPoints[1], A);
			}
		}
		else
		{
			if (OA < OB)
			{
				t = OA;
				if (xPoints)
				{
					assign(xPoints[0], A);
					assign(xPoints[1], B);
				}
			} 
			else
			{
				t = OB;
				if (xPoints)
				{
					assign(xPoints[0], B);
					assign(xPoints[1], A);
				}
			}
		}
		
		return e_overlapping;
    }

	auto t1 = exterior_product_area(v2, v1) / denom;
	if (cmp.less_than(t1, constants::zero<decltype(t1)>()))
		return e_non_crossing;
			
	auto t2 = scalar_projection(v1, v3) / denom;
	if (cmp.less_than(t2, constants::zero<decltype(t2)>()) || cmp.greater_than(t2, constants::one<decltype(t2)>()))
		return e_non_crossing;

	iType = (cmp.equals(t2, constants::zero<decltype(t2)>()) || cmp.equals(t2, constants::one<decltype(t2)>())) ? e_endpoint : e_crossing;

	t = t1;
	if( xPoints )
		assign(xPoints[0], O + t * v);

	return iType;
}

//! \brief Compute whether the ray defined by A->v intersects the specified segment.
template <typename Point, typename UnitVector, typename Segment, typename Length, typename PointX, typename NumberComparisonPolicy>
inline intersection_type ray_segment_intersection( const Point& A, const UnitVector& v, const Segment& segment, Length& t, PointX* xPoint, const NumberComparisonPolicy& compare )
{   
    return ray_segment_intersection( A, v, get_start( segment ), get_end( segment ), t, xPoint, compare );
}

//! \brief Compute whether the ray defined by A->v intersects the specified segment (A,B).
template <typename Point, typename UnitVector, typename PointA, typename PointB, typename NumberComparisonPolicy>
inline intersection_type ray_segment_intersection(const Point& O, const UnitVector& v, const PointA& A, const PointB& B, const NumberComparisonPolicy& cmp)
{
	BOOST_CONCEPT_ASSERT((Point2DConcept<Point>));
	BOOST_CONCEPT_ASSERT((Vector2DConcept<UnitVector>));
	BOOST_CONCEPT_ASSERT((Point2DConcept<PointA>));
	BOOST_CONCEPT_ASSERT((Point2DConcept<PointB>));
	typedef typename geometric_traits<Point>::arithmetic_type arithmetic_type;
	intersection_type iType = e_invalid_intersection;
	
	using length_t = typename select_arithmetic_type_from_sequences<Point, PointA>::type;
	using dimensionless_t = decltype(std::declval<length_t>() / std::declval<length_t>());
	using vector_t = vector<length_t, 2>;
	using unit_vector_t = vector<dimensionless_t, 2>;

	vector_t v1 = O - A;
	vector_t v2 = B - A;
	auto v3 = left_normal<unit_vector_t>(v);

	auto denom = scalar_projection(v2, v3);

	//If denom is zero then ray and segment are parallel.
	if (cmp.equals(denom, constants::zero<decltype(denom)>()))
	{
		if (get_orientation(v, v2, cmp) != oriented_collinear)
			return e_non_crossing;

		vector_t vOB = B - O;
		auto OA = scalar_projection(-v1, v);
		auto OB = scalar_projection(vOB, v);

		//! Check if both are ahead on the ray.
		if (cmp.less_than(OA, constants::zero<decltype(OA)>()))
		{
			if (cmp.less_than(OB, constants::zero<decltype(OB)>()))
				return e_non_crossing;
		}

		return e_overlapping;
	}

	auto t1 = exterior_product_area(v2, v1) / denom;
	if (cmp.less_than(t1, constants::zero<decltype(t1)>()))
		return e_non_crossing;

	auto t2 = scalar_projection(v1, v3) / denom;
	if (cmp.less_than(t2, constants::zero<decltype(t2)>()) || cmp.greater_than(t2, constants::one<decltype(t2)>()))
		return e_non_crossing;

	iType = (cmp.equals(t2, constants::zero<decltype(t2)>()) || cmp.equals(t2, constants::one<decltype(t2)>())) ? e_endpoint : e_crossing;
	
	return iType;
}

//! \brief Compute whether the ray defined by A->v intersects the specified segment.
template <typename Point, typename UnitVector, typename Segment, typename NumberComparisonPolicy>
inline intersection_type ray_segment_intersection(const Point& A, const UnitVector& v, const Segment& segment, const NumberComparisonPolicy& compare)
{
	return ray_segment_intersection(A, v, get_start(segment), get_end(segment), compare);
}

}//namespace geometrix;

#endif //GEOMETRIX_ALGORITHM_RAY_SEGMENT_INTERSECTION_HPP
