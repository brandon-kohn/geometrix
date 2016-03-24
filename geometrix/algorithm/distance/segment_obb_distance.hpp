//
//! Copyright © 2008-2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_ALGORITHM_SEGMENT_OBB_DISTANCE_HPP
#define GEOMETRIX_ALGORITHM_SEGMENT_OBB_DISTANCE_HPP
#pragma once

#include <geometrix/primitive/oriented_bounding_box.hpp>
#include <geometrix/algorithm/euclidean_distance.hpp>

namespace geometrix {
	
	template <typename Segment, typename PointOBB, typename VectorOBB, typename NumberComparisonPolicy>
	inline typename geometric_traits<typename geometric_traits<Segment>::point_type>::arithmetic_type segment_obb_distance_sqrd(const Segment& seg, const oriented_bounding_box<PointOBB, VectorOBB>& obb, const NumberComparisonPolicy& cmp)
	{
		typedef typename geometric_traits<Segment>::point_type point_type;
		typedef typename geometric_traits<point_type>::arithmetic_type arithmetic_type;
		arithmetic_type sqDistance = segment_segment_distance_sqrd(seg, make_segment(obb[0], obb[1]), cmp);
		sqDistance = (std::min)(sqDistance, segment_segment_distance_sqrd(seg, make_segment(obb[1], obb[2]), cmp));
		sqDistance = (std::min)(sqDistance, segment_segment_distance_sqrd(seg, make_segment(obb[2], obb[3]), cmp));
		return (std::min)(sqDistance, segment_segment_distance_sqrd(seg, make_segment(obb[3], obb[1]), cmp));
	}

	template <typename Segment, typename PointOBB, typename VectorOBB, typename NumberComparisonPolicy>
	inline typename geometric_traits<typename geometric_traits<Segment>::point_type>::arithmetic_type segment_obb_distance(const Segment& seg, const oriented_bounding_box<PointOBB, VectorOBB>& b, const NumberComparisonPolicy& cmp)
	{
		return math::sqrt(segment_obb_distance_sqrd(seg, b, cmp));
	}

}//namespace geometrix;

#endif //GEOMETRIX_ALGORITHM_SEGMENT_OBB_DISTANCE_HPP
