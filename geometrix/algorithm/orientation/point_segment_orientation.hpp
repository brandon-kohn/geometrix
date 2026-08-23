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
#include <geometrix/tensor/tensor_access_policy.hpp>

namespace geometrix {

    //! Orientation test to check if point A is left, collinear, or right of the line formed by B-C.
    //!
    //! Keep the point overload flattened rather than constructing the two
    //! temporary vectors A-B and C-B. This is a particularly hot primitive in
    //! segment intersection classification and BVH visibility queries.
    template <typename Point1, typename Point2, typename Point3, typename NumberComparisonPolicy>
    inline orientation_type point_segment_orientation(
        const Point1& A,
        const Point2& B,
        const Point3& C,
        const NumberComparisonPolicy& cmp)
    {
        auto ay = get<1>(A) - get<1>(B);
        auto ax = get<0>(A) - get<0>(B);
        auto cx = get<0>(C) - get<0>(B);
        auto cy = get<1>(C) - get<1>(B);

        return detail::orientation(ay * cx, ax * cy, cmp);
    }

    //! Orientation test to check if point A is left, collinear, or right of the line formed by seg.
    template <typename Point1, typename Segment, typename NumberComparisonPolicy>
    inline orientation_type point_segment_orientation(const Point1& A, const Segment& seg, const NumberComparisonPolicy& cmp)
    {
        return point_segment_orientation(A, get_start(seg), get_end(seg), cmp);
    }

    //! Orientation test to check if point C is left, collinear, or right of the line formed by A-B.
    //! Old interface retained because segment intersection classification uses it heavily.
    //!
    //! This is algebraically identical to:
    //!     vector_vector_orientation(C-A, B-A, cmp)
    //! but avoids both temporary vector objects and the second generic dispatch.
    template <typename Point1, typename Point2, typename Point3, typename NumberComparisonPolicy>
    inline orientation_type get_orientation(
        const Point1& A,
        const Point2& B,
        const Point3& C,
        const NumberComparisonPolicy& cmp)
    {
        auto cy = get<1>(C) - get<1>(A);
        auto cx = get<0>(C) - get<0>(A);
        auto bx = get<0>(B) - get<0>(A);
        auto by = get<1>(B) - get<1>(A);

        return detail::orientation(cy * bx, cx * by, cmp);
    }

}//! namespace geometrix;
