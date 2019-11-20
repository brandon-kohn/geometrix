//
//! Copyright © 2018
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_ALGORITHM_SEGMENT_POLYGON_CLOSEST_POINT_HPP
#define GEOMETRIX_ALGORITHM_SEGMENT_POLYGON_CLOSEST_POINT_HPP
#pragma once

#include <geometrix/algorithm/distance/segment_segment_closest_point.hpp>
#include <geometrix/algorithm/distance/aabb_aabb_distance.hpp>
#include <geometrix/primitive/point_sequence_utilities.hpp>
#include <geometrix/utility/ignore_unused_warnings.hpp>

namespace geometrix {
    namespace result_of {
        template <typename Point>
        struct segment_polygon_closest_point
        {
        private:
            using point_t = Point;
            using length_t = typename arithmetic_type_of<point_t>::type;
        public:

            using type = decltype(std::declval<length_t>() * std::declval<length_t>());
        };
    }//! namespace result_of;
  
    template <typename Point1, typename Point2, typename Polygon, typename Dimensionless, typename Point, typename NumberComparisonPolicy>
    inline typename result_of::segment_polygon_closest_point<Point>::type segment_polygon_closest_point(const Point1& p1, const Point2& p2, const Polygon& poly, std::size_t& segIndex, Dimensionless& s, Dimensionless& t, Point &c1, Point &c2, const NumberComparisonPolicy& cmp)
    {
        using access = point_sequence_traits<Polygon>;
		using dist_type = typename result_of::segment_polygon_closest_point<Point>::type;
        auto distance = std::numeric_limits<dist_type>::infinity();
        auto size = access::size(poly);
        for (std::size_t i = size-1, j = 0; j < size; i = j++)
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

    template <typename Point1, typename Point2, typename Polygon, typename Dimensionless, typename Point, typename NumberComparisonPolicy>
    inline typename result_of::segment_polygon_closest_point<Point>::type segment_polygon_closest_point(const Point1& p1, const Point2& p2, const std::tuple<std::size_t, std::size_t>& subRange, const Polygon& poly, std::size_t& segIndex, Dimensionless& s, Dimensionless& t, Point &c1, Point &c2, const NumberComparisonPolicy& cmp)
    {
        using access = point_sequence_traits<Polygon>;
		using dist_type = typename result_of::segment_polygon_closest_point<Point>::type;
        auto size = access::size(poly);
        ignore_unused_warning_of(size);
        std::size_t i0 = std::get<0>(subRange), i1 = std::get<1>(subRange);
        GEOMETRIX_ASSERT(i0 < i1 || i0 == size-1);//! this doesn't support wrapping around except for the segment just before 0.
        auto distance = std::numeric_limits<dist_type>::infinity();
        for (std::size_t i = i0, j = i0 + 1; j <= i1; i = j++)
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

	template <typename Segment, typename Polygon, typename Dimensionless, typename Point, typename NumberComparisonPolicy>
	inline typename result_of::segment_polygon_closest_point<Point>::type segment_polygon_closest_point(const Segment& seg, const Polygon& poly, std::size_t& segIndex, Dimensionless& s, Dimensionless& t, Point &c1, Point &c2, const NumberComparisonPolicy& cmp)
	{
		return segment_polygon_closest_point(get_start(seg), get_end(seg), poly, segIndex, s, t, c1, c2, cmp);
	}

	template <typename Segment, typename Polygon, typename Dimensionless, typename Point, typename NumberComparisonPolicy>
	inline typename result_of::segment_polygon_closest_point<Point>::type segment_polygon_closest_point(const Segment& seg, const std::tuple<std::size_t, std::size_t>& subRange, const Polygon& poly, std::size_t& segIndex, Dimensionless& s, Dimensionless& t, Point &c1, Point &c2, const NumberComparisonPolicy& cmp)
	{
		return segment_polygon_closest_point(get_start(seg), get_end(seg), subRange, poly, segIndex, s, t, c1, c2, cmp);
	}
}//namespace geometrix;

#endif //GEOMETRIX_ALGORITHM_SEGMENT_POLYGON_CLOSEST_POINT_HPP

