//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_AABB_AABB_INTERSECTION_HPP
#define GEOMETRIX_AABB_AABB_INTERSECTION_HPP
#pragma once

#include <geometrix/utility/array.hpp>
#include <geometrix/primitive/point_sequence_traits.hpp>
#include <geometrix/primitive/segment_traits.hpp>
#include <geometrix/primitive/axis_aligned_bounding_box.hpp>
#include <boost/algorithm/minmax.hpp>

namespace geometrix {

    //! Bounding box intersection test between two boxes (defined by segment diagonals p1->p2 and p3->p4.)
    template <typename Point1, typename Point2, typename NumberComparisonPolicy>
    inline bool aabb_aabb_intersection( const axis_aligned_bounding_box<Point1>& aabb1, const axis_aligned_bounding_box<Point2>& aabb2, const NumberComparisonPolicy& compare ) 
    {
        return aabb1.intersects(aabb2, compare);
    }
	
}//namespace geometrix;

#endif //GEOMETRIX_AABB_AABB_INTERSECTION_HPP
