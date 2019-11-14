//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_POINT_POLYLINE_CLOSEST_POINT_HPP
#define GEOMETRIX_POINT_POLYLINE_CLOSEST_POINT_HPP

#include <geometrix/algorithm/distance/point_segment_closest_point.hpp>
#include <geometrix/algorithm/distance/point_segment_distance.hpp>

namespace geometrix {
    namespace result_of{

        template <typename Point, typename Polyline>
        struct point_polyline_closest_point
        {
        private:
            using segment_type = segment<typename geometric_traits<Polyline>::point_type>;
        public:

            using type = typename point_segment_closest_point<Point, segment_type>::type;

        };

    }//! namespace result_of;

    template <typename Point, typename Polyline>
    inline typename result_of::point_polyline_closest_point<Point, Polyline>::type point_polyline_closest_point(const Point& p, const Polyline& poly)
    {
        using access = point_sequence_traits<Polyline>;
        using segment_type = segment<typename geometric_traits<Polyline>::point_type>;
        using distance_sqrd_type = typename result_of::point_segment_distance_sqrd<Point, segment_type>::type;
        auto distance = std::numeric_limits<distance_sqrd_type>::infinity();
        std::size_t minSegmentI = 0UL, minSegmentJ = 0UL;
        auto size = access::size(poly);
        for (std::size_t i = 0, j = 1; j < size; i = j++)
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
}//namespace geometrix;

#endif //GEOMETRIX_POINT_POLYLINE_CLOSEST_POINT_HPP
