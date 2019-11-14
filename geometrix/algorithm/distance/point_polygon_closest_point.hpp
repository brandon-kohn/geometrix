//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_POINT_POLYGON_CLOSEST_POINT_HPP
#define GEOMETRIX_POINT_POLYGON_CLOSEST_POINT_HPP

#include <geometrix/algorithm/distance/point_segment_closest_point.hpp>
#include <geometrix/algorithm/distance/point_segment_distance.hpp>

namespace geometrix {

    namespace result_of{

        template <typename Point, typename Polygon>
        struct point_polygon_closest_point
        {
        private:
            using segment_type = segment<typename geometric_traits<Polygon>::point_type>;

        public:
            using type = typename point_segment_closest_point<Point, segment_type>::type;
        };

    }//! namespace result_of;

    template <typename Point, typename Polygon>
    inline typename result_of::point_polygon_closest_point<Point, Polygon>::type point_polygon_closest_point(const Point& p, const Polygon& poly)
    {
        using access = point_sequence_traits<Polygon>;
        using segment_type = segment<typename geometric_traits<Polygon>::point_type>;
        using distance_sqrd_type = typename result_of::point_segment_distance_sqrd<Point, segment_type>::type;
        auto distance = std::numeric_limits<distance_sqrd_type>::infinity();
        std::size_t minSegmentI = 0UL, minSegmentJ = 0UL;
        auto size = access::size(poly);
        for (std::size_t i = size - 1, j = 0; j < size; i = j++)
        {
            auto ldistance = point_segment_distance_sqrd(p, access::get_point(poly, i), access::get_point(poly, j));
            if (ldistance < distance)
            {
                distance = ldistance;
                minSegmentI = i;
                minSegmentJ = j;
            }
        }

        return point_segment_closest_point(p, access::get_point(poly, minSegmentI), access::get_point(poly, minSegmentJ));
    }

    template <typename Point, typename PolygonCollection>
    inline typename result_of::point_polygon_closest_point<Point, typename PolygonCollection::value_type>::type point_polygon_collection_closest_point(const Point& p, const PolygonCollection& polygons)
    {
        using polygon_t = typename PolygonCollection::value_type;
        using access = point_sequence_traits<polygon_t>;
        using segment_type = segment<typename geometric_traits<polygon_t>::point_type>;
        using distance_sqrd_type = typename result_of::point_segment_distance_sqrd<Point, segment_type>::type;
        auto distance = std::numeric_limits<distance_sqrd_type>::infinity();
        std::size_t minSegmentI = 0UL, minSegmentJ = 0UL;
        polygon_t const* pgon = nullptr;
        for (const auto& poly : polygons) {
            auto size = access::size(poly);
            for (std::size_t i = size - 1, j = 0; j < size; i = j++) {
                auto ldistance = point_segment_distance_sqrd(p, access::get_point(poly, i), access::get_point(poly, j));
                if (ldistance < distance) {
                    pgon = &poly;
                    distance = ldistance;
                    minSegmentI = i;
                    minSegmentJ = j;
                }
            }
        }

        GEOMETRIX_ASSERT(pgon);
        return point_segment_closest_point(p, access::get_point(*pgon, minSegmentI), access::get_point(*pgon, minSegmentJ));
    }
}//namespace geometrix;

#endif //GEOMETRIX_POINT_POLYGON_CLOSEST_POINT_HPP
