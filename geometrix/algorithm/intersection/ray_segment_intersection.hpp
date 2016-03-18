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
template <typename Point, typename Vector, typename PointA, typename PointB, typename ArithmeticType, typename PointX, typename NumberComparisonPolicy>
inline intersection_type ray_segment_intersection( const Point& O, const Vector& v, const PointA& A, const PointB& B, ArithmeticType& t, PointX* xPoints, const NumberComparisonPolicy& cmp )
{       
    BOOST_CONCEPT_ASSERT((PointConcept<Point>));
    BOOST_CONCEPT_ASSERT((VectorConcept<Vector>));
    BOOST_CONCEPT_ASSERT((PointConcept<PointA>));
    BOOST_CONCEPT_ASSERT((PointConcept<PointB>));
    BOOST_CONCEPT_ASSERT((PointConcept<PointX>));
    intersection_type iType = e_invalid_intersection;
	typedef vector<ArithmeticType, 2> vector_type;

	vector_type v1 = O - A;
	vector_type v2 = B - A;
	vector_type v3 = left_normal<vector_type>(v);

	ArithmeticType denom = dot_product(v2, v3);

    //If denom is zero then ray and segment are parallel.
    if( cmp.equals( denom, 0 ) )
    {
		if (get_orientation(v, v2, cmp) != oriented_collinear)
			return e_non_crossing;

		vector_type vOB = B - O;
		ArithmeticType OA = dot_product(v, -v1);
		ArithmeticType OB = dot_product(v, vOB);

		//! Check if both are ahead on the ray.
		if (cmp.less_than(OA, 0))
		{
			if (cmp.less_than(OB, 0))
				return e_non_crossing;

			t = 0;
			if (xPoints)
			{
				assign(xPoints[0], O);
				assign(xPoints[1], B);
			}
		}
		else if( cmp.less_than(OB, 0) )
		{
			t = 0;
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
		
	ArithmeticType t1 = exterior_product_area(v2, v1);
	if (cmp.less_than(t1, 0))
		return e_non_crossing;
			
	ArithmeticType t2 = dot_product(v1, v3);
	if (cmp.less_than(t2, 0) || cmp.greater_than(t2, denom))
		return e_non_crossing;

	iType = (cmp.equals(t2, 0) || cmp.equals(t2, denom)) ? e_endpoint : e_crossing;

	t = t1 / denom;
	if( xPoints )
		assign(xPoints[0], O + t * v);

	return iType;
}

//! \brief Compute whether the ray defined by A->v intersects the specified segment.
template <typename Point, typename Vector, typename Segment, typename ArithmeticType, typename PointX, typename NumberComparisonPolicy>
inline intersection_type ray_segment_intersection( const Point& A, const Vector& v, const Segment& segment, ArithmeticType& t, PointX* xPoint, const NumberComparisonPolicy& compare )
{   
    return ray_segment_intersection( A, v, get_start( segment ), get_end( segment ), t, xPoint, compare );
}

//! \brief Compute whether the ray defined by A->v intersects the specified segment (A,B).
template <typename Point, typename Vector, typename PointA, typename PointB, typename NumberComparisonPolicy>
inline intersection_type ray_segment_intersection(const Point& O, const Vector& v, const PointA& A, const PointB& B, const NumberComparisonPolicy& cmp)
{
	BOOST_CONCEPT_ASSERT((PointConcept<Point>));
	BOOST_CONCEPT_ASSERT((VectorConcept<Vector>));
	BOOST_CONCEPT_ASSERT((PointConcept<PointA>));
	BOOST_CONCEPT_ASSERT((PointConcept<PointB>));
	typedef typename geometric_traits<Point>::arithmetic_type arithmetic_type;
	intersection_type iType = e_invalid_intersection;
	typedef vector<arithmetic_type, 2> vector_type;

	vector_type v1 = O - A;
	vector_type v2 = B - A;
	vector_type v3 = left_normal<vector_type>(v);

	auto denom = dot_product(v2, v3);

	//If denom is zero then ray and segment are parallel.
	if (cmp.equals(denom, 0))
	{
		if (get_orientation(v, v2, cmp) != oriented_collinear)
			return e_non_crossing;

		vector_type vOB = B - O;
		auto OA = dot_product(v, -v1);
		auto OB = dot_product(v, vOB);

		//! Check if both are ahead on the ray.
		if (cmp.less_than(OA, 0))
		{
			if (cmp.less_than(OB, 0))
				return e_non_crossing;
		}

		return e_overlapping;
	}

	auto t1 = exterior_product_area(v2, v1);
	if (cmp.less_than(t1, 0))
		return e_non_crossing;

	auto t2 = dot_product(v1, v3);
	if (cmp.less_than(t2, 0) || cmp.greater_than(t2, denom))
		return e_non_crossing;

	iType = (cmp.equals(t2, 0) || cmp.equals(t2, denom)) ? e_endpoint : e_crossing;
	
	return iType;
}

//! \brief Compute whether the ray defined by A->v intersects the specified segment.
template <typename Point, typename Vector, typename Segment, typename NumberComparisonPolicy>
inline intersection_type ray_segment_intersection(const Point& A, const Vector& v, const Segment& segment, const NumberComparisonPolicy& compare)
{
	return ray_segment_intersection(A, v, get_start(segment), get_end(segment), compare);
}

}//namespace geometrix;

#endif //GEOMETRIX_ALGORITHM_RAY_SEGMENT_INTERSECTION_HPP
