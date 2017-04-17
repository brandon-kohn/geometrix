//
//! Copyright © 2008-2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_RAY_AABB_INTERSECTION_HPP
#define GEOMETRIX_RAY_AABB_INTERSECTION_HPP

#include <geometrix/primitive/sphere_traits.hpp>
#include <geometrix/primitive/segment_traits.hpp>
#include <geometrix/primitive/axis_aligned_bounding_box.hpp>
#include <geometrix/algebra/expression.hpp>
#include <geometrix/algebra/dot_product.hpp>

namespace geometrix {

	//! From Real Time Collision Detection
	//! Intersects segment <p0, p1> with AABB b and if intersecting returns true.

	namespace detail {
		template <typename Point1, typename Point2, typename AABB>
		inline bool segment_aabb_intersection(const Point1& p0, const Point2& p1, const AABB& b, dimension<3>)
		{
			using std::abs;

			using length_t = typename select_arithmetic_type_from_sequences<Point1, Point2>::type;
			using vector_t = vector<length_t, 3>;
			using point_t = point<length_t, 3>;

			//! Intersect segment R(t) = <p1, p2> against AABB a. When intersecting, 
			//! return intersection distance tmin and point q of intersection 
			vector_t e = b.get_upper_bound() - b.get_lower_bound(); // Box halflength extents
			vector_t d = p1 - p0; // Segment halflength vector
			point_t m = (p0 - b.get_lower_bound()) + (p1 - b.get_upper_bound());

			//! Try world coordinate axes as separating axes
			auto adx = abs(get<0>(d));
			if (abs(get<0>(m)) > get<0>(e) + adx)
				return false;
			auto ady = abs(get<1>(d));
			if (abs(get<1>(m)) > get<1>(e) + ady)
				return false;
			auto adz = abs(get<2>(d));
			if (abs(get<2>(m)) > get<2>(e) + adz)
				return false;

			// Add in an epsilon term to counteract arithmetic errors when segment is
			// (near) parallel to a coordinate axis (see text for detail)
			adx += std::numeric_limits<length_t>::epsilon(); ady += std::numeric_limits<length_t>::epsilon(); adz += std::numeric_limits<length_t>::epsilon();

			// Try cross products of segment direction vector with coordinate axes
			if (abs(get<1>(m) * get<2>(d) - get<2>(m) * get<1>(d)) > get<1>(e) * adz + get<2>(e) * ady)
				return false;
			if (abs(get<2>(m) * get<0>(d) - get<0>(m) * get<2>(d)) > get<0>(e) * adz + get<2>(e) * adx)
				return false;
			if (abs(get<0>(m) * get<1>(d) - get<1>(m) * get<0>(d)) > get<0>(e) * ady + get<1>(e) * adx)
				return false;

			// No separating axis found; segment must be overlapping AABB
			return true;
		}

		template <typename Point1, typename Point2, typename AABB>
		inline bool segment_aabb_intersection(const Point1& p0, const Point2& p1, const AABB& b, dimension<2>)
		{
			using std::abs;

			using length_t = typename select_arithmetic_type_from_sequences<Point1, Point2>::type;
			using vector_t = vector<length_t, 2>;
			using point_t = point<length_t, 2>;

			//! Intersect segment R(t) = <p1, p2> against AABB a. When intersecting, 
			//! return intersection distance tmin and point q of intersection 
			vector_t e = b.get_upper_bound() - b.get_lower_bound(); // Box halflength extents
			vector_t d = p1 - p0; // Segment halflength vector
			point_t m = (p0 - b.get_lower_bound()) + (p1 - b.get_upper_bound());

			//! Try world coordinate axes as separating axes
			auto adx = abs(get<0>(d));
			if (abs(get<0>(m)) > get<0>(e) + adx)
				return false;
			auto ady = abs(get<1>(d));
			if (abs(get<1>(m)) > get<1>(e) + ady)
				return false;

			// Add in an epsilon term to counteract arithmetic errors when segment is
			// (near) parallel to a coordinate axis (see text for detail)
			adx += std::numeric_limits<length_t>::epsilon(); ady += std::numeric_limits<length_t>::epsilon();

			// Try cross products of segment direction vector with coordinate axes

			// Try cross products of segment direction vector with coordinate axes
			if (abs(get<0>(m) * get<1>(d) - get<1>(m) * get<0>(d)) > get<0>(e) * ady + get<1>(e) * adx)
				return false;

			// No separating axis found; segment must be overlapping AABB
			return true;
		}
	}//! namespace detail;
	

	template <typename Point1, typename Point2, typename AABB>
	inline bool segment_aabb_intersection(const Point1& p0, const Point2& p1, const AABB& b)
	{
		return ::geometrix::detail::segment_aabb_intersection(p0, p1, b, dimension_of<Point1>::type());
	}
}//! namespace geometrix;

#endif//! GEOMETRIX_RAY_SPHERE_INTERSECTION_HPP