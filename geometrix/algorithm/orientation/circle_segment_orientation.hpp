//
//! Copyright © 2021
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include <geometrix/algorithm/orientation/circle_line_orientation.hpp>
#include <geometrix/primitive/segment.hpp>


namespace geometrix {
    
    template <typename Point, typename Length, typename Point1, typename Point2, typename NumberComparisonPolicy, typename std::enable_if<is_point<Point>::value, int>::type = 0>
    inline orientation_type circle_segment_orientation(const Point& p, const Length& r, const Point1& p1, const Point2& p2, const NumberComparisonPolicy& cmp)
    {
        BOOST_CONCEPT_ASSERT((PointConcept<Point>));
        BOOST_CONCEPT_ASSERT((PointConcept<Point1>));
        BOOST_CONCEPT_ASSERT((PointConcept<Point2>));

        return circle_line_orientation(p, r, p1, p2, cmp);
    }

    template <typename Point, typename Length, typename Segment, typename NumberComparisonPolicy, typename std::enable_if<is_point<Point>::value, int>::type = 0>
    inline orientation_type circle_segment_orientation(const Point& p, const Length& r, const Segment& seg, const NumberComparisonPolicy& cmp)
    {
        BOOST_CONCEPT_ASSERT((PointConcept<Point>));
        BOOST_CONCEPT_ASSERT((SegmentConcept<Segment>));
        return circle_segment_orientation(p, r, get_start(seg), get_end(seg), cmp); 
    }
    
    template <typename Sphere, typename Length, typename Segment, typename NumberComparisonPolicy, typename std::enable_if<is_sphere<Sphere>::value, int>::type = 0>
    inline orientation_type circle_segment_orientation(const Sphere& s, const Segment& seg, const NumberComparisonPolicy& cmp)
    {
        BOOST_CONCEPT_ASSERT((SegmentConcept<Segment>));
        return circle_segment_orientation(get_center(s), get_radius(s), get_start(seg), get_end(seg), cmp); 
    }
    
}//! namespace geometrix;

