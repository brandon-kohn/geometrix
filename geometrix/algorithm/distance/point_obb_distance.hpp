//
//! Copyright © 2008-2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_ALGORITHM_POINT_OBB_DISTANCE_HPP
#define GEOMETRIX_ALGORITHM_POINT_OBB_DISTANCE_HPP
#pragma once

#include <geometrix/primitive/oriented_bounding_box.hpp>
#include <geometrix/algorithm/euclidean_distance.hpp>

namespace geometrix {

	template <typename Point, typename PointOBB, typename VectorOBB>
	inline typename geometric_traits<Point>::arithmetic_type point_obb_distance_sqrd(const Point& p, const oriented_bounding_box<PointOBB, VectorOBB>& b)
	{
		typedef typename geometric_traits<Point>::arithmetic_type arithmetic_type;
		typedef vector<arithmetic_type, 2> vector_type;
		vector_type v = p - b.get_center();
		arithmetic_type sqDist = 0;
		{
			// Project vector from box center to p on each axis, getting the distance
			// of p along that axis, and count any excess distance outside box extents
			arithmetic_type d = dot_product(v, b.get_v()), excess = 0;
			if (d < -b.get_v_half_width())
				excess=d+b.get_v_half_width();
			else if (d > b.get_v_half_width()) 
				excess=d-b.get_v_half_width();
			sqDist += excess * excess; 
		}
		{
			// Project vector from box center to p on each axis, getting the distance
			// of p along that axis, and count any excess distance outside box extents
			arithmetic_type d = dot_product(v, b.get_u()), excess = 0;
			if (d < -b.get_u_half_width())
				excess = d + b.get_u_half_width();
			else if (d > b.get_u_half_width())
				excess = d - b.get_u_half_width();
			sqDist += excess * excess;
		}

		return sqDist;
	}

	template <typename Point, typename PointOBB, typename VectorOBB>
	inline typename geometric_traits<Point>::arithmetic_type point_obb_distance(const Point& p, const oriented_bounding_box<PointOBB, VectorOBB>& b)
	{
		return math::sqrt(point_obb_distance_sqrd(p, b));
	}

	// Given point p, return point q on (or in) OBB b, closest to p 
	template <typename Point, typename PointOBB, typename VectorOBB>
	inline Point closest_point_on_obb(const Point& p, const oriented_bounding_box<PointOBB, VectorOBB>& b)
	{ 
		typedef typename geometric_traits<Point>::arithmetic_type arithmetic_type;
		typedef vector<arithmetic_type, 2> vector_type;
		
		// Start result at center of box; make steps from there
		Point q = construct<Point>(b.get_center());
		vector_type d = p - b.get_center();
		
		// For each OBB axis...
		{
			// ...project d onto that axis to get the distance
			// along the axis of d from the box center
			arithmetic_type dist = dot_product(d, b.get_u());
			// If distance farther than the box extents, clamp to the box
			if (dist > b.get_u_half_width())
				dist = b.get_u_half_width();
			if (dist < -b.get_u_half_width())
				dist = -b.get_u_half_width();
			// Step that distance along the axis to get world coordinate
			assign(q, q + dist * b.get_u());
		}
		{
			// ...project d onto that axis to get the distance
			// along the axis of d from the box center
			arithmetic_type dist = dot_product(d, b.get_v());
			// If distance farther than the box extents, clamp to the box
			if (dist > b.get_v_half_width())
				dist = b.get_v_half_width();
			if (dist < -b.get_v_half_width())
				dist = -b.get_v_half_width();
			// Step that distance along the axis to get world coordinate
			assign(q, q + dist * b.get_v());
		}

		return q;
	}
}//namespace geometrix;

#endif //GEOMETRIX_ALGORITHM_POINT_OBB_DISTANCE_HPP
