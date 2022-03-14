//
//! Copyright © 2008-2021
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include <geometrix/tensor/vector_traits.hpp>
#include <geometrix/arithmetic/vector.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/primitive/plane.hpp>
#include <geometrix/arithmetic/arithmetic_promotion_policy.hpp>
#include <geometrix/algebra/algebra.hpp>
#include <geometrix/primitive/plane_traits.hpp>

namespace geometrix {
	namespace result_of {
		template <typename Point, typename Plane>
		struct signed_point_plane_distance
		{
			using length_t = typename geometric_traits<Point>::arithmetic_type;
			using type = length_t;
		};
	}
	
	template <typename Point, typename Plane>
	inline typename result_of::signed_point_plane_distance<Point, Plane>::type signed_point_plane_distance(const Point& p, const Plane& l)
	{
        using access = plane_access_traits<typename std::decay<Plane>::type>;
		return scalar_projection(p - access::get_reference_point(l), access::get_normal_vector(l));
	}

	namespace result_of {
		template <typename Point, typename Plane>
		struct point_plane_distance
		{
			using length_t = typename geometric_traits<Point>::arithmetic_type;
			using type = length_t;
		};
	}

	template <typename Point, typename Plane>
	inline typename result_of::point_plane_distance<Point, Plane>::type point_plane_distance(const Point& p, const Plane& l)
	{
		using std::abs;
		return abs(signed_point_plane_distance(p, l));
	}

}//namespace geometrix;

