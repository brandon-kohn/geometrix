//
//! Copyright © 2008-2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_SEGMENT_PROJECTS_OUTSIDE_POLYGON_HPP
#define GEOMETRIX_SEGMENT_PROJECTS_OUTSIDE_POLYGON_HPP

#include <geometrix/algorithm/intersection/segment_segment_intersection.hpp>
#include <geometrix/primitive/point_sequence_traits.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/algorithm/point_in_polygon.hpp>
#include <geometrix/utility/utilities.hpp>

/////////////////////////////////////////////////////////////////////////////
//
// NAMESPACE
//
/////////////////////////////////////////////////////////////////////////////
namespace geometrix {

	template<typename Segment, typename Polygon, typename NumberComparisonPolicy>
	inline bool segment_projects_outside_polygon(const Segment& seg, const Polygon& poly, const NumberComparisonPolicy& cmp)
	{
		typedef typename geometric_traits<Segment>::point_type point_type;
		typedef point_sequence_traits<Polygon> access;

		bool startInside = (point_polygon_containment_or_on_border(get_start(seg), poly, cmp) != polygon_containment::exterior);
		bool endInside = (point_polygon_containment_or_on_border(get_end(seg), poly, cmp) != polygon_containment::exterior);

		std::set<point_type, lexicographical_comparer<NumberComparisonPolicy>> intersections(cmp);

		std::size_t size = access::size(poly);
		for (size_t i = 0; i < size; ++i)
		{
			std::size_t j = (i + 1) % size;
			point_type xpoints[2];
			auto itype = segment_segment_intersection(get_start(seg), get_end(seg), access::get_point(poly, i), access::get_point(poly, j), xpoints, cmp);
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
					if (point_polygon_containment_or_on_border(segment_mid_point(test), poly, cmp) == polygon_containment::exterior)
						return true;
				}
			}
		}

		return false;
	}

}//namespace geometrix;

#endif //GEOMETRIX_SEGMENT_PROJECTS_OUTSIDE_POLYGON_HPP
