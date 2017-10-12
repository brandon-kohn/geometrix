//
//! Copyright © 2008-2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_LINE_AABB_INTERSECTION_HPP
#define GEOMETRIX_LINE_AABB_INTERSECTION_HPP

#include <geometrix/primitive/sphere_traits.hpp>
#include <geometrix/primitive/segment_traits.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/primitive/line_traits.hpp>
#include <geometrix/algebra/expression.hpp>
#include <geometrix/algebra/dot_product.hpp>
#include <cstdint>

namespace geometrix {
	
	struct line_aabb_intersection_result
	{
		typedef void (line_aabb_intersection_result::*bool_type)() const;

		line_aabb_intersection_result() = default;

		line_aabb_intersection_result(bool isIntersecting, bool isOriginInside = false)
			: result((isIntersecting ? e_is_intersecting : 0) | (isOriginInside ? e_origin_inside : 0))
		{}

		bool is_intersecting() const { return (result & e_is_intersecting) != 0; }

		operator bool_type() const
		{
			return is_intersecting() ? &line_aabb_intersection_result::bool_type_mfn : 0;
		}

		bool operator == (bool b) const { return b == is_intersecting(); }

	private:

		void bool_type_mfn() const {}

		enum Flags
		{
			e_is_intersecting = 1
		};

		std::uint32_t result{ 0 };

	};
	
	//! From Real Time Collision Detection
	// Intersects line l = p + td, |d| = 1,with AABB a and, if intersecting, 
	// returns t0 and t1 values of both intersection distances and intersection points q0, q1. If intersecting at a corner both will be the same value.
	template <typename Line, typename AABB, typename Scalar, typename Point, typename NumberComparisonPolicy>
	inline line_aabb_intersection_result line_aabb_intersection(const Line& l, const AABB& a, Scalar& tentrance, Point& q0, Scalar& texit, Point& q1, const NumberComparisonPolicy& cmp)
	{
		// Intersect line R(t) = p + t*d against AABB a.
		using line_t = typename std::decay<Line>::type;
		using line_access = line_access_traits<line_t>;
		auto p = line_access::get_reference_point(l);
		auto d = line_access::get_parallel_vector(l);

		using point_t = typename std::decay<decltype(p)>::type;
		using vector_t = typename std::decay<decltype(v)>::type;

#if BOOST_VERSION >= 105700
		auto tmin = std::numeric_limits<Scalar>::lowest(); // set to -FLT_MAX to get first hit on lineb
#else
		auto tmin = construct<Scalar>(-(std::numeric_limits<double>::max)());
#endif
		auto tmax = (std::numeric_limits<Scalar>::max)(); // set to max distance line can travel (for segment)
													 // For all sides.

		{
			if (!cmp.equals(get<0>(d), constants::zero<typename type_at<vector_t, 0>::type>()))
			{
				// Compute intersection t value of line with near and far plane of slab 
				using factor_t = decltype((Scalar() * typename type_at<vector_t, 0>::type()) / (typename type_at<point_t, 0>::type()));
				auto ood = constants::one<factor_t>() / get<0>(d);
				Scalar t1 = (get<0>(a.get_lower_bound()) - get<0>(p)) * ood;
				Scalar t2 = (get<0>(a.get_upper_bound()) - get<0>(p)) * ood;

				// Make t1 be intersection with near plane, t2 with far plane 
				if (t1 > t2)
					std::swap(t1, t2);

				// Compute the intersection of slab intersection intervals 
				if (t1 > tmin)
					tmin = t1;
				if (t2 < tmax)
					tmax = t2;
				
				// Exit with no collision as soon as slab intersection becomes empty 
				if (tmin > tmax)
					return false;
			}
		}

		texit = tmax;
		{
			if (!cmp.equals(get<1>(d), constants::zero<typename type_at<vector_t, 1>::type>()))
			{
				// Compute intersection t value of line with near and far plane of slab 
				using factor_t = decltype((Scalar() * typename type_at<vector_t, 1>::type()) / (typename type_at<point_t, 1>::type()));
				auto ood = constants::one<factor_t>() / get<1>(d);
				Scalar t1 = (get<1>(a.get_lower_bound()) - get<1>(p)) * ood;
				Scalar t2 = (get<1>(a.get_upper_bound()) - get<1>(p)) * ood;

				// Make t1 be intersection with near plane, t2 with far plane 
				if (t1 > t2)
					std::swap(t1, t2);

				// Compute the intersection of slab intersection intervals 
				if (t1 > tmin)
					tmin = t1;
				if (t2 > tmax)
					tmax = t2;
				
				// Exit with no collision as soon as slab intersection becomes empty 
				if (tmin > tmax)
					return false;

				texit = (std::min)(t2, texit);
			}
		}

		// line intersects all 2 slabs. Return point (q) and intersection t value (tmin) 
		tentrance = tmin;
		assign(q0, p + d*tentrance);
		assign(q1, p + d*texit);

		return true;
	}
}//! namespace geometrix;

#endif//! GEOMETRIX_RAY_SPHERE_INTERSECTION_HPP