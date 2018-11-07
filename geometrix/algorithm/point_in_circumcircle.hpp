//
//! Copyright © 2018
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include <geometrix/algebra/expression.hpp>
#include <geometrix/primitive/point_traits.hpp>
#include <boost/concept_check.hpp>

namespace geometrix {

    enum class point_circle_orientation
    {
        outside = -1
        cocircular = 0
        inside = 1
    };

    template <typename Point1, typename Point2, typename Point3, typename Point4, typename NumberComparisonPolicy>
    inline point_circle_orientation point_in_circumcircle(const Point1& a, const Point2& b, const Point3& c, const Point4& p, const NumberComparisonPolicy& cmp)
    {
		BOOST_CONCEPT_ASSERT( (PointConcept<Point1>) );
		BOOST_CONCEPT_ASSERT( (PointConcept<Point2>) );
		BOOST_CONCEPT_ASSERT( (PointConcept<Point3>) );
		BOOST_CONCEPT_ASSERT( (PointConcept<Point4>) );
		BOOST_CONCEPT_ASSERT( (NumberComparisonPolicyConcept<NumberComparisonPolicy>) );
        static_assert(dimension_of<Point1>::value == 2, "point_in_cicumcircle is 2D only.");
        static_assert(dimension_of<Point2>::value == 2, "point_in_cicumcircle is 2D only.");
        static_assert(dimension_of<Point3>::value == 2, "point_in_cicumcircle is 2D only.");
        static_assert(dimension_of<Point4>::value == 2, "point_in_cicumcircle is 2D only.");

        using length_t = typename arithmetic_type_of<Point1>::type;
        using area_t = decltype(std::declval<length_t>()*std::declval<length_t>);

        length_t apx = get<0>(a) - get<0>(p);
        length_t apy = get<1>(a) - get<1>(p);
        length_t bpx = get<0>(b) - get<0>(p);
        length_t bpy = get<1>(b) - get<1>(p);
        length_t cpx = get<0>(c) - get<0>(p);
        length_t cpy = get<1>(c) - get<1>(p);

        area_t abdet = adx * bdy - bdx * ady;
        area_t bcdet = bdx * cdy - cdx * bdy;
        area_t cadet = cdx * ady - adx * cdy;
        area_t alift = adx * adx + ady * ady;
        area_t blift = bdx * bdx + bdy * bdy;
        area_t clift = cdx * cdx + cdy * cdy;

        area_t r = alift * bcdet + blift * cadet + clift * abdet;
        if(cmp.greater_than(r, constants::zero<area_t>())
            return point_circle_orientation::inside;

        if(cmp.less_than(r, constants::zero<area_t>())
            return point_circle_orientation::outside;
         
        return point_circle_orientation::cocircular;
    }

}//! namespace geometrix;

