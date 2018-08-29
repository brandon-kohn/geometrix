//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_POINT_POLYGON_DISTANCE_HPP
#define GEOMETRIX_POINT_POLYGON_DISTANCE_HPP

#include <geometrix/algorithm/distance/point_segment_closest_point.hpp>
#include <geometrix/algorithm/distance/point_segment_distance.hpp>

namespace geometrix {
    namespace result_of {
        template <typename Point, typename Polygon>
        struct point_polygon_distance_sqrd
        {
            using segment_type = segment<typename geometric_traits<Polygon>::point_type>;
            using type = typename result_of::point_segment_distance_sqrd<Point, segment_type>::type;
        };
    }

    template <typename Point, typename Polygon>
    inline typename result_of::point_polygon_distance_sqrd<Point, Polygon>::type point_polygon_distance_sqrd(const Point& p, const Polygon& poly)
    {
        using access = point_sequence_traits<Polyline>;
        using length_t = typename geometric_traits<Point>::arithmetic_type;
        using area_t = decltype(std::declval<length_t>() * std::declval<length_t>());
        auto dist = std::numeric_limits<area_t>::infinity();
        const auto size = access::size(poly);
        for (size_t i = size - 1, j = 0; j < size; i = j++)
        {
            auto d = point_segment_distance_sqrd(p, access::get_point(poly, i), access::get_point(poly, j));
            if( d < dist )
                dist = d;
        }

        return dist;
    }

    namespace result_of {
        template <typename Point, typename Polygon>
        struct point_polygon_distance
        {
            using segment_type = segment<typename geometric_traits<Polygon>::point_type>;
            using type = typename result_of::point_segment_distance<Point, segment_type>::type;
        };
    }

    template <typename Point, typename Polygon>
    inline typename result_of::point_polygon_distance<Point, Polygon>::type point_polygon_distance(const Point& p, const Polygon& poly)
    {
        using std::sqrt;
        return sqrt(point_polygon_distance_sqrd(p, poly));
    }
}//namespace geometrix;

#endif //GEOMETRIX_POINT_POLYGON_DISTANCE_HPP
