//
//! Copyright © 2008-2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_POLYGON_AABB_INTERSECTION_HPP
#define GEOMETRIX_POLYGON_AABB_INTERSECTION_HPP

#include <geometrix/algorithm/intersection/segment_aabb_intersection.hpp>
#include <geometrix/primitive/polygon.hpp>
#include <geometrix/algorithm/point_in_polygon.hpp>

namespace geometrix {

	template <typename Polygon, typename AABB>
	inline bool polygon_aabb_intersection(const Polygon& poly, const AABB& b)
	{
		BOOST_CONCEPT_ASSERT((PointSequenceConcept<Polygon>));
		using access = point_sequence_traits<Polygon>;

		//! First check if any of the 4 corners of the aabb are inside poly.
		if( point_in_polygon(b[0], poly) ||
			point_in_polygon(b[1], poly) || 
			point_in_polygon(b[2], poly) || 
			point_in_polygon(b[3], poly) )
		{
			return true;
		}

		//! If none of the box corners are inside the polygon, there must be an border intersection 
		//! for it to intersect.
		std::size_t size = access::size(poly);
		for (std::size_t i = 0, j = 1; i < size; ++i, j = (j + 1) % size)
		{
			auto p1 = access::get_point(poly, i);
			auto p2 = access::get_point(poly, j);
			if (segment_aabb_intersection(p1, p2, b))
				return true;
		}

		return false;
	}
}//! namespace geometrix;

#endif//! GEOMETRIX_POLYGON_AABB_INTERSECTION_HPP
