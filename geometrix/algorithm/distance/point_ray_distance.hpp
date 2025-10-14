//
//! Copyright � 2008-2021
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
#include <geometrix/algorithm/distance/point_point_distance.hpp>
#include <geometrix/primitive/ray.hpp>
#include <geometrix/arithmetic/arithmetic_promotion_policy.hpp>
#include <geometrix/algebra/algebra.hpp>

namespace geometrix {
	
	template <typename Point, typename Ray>
	inline auto point_ray_distance(const Point& p, const Ray& l)
	{
		auto t = scalar_projection(p - l.get_reference_point(), l.get_normal_vector());
		if( t >= constants::zero<decltype(t)>() )
		{
			return t;
		}
		return point_point_distance(p, l.get_reference_point());
	}
	
    template <typename Point, typename Point1, typename Point2>
	inline auto point_ray_distance(const Point& p, const Point1& p1, const Point2& p2)
	{
        auto n = left_normal(normalize(p2-p1));
		auto t = scalar_projection(p - p1, n);
		if( t >= constants::zero<decltype(t)>() )
		{
			return t;
		}
		return point_point_distance(p, p1);
	}

}//namespace geometrix;

