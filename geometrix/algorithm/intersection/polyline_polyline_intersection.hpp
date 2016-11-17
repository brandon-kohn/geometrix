//
//! Copyright © 2008-2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_POLYLINE_POLYLINE_INTERSECTION_HPP
#define GEOMETRIX_POLYLINE_POLYLINE_INTERSECTION_HPP

#include <geometrix/algorithm/intersection/segment_polyline_intersection.hpp>
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
template <typename Polyline1, typename Polyline2, typename Visitor, typename NumberComparisonPolicy>
inline bool polyline_polyline_intersect( const Polyline1& A, const Polyline2& B, Visitor&& visitor, const NumberComparisonPolicy& cmp )
{
	BOOST_CONCEPT_ASSERT((PointSequenceConcept<Polyline1>));
	BOOST_CONCEPT_ASSERT((PointSequenceConcept<Polyline2>));
	BOOST_CONCEPT_ASSERT((NumberComparisonPolicyConcept<NumberComparisonPolicy>));
	using access1 = point_sequence_traits<Polyline1>;
	using access2 = point_sequence_traits<Polyline2>;
	using length_t = typename select_arithmetic_type_from_sequences<typename access1::point_type, typename access2::point_type>::type;
	using point_type = point<length_t, 2>;
	
	bool intersected = false;
	for (std::size_t i1 = 0, j1 = 1; j1 < access1::size(A); i1 = j1++)
	{
		auto iVisitor = [&](intersection_type iType, std::size_t i, std::size_t j, const point_type& x1, const point_type& x2) -> bool
		{
			if (iType == e_non_crossing)
				return false;
			
			intersected = true;
	
			return visitor(iType, i1, j1, i, j, x1, x2);
		};

		bool stop = segment_polyline_intersect(access1::get_point(A, i1), access1::get_point(A, j1), B, iVisitor, cmp);
		if (stop)
			return intersected;
	}

	return intersected;
}

}//namespace geometrix;

#endif //GEOMETRIX_POLYLINE_POLYLINE_INTERSECTION_HPP
