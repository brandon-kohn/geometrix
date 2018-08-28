//
//! Copyright © 2018
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_OBB_OBB_INTERSECTION_HPP
#define GEOMETRIX_OBB_OBB_INTERSECTION_HPP

#include <geometrix/algorithm/intersection/detail/separating_axis_common.hpp>
#include <geometrix/primitive/oriented_bounding_box.hpp>
#include <geometrix/arithmetic/arithmetic_promotion_policy.hpp>

namespace geometrix {

	template <typename Point1, typename Vector1, typename Point2, typename Vector2, typename NumberComparisonPolicy>
	inline bool obb_obb_intersection(const oriented_bounding_box<Point1, Vector1>& o1, const oriented_bounding_box<Point2, Vector2>& o2, const NumberComparisonPolicy& cmp)
	{
        using std::abs;

        static_assert(dimension_of<Point1>::value == 2, "TODO: higher dimensionality intersection tests for OBB.");
        static_assert(dimension_of<Point2>::value == 2, "TODO: higher dimensionality intersection tests for OBB.");

		using namespace detail_separating_axis;
        using length_t = typename select_arithmetic_type_from_sequences<Point1, Point2>::type;
		using vector_t = vector<length_t, dimension_of<Point1>::value>;

        auto cdiff = vector_t{o2.get_center() - o1.get_center()};

        //! U-Axis from 1.
        //! u axis from 1 against u from 2.
        auto abscuu = abs(dot_product(o1.get_axis(0), o2.get_axis(0))); 

        //! u axis from 1 against v from 2.
        auto abscuv = abs(dot_product(o1.get_axis(0), o2.get_axis(1))); 

        auto r = abs(scalar_projection(cdiff, o1.get_axis(0)));
        auto r1 = o1.get_halfwidth(0);
        auto r2 = o2.get_halfwidth(0) * abscuu + o2.get_halfwidth(1) * abscuv;
        
        if(cmp.greater_than(r, r1 + r2))
            return false;

        //! V-Axis from 1.

        //! v axis from 1 against u from 2.
        auto abscvu = abs(dot_product(o1.get_axis(1), o2.get_axis(0))); 

        //! v axis from 1 against v from 2.
        auto abscvv = abs(dot_product(o1.get_axis(1), o2.get_axis(1))); 

        r = abs(scalar_projection(cdiff, o1.get_axis(1)));
        r1 = o1.get_halfwidth(1);
        r2 = o2.get_halfwidth(0) * abscvu + o2.get_halfwidth(1) * abscvv;
        
        if(cmp.greater_than(r, r1 + r2))
            return false;

        //! U-Axis from 2.
        //! u axis from 2 against u from 1.
        //! u axis from 1 against v from 2.
        r = abs(scalar_projection(cdiff, o2.get_axis(0)));
        r2 = o2.get_halfwidth(0);
        r1 = o1.get_halfwidth(0) * abscuu + o1.get_halfwidth(1) * abscuv;

        if(cmp.greater_than(r, r1 + r2))
            return false;

        //! V-Axis from 2.
        //! v axis from 2 against u from 1.
        //! v axis from 1 against v from 2.
        r = abs(scalar_projection(cdiff, o2.get_axis(1)));
        r2 = o2.get_halfwidth(1);
        r1 = o1.get_halfwidth(0) * abscvu + o1.get_halfwidth(1) * abscvv;
        
        if(cmp.greater_than(r, r1 + r2))
            return false;

		return true;
	}

}//namespace geometrix;

#endif //! GEOMETRIX_OBB_OBB_INTERSECTION_HPP
