//
//! Copyright © 2021
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_RAY_POLYGON_INTERSECTION_HPP
#define GEOMETRIX_RAY_POLYGON_INTERSECTION_HPP
#pragma once

#include <geometrix/algorithm/intersection/ray_segment_intersection.hpp>

/////////////////////////////////////////////////////////////////////////////
//
// NAMESPACE
//
/////////////////////////////////////////////////////////////////////////////
namespace geometrix {

    template <typename PointA, typename UnitVector, typename Polygon, typename Visitor, typename NumberComparisonPolicy>
    inline bool ray_polygon_border_intersection(const PointA& o, const UnitVector& v, const Polygon& poly, Visitor&& visitor, const NumberComparisonPolicy& cmp)
    {
        BOOST_CONCEPT_ASSERT((Point2DConcept<PointA>));
        BOOST_CONCEPT_ASSERT((Vector2DConcept<UnitVector>));
        BOOST_CONCEPT_ASSERT((PointSequenceConcept<Polygon>));
        BOOST_CONCEPT_ASSERT((NumberComparisonPolicyConcept<NumberComparisonPolicy>));
        typedef point_sequence_traits<Polygon> access;
        typedef typename arithmetic_type_of<PointA>::type length_type;
        typedef point<length_type, 2> point_type;
        bool intersected = false;
        auto size = access::size(poly);
        for (std::size_t i = size - 1, j = 0; j < size; i = j++)
        {
            length_type l;
            point_type xPoints[2];
            auto iType = ray_segment_intersection(o, v, access::get_point(poly, i), access::get_point(poly, j), l, xPoints, cmp);
            if (iType != e_non_crossing)
            {
                bool stop = visitor(iType, i, j, l, xPoints[0], xPoints[1]);
                if (stop)
                    return true;
                intersected = true;
            }
        }

        return intersected;
    }

    template <typename PointA, typename UnitVector, typename Polyline, typename Visitor, typename NumberComparisonPolicy>
    inline bool ray_polyline_intersection(const PointA& o, const UnitVector& v, const Polyline& poly, Visitor&& visitor, const NumberComparisonPolicy& cmp)
    {
        typedef point_sequence_traits<Polyline> access;
        typedef typename arithmetic_type_of<PointA>::type length_type;
        typedef point<length_type, 2> point_type;
        bool intersected = false;
        auto size = access::size(poly);
        for (std::size_t i = 0, j = 1; j < size; i = j++)
        {
            length_type l;
            point_type xPoints[2];
            auto iType = ray_segment_intersection(o, v, access::get_point(poly, i), access::get_point(poly, j), l, xPoints, cmp);
            if (iType != e_non_crossing)
            {
                bool stop = visitor(iType, i, j, l, xPoints[0], xPoints[1]);
                if (stop)
                    return true;
                intersected = true;
            }
        }

        return intersected;
    }
}//namespace geometrix;

#endif //! GEOMETRIX_RAY_POLYGON_INTERSECTION_HPP
