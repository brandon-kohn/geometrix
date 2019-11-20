//
//! Copyright © 2008-2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_POLYGON_POLYLINE_CONTAINMENT_HPP
#define GEOMETRIX_POLYGON_POLYLINE_CONTAINMENT_HPP
#pragma once

#include <geometrix/primitive/point_sequence_traits.hpp>
#include <geometrix/primitive/segment.hpp>
#include <geometrix/algorithm/intersection/segment_segment_intersection.hpp>
#include <geometrix/algorithm/segment_mid_point.hpp>
#include <geometrix/algorithm/point_in_polygon.hpp>
#include <geometrix/algorithm/intersection/segment_polygon_intersection.hpp>
#include <geometrix/algorithm/intersection/segment_projects_outside_polygon.hpp>
#include <geometrix/utility/utilities.hpp>
#include <set>

namespace geometrix {

	//! Test if polyline is contained inside polygon. 
	template <typename Polyline, typename Polygon, typename NumberComparisonPolicy>
	inline bool polygon_polyline_containment( const Polyline& pline, const Polygon& pgon, const NumberComparisonPolicy& cmp )
	{
		typedef point_sequence_traits<Polyline> access1;
		typedef typename access1::point_type point_type;
		std::size_t size1 = access1::size( pline );
		
		typedef point_sequence_traits<Polygon> access2;
		std::size_t size2 = access2::size(pgon);
		GEOMETRIX_ASSERT(size2 > 2);

		//! First check if all points from 1 are contained in 2.
		bool isInside = false;
		for (std::size_t i = 0; i < size1; ++i)
		{
			if (point_polygon_containment_or_on_border(access1::get_point(pline, i), pgon, cmp) != polygon_containment::exterior)
			{
				isInside = true;
				break;
			}
		}

		if (!isInside)
			return false;

		//! All the points are inside. Check if there are any intersections between the edges.
		auto is_crossing_out = [&pline, &pgon, size2, &cmp]( std::size_t i, std::size_t j ) -> bool
		{
			std::size_t inext = (i + 1);
			std::size_t jnext = (j + 1) % size2;
			GEOMETRIX_ASSERT(inext < size1);
			point_type xpoint[2];
			auto itype = segment_segment_intersection( access1::get_point( pline, i ), access1::get_point( pline, inext ), access1::get_point( pgon, j ), access1::get_point( pgon, jnext ), xpoint, cmp );
			if (itype == e_non_crossing)
				return false;
			if (itype == e_crossing)
				return true;
			
			GEOMETRIX_ASSERT(itype == e_overlapping || itype == e_endpoint);

			//! Check if the segment exits polyline2 at any point.
			return segment_projects_outside_polygon(make_segment(access1::get_point(pline, i), access1::get_point(pline, inext)), pgon, cmp);
		};
		
		for (std::size_t i = 0; (i+1) < size1; ++i)
		{
			for (std::size_t j = 0; j < size2; ++j)
			{
				if (is_crossing_out(i, j))
					return false;
			}
		}
			
		return true;
	}

}//namespace geometrix;

#endif //! GEOMETRIX_POLYGON_POLYLINE_CONTAINMENT_HPP
