//
//! Copyright © 2021
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_ALGORITHM_POLYGON_FROM_POINTS_HPP
#define GEOMETRIX_ALGORITHM_POLYGON_FROM_POINTS_HPP
#pragma once

#include <geometrix/primitive/point_sequence_traits.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/algebra/algebra.hpp>
#include <geometrix/algorithm/distance/point_point_distance.hpp>
#include <geometrix/utility/point_angle_compare.hpp>
#include <boost/container/flat_set.hpp>
#include <algorithm>

namespace geometrix {

    template <typename Polygon, typename Point, typename Points, typename NumberComparisonPolicy>
    inline Polygon convex_polygon_from_points(const Point& centralPoint, const Points& hullPoints, const NumberComparisonPolicy& cmp)
    {
        using length_t = typename arithmetic_type_of<Point>::type;
        using vector_t = vector<length_t, dimension_of<Point>::value>;

        Polygon result;
        using access = point_sequence_traits<Points>;
    
        auto cmprer = point_angle_compare<NumberComparisonPolicy>(centralPoint, cmp); 
        boost::container::flat_set<Point, decltype(cmprer)> sortedPoints(access::begin(hullPoints), access::end(hullPoints), cmprer);

        for(auto const& p : sortedPoints)
            access::push_back(result, p);

        return result;
    }

}//namespace geometrix;

#endif //! GEOMETRIX_ALGORITHM_POLYGON_FROM_POINTS_HPP
