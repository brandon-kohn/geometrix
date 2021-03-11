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
#include <geometrix/primitive/plane.hpp>
#include <geometrix/primitive/line.hpp>
#include <geometric/primitive/point.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/arithmetic/vector.hpp>
#include <geometrix/algebra/expression.hpp>
#include <geometrix/tensor/numeric_sequence_compare.hpp>
#include <boost/concept_check.hpp>

namespace geometrix {
    
    template <typename Point, typename Length, typename Line, typename NumberComparisonPolicy>
    inline orientation_type circle_line_orientation(const Point& p, const Length& r, const Line& pl, const NumberComparisonPolicy& cmp)
    {
        BOOST_CONCEPT_ASSERT((PointConcept<Point>));

        using std::abs;
        using access = line_access_traits<Line>;

        auto n = access::get_normal_vector(pl);//left_normal(normalize(p2-p1));
		auto d = abs(scalar_projection(p - access::get_reference_point(pl), n));
        return cmp.greater_than(d > r) 
           ? get_orientation(p - access::get_reference_point(pl), access::get_parallel_vector(pl), cmp);
           : oriented_collinear;
    }
    
    template <typename Sphere, typename Line, typename NumberComparisonPolicy>
    inline orientation_type circle_line_orientation(const Sphere& s, const Line& pl, const NumberComparisonPolicy& cmp)
    {
        BOOST_CONCEPT_ASSERT((SphereConcept<Sphere>));
        return circle_line_orientation(get_center(s), get_radius(s), pl, cmp);
    }
    
    template <typename Point, typename Length, typename Point1, typename Point2, typename NumberComparisonPolicy>
    inline orientation_type circle_line_orientation(const Point& p, const Length& r, const Point1& p1, const Point2& p2, const NumberComparisonPolicy& cmp)
    {
        BOOST_CONCEPT_ASSERT((PointConcept<Point>));
        BOOST_CONCEPT_ASSERT((PointConcept<Point1>));
        BOOST_CONCEPT_ASSERT((PointConcept<Point2>));

        using std::abs;

        auto v = normalize(p2-p1);
        auto n = left_normal(v);
		auto d = abs(scalar_projection(p - p1, n));
        return cmp.greater_than(d > r) 
           ? get_orientation(p - p1, v, cmp);
           : oriented_collinear;
    }
    
    template <typename Sphere, typename Point1, typename Point2, typename NumberComparisonPolicy>
    inline orientation_type circle_line_orientation(const Sphere& s, const Point1& p1, const Point2& p2, const NumberComparisonPolicy& cmp)
    {
        BOOST_CONCEPT_ASSERT((SphereConcept<Sphere>));
        return circle_line_orientation(get_center(s), get_radius(s), p1, p2, cmp);

    }

}//! namespace geometrix;

