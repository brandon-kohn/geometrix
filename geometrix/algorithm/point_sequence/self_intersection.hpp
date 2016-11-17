//
//! Copyright © 2008-2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_SELFINTERSECTION_HPP
#define GEOMETRIX_SELFINTERSECTION_HPP
#pragma once

#include <geometrix/primitive/point_sequence_traits.hpp>
#include <geometrix/algorithm/segment_intersection.hpp>

namespace geometrix {
	
	template <typename Polyline, typename Visitor, typename NumberComparisonPolicy>
	inline bool polyline_self_intersection( const Polyline& poly, Visitor&& visit, const NumberComparisonPolicy& cmp )
	{
		typedef point_sequence_traits<Polyline> access;
		typedef typename access::point_type point_type;
		std::size_t size = access::size( poly );

		if (size < 3)
			return false;

		auto next = []( std::size_t i ){ return i + 1; };
		auto adjacent = [&next](std::size_t i, std::size_t j) { return next(i) == j || next(j) == i; };
		point_type xPoints[2];
		intersection_type iType;
		auto is_intersecting = [&]( std::size_t i, std::size_t j ) -> bool
		{
			iType = segment_segment_intersection( access::get_point( poly, i ), access::get_point( poly, next(i) ), access::get_point( poly, j ), access::get_point( poly, next(j) ), xPoints, cmp );
			return iType != e_non_crossing;
		};

		bool isIntersecting = false;
		for (std::size_t i = 0; i < size - 2; ++i)
		{
			for (std::size_t j = i + 1; (j + 1) < size; ++j)
			{
				if (is_intersecting(i, j) && (!adjacent(i, j) || iType == e_overlapping))
				{
					isIntersecting = true;
					visit(i, j, iType, xPoints[0], xPoints[1]);
				}
			}
		}
		return isIntersecting;
	}

}//namespace geometrix;

#endif //! GEOMETRIX_SELFINTERSECTION_HPP
