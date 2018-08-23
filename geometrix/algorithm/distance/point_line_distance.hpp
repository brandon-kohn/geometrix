//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_POINT_LINE_DISTANCE_HPP
#define GEOMETRIX_POINT_LINE_DISTANCE_HPP

#include <geometrix/tensor/vector_traits.hpp>
#include <geometrix/arithmetic/vector.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/primitive/line.hpp>
#include <geometrix/arithmetic/arithmetic_promotion_policy.hpp>
#include <geometrix/algebra/algebra.hpp>

namespace geometrix {
	namespace result_of {
		template <typename Point, typename Line>
		struct signed_point_line_distance
		{
			using length_t = typename geometric_traits<Point>::arithmetic_type;
			using type = length_t;
		};
	}
	
	template <typename Point, typename Line>
	inline typename result_of::signed_point_line_distance<Point, Line>::type signed_point_line_distance(const Point& p, const Line& l)
	{
		return scalar_projection(p - l.get_reference_point(), l.get_normal_vector());
	}

	namespace result_of {
		template <typename Point, typename Line>
		struct point_line_distance
		{
			using length_t = typename geometric_traits<Point>::arithmetic_type;
			using type = length_t;
		};
	}

	template <typename Point, typename Line>
	inline typename result_of::point_line_distance<Point, Line>::type point_line_distance(const Point& p, const Line& l)
	{
		using std::abs;
		return abs(signed_point_line_distance(p, l));
	}

}//namespace geometrix;

#endif //GEOMETRIX_POINT_LINE_DISTANCE_HPP
