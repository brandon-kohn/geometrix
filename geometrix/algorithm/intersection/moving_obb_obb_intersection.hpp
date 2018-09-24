//
//! Copyright © 2018
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_MOVING_OBB_OBB_INTERSECTION_HPP
#define GEOMETRIX_MOVING_OBB_OBB_INTERSECTION_HPP

#include <geometrix/algorithm/intersection/detail/separating_axis_common.hpp>
#include <geometrix/primitive/oriented_bounding_box.hpp>
#include <geometrix/arithmetic/arithmetic_promotion_policy.hpp>

namespace geometrix {

	template <typename Point1, typename Vector1, typename Velocity1, typename Point2, typename Vector2, typename Velocity2, typename Time, typename NumberComparisonPolicy>
	inline bool moving_obb_obb_intersection(const oriented_bounding_box<Point1, Vector1>& o1, const Velocity1& v1, const oriented_bounding_box<Point2, Vector2>& o2, const Velocity2& v2, const Time& tmax, Time& tfirst, Time& tlast, const NumberComparisonPolicy& cmp)
	{
        static_assert(dimension_of<Point1>::value == 2, "TODO: higher dimensionality intersection tests for OBB.");
        static_assert(dimension_of<Point2>::value == 2, "TODO: higher dimensionality intersection tests for OBB.");

		using namespace detail_separating_axis;
        using length_t = typename select_arithmetic_type_from_sequences<Point1, Point2>::type;
        using speed_t = typename select_arithmetic_type_from_sequences<Velocity1, Velocity2>::type;
		using vector_t = vector<length_t, dimension_of<Point1>::value>;
		using velocity_t = vector<speed_t, dimension_of<Velocity1>::value>;
        auto side = int{};
        tfirst = constants::zero<Time>();
        tlast = constants::infinity<Time>();

        bool existsParallelPair = false;
        auto cdiff = vector_t{o2.get_center() - o1.get_center()};
        auto v = velocity_t{v2 - v1};

        //! U-Axis from 1.
        //! u axis from 1 against u from 2.
        auto abscuu = abs(dot_product(o1.get_axis(0), o2.get_axis(0))); 
        //if(cmp.greater_than_equals(abscuu, constants::one<decltype(abscuu)>()))
        //   existsParallelPair = true;

        //! u axis from 1 against v from 2.
        auto abscuv = abs(dot_product(o1.get_axis(0), o2.get_axis(1))); 
        //if(cmp.greater_than_equals(abscuv, constants::one<decltype(abscuv)>()))
        //    existsParallelPair = true;

        auto center = scalar_projection(cdiff, o1.get_axis(0));
        auto speed = scalar_projection(v, o1.get_axis(0));
        auto r1 = o1.get_halfwidth(0);
        auto r2 = o2.get_halfwidth(0) * abscuu + o2.get_halfwidth(1) * abscuv;

        auto min1 = -r1;
        auto max1 = r1;
        auto min2 = center - r2;
        auto max2 = center + r2;

        if(no_intersect(tmax, speed, min1, max1, min2, max2, side, tfirst, tlast))
            return false;

        //! V-Axis from 1.

        //! v axis from 1 against u from 2.
        auto abscvu = abs(dot_product(o1.get_axis(1), o2.get_axis(0))); 
        //if(cmp.greater_than_equals(abscvu, constants::one<decltype(abscvu)>()))
        //    existsParallelPair = true;

        //! v axis from 1 against v from 2.
        auto abscvv = abs(dot_product(o1.get_axis(1), o2.get_axis(1))); 
        //if(cmp.greater_than_equals(abscvv, constants::one<decltype(abscvv)>()))
        //    existsParallelPair = true;

        center = scalar_projection(cdiff, o1.get_axis(1));
        speed = scalar_projection(v, o1.get_axis(1));
        r1 = o1.get_halfwidth(1);
        r2 = o2.get_halfwidth(0) * abscvu + o2.get_halfwidth(1) * abscvv;

        min1 = -r1;
        max1 = r1;
        min2 = center - r2;
        max2 = center + r2;

        if(no_intersect(tmax, speed, min1, max1, min2, max2, side, tfirst, tlast))
            return false;

        //! U-Axis from 2.
        //! u axis from 2 against u from 1.
        //! u axis from 1 against v from 2.
        center = scalar_projection(cdiff, o2.get_axis(0));
        speed = scalar_projection(v, o2.get_axis(0));
        r2 = o2.get_halfwidth(0);
        r1 = o1.get_halfwidth(0) * abscuu + o1.get_halfwidth(1) * abscuv;

        min1 = -r1;
        max1 = r1;
        min2 = center - r2;
        max2 = center + r2;

        if(no_intersect(tmax, speed, min1, max1, min2, max2, side, tfirst, tlast))
            return false;

        //! V-Axis from 2.
        //! v axis from 2 against u from 1.
        //! v axis from 1 against v from 2.
        center = scalar_projection(cdiff, o2.get_axis(1));
        speed = scalar_projection(v, o2.get_axis(1));
        r2 = o2.get_halfwidth(1);
        r1 = o1.get_halfwidth(0) * abscvu + o1.get_halfwidth(1) * abscvv;

        min1 = -r1;
        max1 = r1;
        min2 = center - r2;
        max2 = center + r2;

        if(no_intersect(tmax, speed, min1, max1, min2, max2, side, tfirst, tlast))
            return false;

        //! if side == 0, the polygons were already intersecting before moving.

		return true;
	}

}//namespace geometrix;

#endif //! GEOMETRIX_MOVING_OBB_OBB_INTERSECTION_HPP
