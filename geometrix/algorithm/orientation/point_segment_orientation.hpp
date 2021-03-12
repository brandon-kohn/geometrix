//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include <geometrix/algorithm/orientation/orientation_enum.hpp>
#include <geometrix/algorithm/orientation/vector_vector_orientation.hpp>

namespace geometrix {

    //! Orientation test to check if point A is left, collinear, or right of the line formed by B-C.
    template <typename Point1, typename Point2, typename Point3, typename NumberComparisonPolicy>
    inline orientation_type point_segment_orientation( const Point1& A, const Point2& B, const Point3& C, const NumberComparisonPolicy& cmp )
    {
        return vector_vector_orientation(A-B, C-B, cmp);
    }
    
    //! Orientation test to check if point A is left, collinear, or right of the line formed by seg.
    template <typename Point1, typename Segment, typename NumberComparisonPolicy>
    inline orientation_type point_segment_orientation( const Point1& A, const Segment& seg, const NumberComparisonPolicy& cmp )
    {
        return point_segment_orientation(A, get_start(seg), get_end(seg), cmp);
    }
    
    //! Orientation test to check if point C is left, collinear, or right of the line formed by A-B.
    //! Old interface.
    template <typename Point1, typename Point2, typename Point3, typename NumberComparisonPolicy>
    inline orientation_type get_orientation( const Point1& A, const Point2& B, const Point3& C, const NumberComparisonPolicy& cmp )
    {
        return vector_vector_orientation(C-A, B-A, cmp);
    }

}//! namespace geometrix;

