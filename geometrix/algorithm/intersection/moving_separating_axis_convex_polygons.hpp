//
//! Copyright © 2018
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_MOVING_SEPARATING_AXIS_CONVEX_POLYGONS_HPP
#define GEOMETRIX_MOVING_SEPARATING_AXIS_CONVEX_POLYGONS_HPP

#include <geometrix/algorithm/intersection/detail/separating_axis_common.hpp>

namespace geometrix {

	template <typename Polygon1, typename Velocity1, typename Polygon2, typename Velocity2, typename Time, typename NumberComparisonPolicy>
	inline bool moving_convex_polygons_intersection(const Polygon1& p1, const Velocity1& v1, const Polygon2& p2, const Velocity2& v2, const Time& tmax, Time& tfirst, Time& tlast, typename NumberComparisonPolicy& cmp)
	{
		using namespace detail_separating_axis;
		using access1 = point_sequence_traits<Polygon1>;
		using access2 = point_sequence_traits<Polygon2>;
		using point_type = typename access1::point_type;
        using length_t = typename arithmetic_type_of<point_type>::type;
		using vector_t = vector<length_t, dimension_of<point_type>::value>;
        auto v = vector_t{v2 - v1};
        auto side = int{};
        tfirst = constants::zero<Time>();
        tlast = constants::infinity<Time>();

        auto min1 = length_t{};
        auto max1 = length_t{};
        auto min2 = length_t{};
        auto max2 = length_t{};

		for(std::size_t i0 = 0, i1 = access1::size(p1) - 1; i0 < access1::size(p1); i1 = i0, ++i0)
		{
			auto d = left_normal<vector_t>(access1::get_point(p1, i0) - access1::get_point(p1, i1));
            auto speed = dot_product(d, v);
            compute_interval(p1, d, min1, max1, cmp);
            compute_interval(p2, d, min2, max2, cmp);
            if(no_intersect(tmax, speed, min1, max1, min2, max2, side, tfirst, tlast))
                return false;
		}

		for(std::size_t i0 = 0, i1 = access2::size(p2) - 1; i0 < access2::size(p2); i1 = i0, ++i0)
		{
			auto d = left_normal<vector_t>(access2::get_point(p2, i0) - access2::get_point(p2, i1));
            auto speed = dot_product(d, v);
            compute_interval(p1, d, min1, max1, cmp);
            compute_interval(p2, d, min2, max2, cmp);
            if(no_intersect(tmax, speed, min1, max1, min2, max2, side, tfirst, tlast))
                return false;
		}

		//! if side == 0, the polygons were already intersecting before moving.

		return true;
	}

}//namespace geometrix;

#endif //! GEOMETRIX_MOVING_SEPARATING_AXIS_CONVEX_POLYGONS_HPP
