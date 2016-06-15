//
//! Copyright © 2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_ALGORITHM_POINT_SEQUENCE_FINDSUBSEQUENCE_HPP
#define GEOMETRIX_ALGORITHM_POINT_SEQUENCE_FINDSUBSEQUENCE_HPP
#pragma once

#include <geometrix/primitive/point_sequence_traits.hpp>
#include <geometrix/utility/utilities.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/primitive/polyline.hpp>
#include <geometrix/algorithm/euclidean_distance.hpp>

#include <boost/optional.hpp>
#include <algorithm>

namespace geometrix {

	struct point_not_on_polygon_exception : std::bad_exception {};

	//! Given a polygon P and a point p which is on P find the subsequence of P which contains all points in P whose linear distance along P is less than extent distance from p.
	template <typename Polygon, typename Point, typename NumberComparisonPolicy>
	inline boost::optional<std::size_t> find_containing_polygon_subsegment(const Polygon& P, const Point& p, const NumberComparisonPolicy& cmp)
	{
		using access = point_sequence_traits<Polygon>;
		auto size = access::size(P);
		for (std::size_t i = 0, j = 1; i < size; ++i, j = (j + 1) % size)
		{
			if (is_between(access::get_point(P, i), access::get_point(P, j), p, true, cmp))
				return i;
		}

		return boost::none;
	}

	namespace detail {
		template <polygon_winding Direction>
		struct polygon_incrementer;

		template <>
		struct polygon_incrementer<polygon_winding::clockwise>
		{
			polygon_incrementer(std::size_t size) : size(size){}
			std::size_t operator()(std::size_t i) const { return (i + size - 1) % size; }
			std::size_t size;
		};

		template <>
		struct polygon_incrementer<polygon_winding::counterclockwise>
		{
			polygon_incrementer(std::size_t size) : size(size){}
			std::size_t operator()(std::size_t i) const { return (i + 1) % size; }
			std::size_t size;
		};

		template <polygon_winding Direction, typename Polygon>
		inline polygon_incrementer<Direction> make_polygon_incrementer(const Polygon& P)		
		{
			using access = point_sequence_traits<Polygon>;
			return polygon_incrementer<Direction>(access::size(P));
		}
	
		template <polygon_winding Direction, typename Polygon, typename Point, typename Length, typename NumberComparisonPolicy>
		inline polyline<Point> polygon_subsequence_half(const Polygon& P, const Point& p, Length extent, std::size_t startIndex, const NumberComparisonPolicy& cmp)
		{
			using access = point_sequence_traits<Polygon>;
			Point currentP = construct<Point>(p);
		
			polyline<Point> result = { currentP };
			auto inc = detail::make_polygon_incrementer<Direction>(P);
			for (std::size_t i = startIndex, j = inc(i); extent > constants::zero<Length>(); i = j, j = inc(j))
			{
				auto pointj = access::get_point(P, j);
				auto length = point_point_distance(currentP, pointj);
				if (length <= extent)
				{
					currentP = pointj;
					extent -= length;
				}
				else
				{
					currentP = construct<Point>(currentP + extent * normalize(pointj - currentP));
					extent = constants::zero<Length>();
				}
			
				result.push_back(currentP);
			}

			return result;
		}
	}//! namespace detail;

	 //! Given a polygon P and a point p which is on P find the subsequence of P which contains all points in P whose linear distance along P is less than extent distance from p.
	template <typename Polygon, typename Point, typename Length, typename NumberComparisonPolicy>
	inline polyline<Point> polygon_subsequence(const Polygon& P, const Point& p, Length extent, const NumberComparisonPolicy& cmp)
	{
		using access = point_sequence_traits<Polygon>;
		using detail::polygon_subsequence_half;
		
		auto startIndex = find_containing_polygon_subsegment(P, p, cmp);
		if (startIndex)
		{
			polyline<Point> result1 = polygon_subsequence_half<polygon_winding::clockwise>(P, p, extent, *startIndex + 1, cmp);
			polyline<Point> result2 = polygon_subsequence_half<polygon_winding::counterclockwise>(P, p, extent, *startIndex, cmp);
			polyline<Point> result(result1.rbegin(), result1.rend()-1);
			result.insert(result.end(), result2.begin() + 1, result2.end());
			return result;
		}
		else
		{
			return polyline<Point>{};
		}
	}
}//! namespace geometrix;

#endif//! GEOMETRIX_ALGORITHM_POINT_SEQUENCE_FINDSUBSEQUENCE_HPP
