//
//! Copyright © 2018
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_ALGORITHM_SEGMENT_POLYLINE_CLOSEST_POINT_HPP
#define GEOMETRIX_ALGORITHM_SEGMENT_POLYLINE_CLOSEST_POINT_HPP
#pragma once

#include <geometrix/algorithm/distance/segment_segment_closest_point.hpp>
#include <geometrix/algorithm/distance/aabb_aabb_distance.hpp>
#include <geometrix/primitive/point_sequence_utilities.hpp>

namespace geometrix {
    namespace result_of {
        template <typename Point>
        struct segment_polyline_closest_point
        {
        private:
            using point_t = Point;
            using length_t = typename arithmetic_type_of<point_t>::type;
        public:

            using type = decltype(std::declval<length_t>() * std::declval<length_t>());
        };
    }//! namespace result_of;
  
    template <typename Point1, typename Point2, typename Polyline, typename Dimensionless, typename Point, typename NumberComparisonPolicy>
    inline typename result_of::segment_polyline_closest_point<Point>::type segment_polyline_closest_point(const Point1& p1, const Point2& p2, const Polyline& poly, std::size_t& segIndex, Dimensionless& s, Dimensionless& t, Point &c1, Point &c2, const NumberComparisonPolicy& cmp)
    {
        using access = point_sequence_traits<Polyline>;
		using dist_type = typename result_of::segment_polyline_closest_point<Point>::type;
        auto distance = std::numeric_limits<dist_type>::infinity();
        auto size = access::size(poly);
        for (std::size_t i = 0, j = 1; j < size; i = j++)
        {
            auto sl = Dimensionless{};
            auto tl = Dimensionless{};
            Point c1l, c2l;
            auto d2 = segment_segment_closest_point(p1, p2, access::get_point(poly, i), access::get_point(poly, j), sl, tl, c1l, c2l, cmp);
            if(d2 < distance)
            {
                distance = d2;
                s = sl;
                t = tl;
				segIndex = i;
                c1 = c1l;
                c2 = c2l;
            }
        }

        return distance;
    }

    template <typename Point1, typename Point2, typename Polyline, typename Dimensionless, typename Point, typename NumberComparisonPolicy>
    inline typename result_of::segment_polyline_closest_point<Point>::type segment_polyline_closest_point(const Point1& p1, const Point2& p2, const std::tuple<std::size_t, std::size_t>& subRange, const Polyline& poly, std::size_t& segIndex, Dimensionless& s, Dimensionless& t, Point &c1, Point &c2, const NumberComparisonPolicy& cmp)
    {
        using access = point_sequence_traits<Polyline>;
		using dist_type = typename result_of::segment_polyline_closest_point<Point>::type;
        auto distance = std::numeric_limits<dist_type>::infinity();
        auto size = access::size(poly);
        for (std::size_t i = std::get<0>(subRange), j = std::get<0>(subRange) + 1; j <= std::get<1>(subRange); i = j++)
        {
            auto sl = Dimensionless{};
            auto tl = Dimensionless{};
            Point c1l, c2l;
            auto d2 = segment_segment_closest_point(p1, p2, access::get_point(poly, i), access::get_point(poly, j), sl, tl, c1l, c2l, cmp);
            if(d2 < distance)
            {
                distance = d2;
				segIndex = i;
                s = sl;
                t = tl;
                c1 = c1l;
                c2 = c2l;
            }
        }

        return distance;
    }

	template <typename Segment, typename Polyline, typename Dimensionless, typename Point, typename NumberComparisonPolicy>
    inline typename result_of::segment_polyline_closest_point<Point>::type segment_polyline_closest_point(const Segment& seg, const Polyline& poly, std::size_t& segIndex, Dimensionless& s, Dimensionless& t, Point &c1, Point &c2, const NumberComparisonPolicy& cmp)
    {
		return segment_polyline_closest_point(get_start(seg), get_end(seg), poly, segIndex, s, t, c1, c2, cmp);
    }

    template <typename Segment, typename Polyline, typename Dimensionless, typename Point, typename NumberComparisonPolicy>
    inline typename result_of::segment_polyline_closest_point<Point>::type segment_polyline_closest_point(const Segment& seg, const std::tuple<std::size_t, std::size_t>& subRange, const Polyline& poly, std::size_t& segIndex, Dimensionless& s, Dimensionless& t, Point &c1, Point &c2, const NumberComparisonPolicy& cmp)
    {
		return segment_polyline_closest_point(get_start(seg), get_end(seg), subRange, poly, segIndex, s, t, c1, c2, cmp);
    }
}//namespace geometrix;

#endif //GEOMETRIX_ALGORITHM_SEGMENT_POLYLINE_CLOSEST_POINT_HPP

