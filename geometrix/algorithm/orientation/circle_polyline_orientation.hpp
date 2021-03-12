//
//! Copyright © 2021
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include <geometrix/algorithm/orientation_enum.hpp>
#include <geometrix/algorithm/orientation/circle_segment_orientation.hpp>
#include <geometrix/primitive/point_sequence_traits.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/arithmetic/arithmetic_promotion_policy.hpp>
#include <geometrix/algorithm/distance/point_segment_closest_point.hpp>
#include <geometrix/algorithm/distance/point_segment_distance.hpp>
#include <geometrix/algorithm/distance/point_point_distance.hpp>
#include <geometrix/utility/utilities.hpp>
#include <type_traits>

namespace geometrix {

    template <typename Point, typename Radius, typename Polyline, typename Length, typename NumberComparisonPolicy, typename std::enable_if<is_point<Point>::value, int>::type = 0>
    inline orientation_type circle_polyline_orientation(const Point& p, const Radius& r, const Polyline& pline, const Length& minDistance, std::size_t segIndex, const NumberComparisonPolicy& cmp)
    {
        BOOST_CONCEPT_ASSERT((PointConcept<Point>));
        BOOST_CONCEPT_ASSERT((PointSequenceConcept<Polyline>));
        BOOST_CONCEPT_ASSERT((NumberComparisonPolicyConcept<NumberComparisonPolicy>));

        if (cmp.greater_than(minDistance, r))
            return point_polyline_orientation(p, pline, minDistance, segIndex, cmp);

        return oriented_collinear;
    }

    template <typename Sphere, typename Polyline, typename Length, typename NumberComparisonPolicy, typename std::enable_if<is_sphere<Sphere>::value, int>::type = 0>
    inline orientation_type circle_polyline_orientation(const Sphere& s, const Polyline& pline, const Length& minDistance, std::size_t segIndex, const NumberComparisonPolicy& cmp)
    {
        BOOST_CONCEPT_ASSERT((PointSequenceConcept<Polyline>));
        BOOST_CONCEPT_ASSERT((NumberComparisonPolicyConcept<NumberComparisonPolicy>));

        if (cmp.greater_than(minDistance, get_radius(s)))
            return point_polyline_orientation(get_center(s), pline, minDistance, segIndex, cmp);

        return oriented_collinear;
    }

    template <typename Point, typename Radius, typename Polyline, typename NumberComparisonPolicy, typename std::enable_if<is_point<Point>::value, int>::type = 0>
    inline orientation_type circle_polyline_orientation(const Point& p, const Radius& r, const Polyline& pline, const NumberComparisonPolicy& cmp)
    {
        BOOST_CONCEPT_ASSERT((PointSequenceConcept<Polyline>));
        BOOST_CONCEPT_ASSERT((NumberComparisonPolicyConcept<NumberComparisonPolicy>));

        using access = point_sequence_traits<Polyline>;
        GEOMETRIX_ASSERT(access::size(pline) > 1);

        using length_t = typename select_arithmetic_type_from_sequences<Point, typename access::point_type>::type;
        using area_t = decltype(std::declval<length_t>() * std::declval<length_t>());

        auto minDistance = (std::numeric_limits<area_t>::max)();
        auto segIndex = std::size_t{};
        for (std::size_t i = 0, j = 1; j < access::size(pline); i = j++)
        {
            auto d2 = point_segment_distance_sqrd(p, access::get_point(pline, i), access::get_point(pline, j));
            if (d2 < minDistance)
            {
                minDistance = d2;
                segIndex = i;
            }
        }

        return circle_polyline_orientation(p, r, pline, minDistance, segIndex, cmp);
    }

    template <typename Sphere, typename Polyline, typename NumberComparisonPolicy, typename std::enable_if<is_sphere<Sphere>::value, int>::type = 0>
    inline orientation_type circle_polyline_orientation(const Sphere& s, const Polyline& pline, const NumberComparisonPolicy& cmp)
    {
        BOOST_CONCEPT_ASSERT((PointSequenceConcept<Polyline>));
        BOOST_CONCEPT_ASSERT((NumberComparisonPolicyConcept<NumberComparisonPolicy>));

        using access = point_sequence_traits<Polyline>;
        GEOMETRIX_ASSERT(access::size(pline) > 1);

        using length_t = typename select_arithmetic_type_from_sequences<typename geometric_traits<Sphere>::point_type, typename access::point_type>::type;
        using area_t = decltype(std::declval<length_t>() * std::declval<length_t>());

        auto minDistance = (std::numeric_limits<area_t>::max)();
        auto segIndex = std::size_t{};
        for (std::size_t i = 0, j = 1; j < access::size(pline); i = j++)
        {
            auto d2 = point_segment_distance_sqrd(get_center(s), access::get_point(pline, i), access::get_point(pline, j));
            if (d2 < minDistance)
            {
                minDistance = d2;
                segIndex = i;
            }
        }

        return circle_polyline_orientation(s, pline, minDistance, segIndex, cmp);
    }
}//! namespace geometrix;

