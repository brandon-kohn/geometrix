//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_SEGMENT_POLYGON_DISTANCE_HPP
#define GEOMETRIX_SEGMENT_POLYGON_DISTANCE_HPP

#include <geometrix/algorithm/distance/segment_segment_distance.hpp>
#include <geometrix/primitive/point_sequence_traits.hpp>

namespace geometrix {
    namespace result_of {
        template <typename Segment, typename Polygon, typename T3 = void, typename T4 = void>
        struct segment_polygon_distance_sqrd
        {
		private:
            using seg_point_t = typename geometric_traits<Segment>::point_type;
            using point_t = typename geometric_traits<Polygon>::point_type;
		public:
            using type = typename result_of::segment_segment_distance_sqrd<seg_point_t, seg_point_t, point_t, point_t>::type;
        };

        template <typename Point1, typename Point2, typename Polygon>
        struct segment_polygon_distance_sqrd<Point1, Point2, Polygon, typename geometric_traits<Point2>::is_point>
        {
		private:
            using point_t = typename geometric_traits<Polygon>::point_type;
		public:
            using type = typename result_of::segment_segment_distance_sqrd<Point1, Point2, point_t, point_t>::type;
        };
    }//! namespace result_of;

    template <typename Point1, typename Point2, typename Polygon, typename NumberComparisonPolicy>
    inline typename result_of::segment_polygon_distance_sqrd<Point1, Point2, Polygon>::type segment_polygon_distance_sqrd(const Point1& p1, const Point2& p2, const Polygon& poly, const NumberComparisonPolicy& cmp)
    {
        using access = point_sequence_traits<Polygon>;
        using point_t = typename geometric_traits<Polygon>::point_type;
        using distance_sqrd_type = typename result_of::segment_segment_distance_sqrd<Point1, Point2, point_t, point_t>::type;
        auto distance = std::numeric_limits<distance_sqrd_type>::infinity();
        auto size = access::size(poly);
        for (std::size_t i = size-1, j = 0; j < size; i = j++)
            distance = (std::min)(distance, segment_segment_distance_sqrd(p1, p2, access::get_point(poly, i), access::get_point(poly, j), cmp));

        return distance;
    }

    template <typename Segment, typename Polygon, typename NumberComparisonPolicy>
    inline typename result_of::segment_polygon_distance_sqrd<Segment, Polygon>::type segment_polygon_distance_sqrd(const Segment& seg, const Polygon& poly, const NumberComparisonPolicy& cmp)
    {
        return segment_polygon_distance_sqrd(get_start(seg), get_end(seg), poly, cmp);
    }

    namespace result_of {
        template <typename Segment, typename Polygon, typename T3=void, typename T4=void>
        struct segment_polygon_distance
        {
		private:
            using seg_point_t = typename geometric_traits<Segment>::point_type;
            using point_t = typename geometric_traits<Polygon>::point_type;
		public:
            using type = typename result_of::segment_segment_distance<seg_point_t, seg_point_t, point_t, point_t>::type;
        };

        template <typename Point1, typename Point2, typename Polygon>
        struct segment_polygon_distance<Point1, Point2, Polygon, typename geometric_traits<Point2>::is_point>
        {
		private:
            using point_t = typename geometric_traits<Polygon>::point_type;
		public:
            using type = typename result_of::segment_segment_distance<Point1, Point2, point_t, point_t>::type;
        };
    }

    template <typename Segment, typename Polygon, typename NumberComparisonPolicy>
    inline typename result_of::segment_polygon_distance<Segment, Polygon>::type segment_polygon_distance(const Segment& seg, const Polygon& poly, const NumberComparisonPolicy& cmp)
    {
        using std::sqrt;
        return sqrt(segment_polygon_distance_sqrd(seg, poly, cmp));
    }

    template <typename Point1, typename Point2, typename Polygon, typename NumberComparisonPolicy>
    inline typename result_of::segment_polygon_distance<Point1, Point2, Polygon>::type segment_polygon_distance(const Point1& p1, const Point2& p2, const Polygon& poly, const NumberComparisonPolicy& cmp)
    {
        using std::sqrt;
        return sqrt(segment_polygon_distance_sqrd(p1, p2, poly, cmp));
    }
}//namespace geometrix;

#endif //GEOMETRIX_SEGMENT_POLYGON_DISTANCE_HPP
