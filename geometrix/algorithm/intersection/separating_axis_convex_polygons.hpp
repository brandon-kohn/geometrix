//
//! Copyright © 2008-2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_SEPARATING_AXIS_CONVEX_POLYGONS_HPP
#define GEOMETRIX_SEPARATING_AXIS_CONVEX_POLYGONS_HPP

#include <geometrix/algorithm/intersection/detail/separating_axis_common.hpp>

namespace geometrix {

	template <typename Polygon1, typename Polygon2, typename NumberComparisonPolicy>
	inline bool convex_polygons_intersection(const Polygon1& p1, const Polygon2& p2, const NumberComparisonPolicy& cmp)
	{
		using namespace detail_separating_axis;
		using access1 = point_sequence_traits<Polygon1>;
		using access2 = point_sequence_traits<Polygon2>;
		using point_type = typename access1::point_type;
        using length_t = typename arithmetic_type_of<point_type>::type;
		using vector_t = vector<length_t, dimension_of<point_type>::value>;

		for(std::size_t i0 = 0, i1 = access1::size(p1) - 1; i0 < access1::size(p1); i1 = i0, ++i0)
		{
			auto d = left_normal<vector_t>(access1::get_point(p1, i0) - access1::get_point(p1, i1));
			std::size_t min = get_extreme_index(p2, -d, cmp);
			vector_t diff = access2::get_point(p2, min) - access1::get_point(p1, i0);
			auto dd = dot_product(d, diff);
			if(cmp.greater_than(dd, constants::zero<decltype(dd)>()))
				return false;
		}

		for(std::size_t i0 = 0, i1 = access2::size(p2) - 1; i0 < access2::size(p2); i1 = i0, ++i0)
		{
			auto d = left_normal<vector_t>(access2::get_point(p2, i0) - access2::get_point(p2, i1));
			std::size_t min = get_extreme_index(p1, -d, cmp);
			vector_t diff = access1::get_point(p1, min) - access2::get_point(p2, i0);
			auto dd = dot_product(d, diff);
			if(cmp.greater_than(dd, constants::zero<decltype(dd)>()))
				return false;
		}

		return true;
	}

}//namespace geometrix;

#endif //GEOMETRIX_SEPARATING_AXIS_CONVEX_POLYGONS_HPP
