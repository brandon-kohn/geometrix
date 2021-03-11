//
//! Copyright © 2008-2021
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
		template <typename T1, typename T2, typename T3 = void, typename T4 = void>
        struct signed_point_line_distance;

		template <typename Point, typename Line>
		struct signed_point_line_distance<Point, Line>
		{
			using length_t = typename geometric_traits<Point>::arithmetic_type;
			using type = length_t;
		};

		template <typename Point, typename Point1, typename Point2>
		struct signed_point_line_distance<Point, Point1, Point2>
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
	
    template <typename Point, typename Point1, typename Point2>
	inline typename result_of::signed_point_line_distance<Point, Point1, Point2>::type signed_point_line_distance(const Point& p, const Point1& p1, const Point2& p2)
	{
        auto n = left_normal(normalize(p2-p1));
		return scalar_projection(p - p1, n);
	}

	namespace result_of {
		template <typename T1, typename T2, typename T3 = void, typename T4 = void>
        struct point_line_distance;

		template <typename Point, typename Line>
		struct point_line_distance<Point, Line>
		{
			using length_t = typename geometric_traits<Point>::arithmetic_type;
			using type = length_t;
		};
		
        template <typename Point, typename Point1, typename Point2>
		struct point_line_distance<Point, Point1, Point2>
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
	
    template <typename Point, typename Point1, typename Point2>
	inline typename result_of::point_line_distance<Point, Point1, Point2>::type point_line_distance(const Point& p, const Point1& p1, const Point2& p2)
	{
		using std::abs;
		return abs(signed_point_line_distance(p, p1, p2));
	}

}//namespace geometrix;

#endif //GEOMETRIX_POINT_LINE_DISTANCE_HPP
