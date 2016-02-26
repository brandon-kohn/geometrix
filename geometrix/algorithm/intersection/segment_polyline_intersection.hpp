//
//! Copyright © 2008-2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_SEGMENT_POLYLINE_INTERSECTION_HPP
#define GEOMETRIX_SEGMENT_POLYLINE_INTERSECTION_HPP

#include <geometrix/algorithm/intersection/segment_segment_intersection.hpp>
#include <geometrix/primitive/point_sequence_traits.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/arithmetic/arithmetic_promotion_policy.hpp>

/////////////////////////////////////////////////////////////////////////////
//
// NAMESPACE
//
/////////////////////////////////////////////////////////////////////////////
namespace geometrix {
	
//! \brief Compute whether the segment defined by A->B intersects a polyline.
//! The type Visitor must define bool operator()(intersection_type, size_t i, size_t j, point<arithmetic_type_of<PointA,PointB>::type, 2>[2]) which processes 
//! an intersection event between the segment and the i-j segment of the polyline 
//! along with the resulting intersection points. The return value is whether the algorithm should cease.
template <typename PointA, typename PointB, typename Polyline, typename Visitor, typename NumberComparisonPolicy>
inline bool segment_polyline_intersect( const PointA& A, const PointB& B, const Polyline& polyline, Visitor&& visitor, const NumberComparisonPolicy& cmp )
{
	BOOST_CONCEPT_ASSERT( (Point2DConcept<PointA>) );
	BOOST_CONCEPT_ASSERT( (Point2DConcept<PointB>) );
	BOOST_CONCEPT_ASSERT( (PointSequenceConcept<Polyline>) );
	BOOST_CONCEPT_ASSERT( (NumberComparisonPolicyConcept<NumberComparisonPolicy>) );
	typedef point_sequence_traits<Polyline> access;
	typedef typename select_arithmetic_type_from_sequences<PointA, PointB>::type arithmetic_type;
	typedef point<arithmetic_type, 2> point_type;
	bool intersected = false;
	for (std::size_t i = 0, j = 1; j < access::size(polyline); i = j++)
	{
		point_type xPoints[2];
		auto iType = segment_segment_intersection(A, B, access::get_point(polyline, i), access::get_point(polyline, j), xPoints, cmp);
		if (iType != e_non_crossing)
		{
			bool stop = visitor(iType, i, j, xPoints[0], xPoints[1]);
			if (stop)
				return true;
			intersected = true;
		}
	}

	return intersected;
}

//! \brief Compute whether the segment defined by A->B intersects the polyline defined by t0, t1, t2.
template <typename Segment, typename Polyline, typename Visitor, typename NumberComparisonPolicy>
inline bool segment_polyline_intersect( const Segment& segment, const Polyline& polyline, Visitor&& visitor, const NumberComparisonPolicy& cmp)
{   
	return segment_polyline_intersect( get_start( segment ), get_end( segment ), polyline, std::forward<Visitor>(visitor), cmp );
}

}//namespace geometrix;

#endif //GEOMETRIX_SEGMENT_POLYLINE_INTERSECTION_HPP
