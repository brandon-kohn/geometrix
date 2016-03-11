//
//! Copyright © 2008-2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_SEGMENT_POLYGON_INTERSECTION_HPP
#define GEOMETRIX_SEGMENT_POLYGON_INTERSECTION_HPP

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

	template<typename Segment, typename Polygon, typename Visitor, typename NumberComparisonPolicy>
	inline bool segment_polygon_intersection(const Segment& seg, const Polygon& poly, Visitor&& visitor, const NumberComparisonPolicy& cmp)
	{
		typedef geometric_traits<Segment>::point_type point_type;
		typedef point_sequence_traits<Polygon> access;

		bool startInside = (point_polygon_containment_or_on_border(get_start(seg), poly, cmp) != polygon_containment::exterior);
		bool endInside = (point_polygon_containment_or_on_border(get_end(seg), poly, cmp) != polygon_containment::exterior);

		std::set<point_type, lexicographical_comparer<NumberComparisonPolicy>> intersections(cmp);

		std::size_t size = access::size(poly);
		for (size_t i = 0; i < size; ++i)
		{
			std::size_t j = (i + 1) % size;
			point_type xpoints[2];
			auto itype = segment_segment_intersection(get_start(seg), get_end(seg), access::get_point(poly, i), access::get_point(poly, j), xpoints);
			if (itype == e_crossing || itype == e_endpoint)
				intersections.insert(xpoints[0]);
			else if (itype == e_overlapping)
				intersections.insert(xpoints, xpoints + 1);
		}

		if (!intersections.empty())
		{
			intersections.insert(get_start(seg));
			intersections.insert(get_end(seg));

			for (auto it = intersections.begin(); it != intersections.end(); ++it)
			{
				auto nextIT = it;
				++nextIT;
				if (nextIT != intersections.end())
				{
					auto test = construct<Segment>(*it, *nextIT);
					if (point_polygon_containment_or_on_border(segment_mid_point(test), poly, cmp))
						visitor(test);
				}
			}

			return true;
		}
		else if (startInside && endInside && point_polygon_containment_or_on_border(segment_mid_point(seg), poly, cmp))
		{
			visitor(seg);
			return true;
		}

		return false;
	}
	
}//namespace geometrix;

#endif //GEOMETRIX_SEGMENT_POLYLINE_INTERSECTION_HPP
