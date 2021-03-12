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
#include <geometrix/primitive/point.hpp>
#include <geometrix/primitive/sphere.hpp>
#include <geometrix/arithmetic/vector.hpp>
#include <geometrix/algebra/expression.hpp>
#include <geometrix/tensor/numeric_sequence_compare.hpp>
#include <boost/concept_check.hpp>
#include <type_traits>

namespace geometrix {

    template <typename Point, typename Length, typename Plane, typename NumberComparisonPolicy, typename std::enable_if<is_point<Point>::value, int>::type = 0>
    inline orientation_type circle_plane_orientation(const Point& p, const Length& r, const Plane& pl, const NumberComparisonPolicy& cmp)
    {
        BOOST_CONCEPT_ASSERT((PointConcept<Point>));

        using std::abs;
        using access = plane_access_traits<Plane>;

        auto n = access::get_normal_vector(pl);
		auto d = abs(scalar_projection(p - access::get_reference_point(pl), n));
        return cmp.greater_than(d, r) 
           ? vector_vector_orientation(p - access::get_reference_point(pl), access::get_parallel_vector(pl), cmp)
           : oriented_collinear;
    }
    
    template <typename Sphere, typename Plane, typename NumberComparisonPolicy, typename std::enable_if<is_sphere<Sphere>::value, int>::type = 0>
    inline orientation_type circle_plane_orientation(const Sphere& s, const Plane& pl, const NumberComparisonPolicy& cmp)
    {
        return circle_plane_orientation(get_center(s), get_radius(s), pl, cmp);
    }
    
    template <typename Point, typename Length, typename Point1, typename Point2, typename NumberComparisonPolicy, typename std::enable_if<is_point<Point>::value, int>::type = 0>
    inline orientation_type circle_plane_orientation(const Point& p, const Length& r, const Point1& p1, const Point2& p2, const NumberComparisonPolicy& cmp)
    {
        BOOST_CONCEPT_ASSERT((PointConcept<Point>));
        BOOST_CONCEPT_ASSERT((PointConcept<Point1>));
        BOOST_CONCEPT_ASSERT((PointConcept<Point2>));

        using std::abs;

        auto v = normalize(p2-p1);
        auto n = left_normal(v);
		auto d = abs(scalar_projection(p - p1, n));
        return cmp.greater_than(d, r) 
           ? vector_vector_orientation(p - p1, v,cmp)
           : oriented_collinear;
    }
    
    template <typename Sphere, typename Point1, typename Point2, typename NumberComparisonPolicy, typename std::enable_if<is_sphere<Sphere>::value, int>::type = 0>
    inline orientation_type circle_plane_orientation(const Sphere& s, const Point1& p1, const Point2& p2, const NumberComparisonPolicy& cmp)
    {
        return circle_line_orientation(get_center(s), get_radius(s), p1, p2, cmp);
    }

}//! namespace geometrix;

