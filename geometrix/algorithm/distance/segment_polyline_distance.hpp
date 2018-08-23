//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_SEGMENT_POLYLINE_DISTANCE_HPP
#define GEOMETRIX_SEGMENT_POLYLINE_DISTANCE_HPP

#include <geometrix/algorithm/distance/segment_segment_distance.hpp>
#include <geometrix/primitive/point_sequence_traits.hpp>

namespace geometrix {
    namespace result_of {
        template <typename Segment, typename Polyline, typename T3 = void, typename T4 = void>
        struct segment_polyline_distance_sqrd
        {
		private:
			using point_t = typename geometric_traits<Polyline>::point_type;
			using seg_point_t = typename geometric_traits<Segment>::point_type;
		public:
            using type = typename result_of::segment_segment_distance_sqrd<seg_point_t, seg_point_t, point_t, point_t>::type;
        };

        template <typename Point1, typename Point2, typename Polyline>
        struct segment_polyline_distance_sqrd<Point1, Point2, Polyline, typename geometric_traits<Point2>::is_point>
        {
		private:
			using point_t = typename geometric_traits<Polyline>::point_type;
		public:
            using type = typename result_of::segment_segment_distance_sqrd<Point1, Point2, point_t, point_t>::type;
        };
    }//! namespace result_of;

    template <typename Point1, typename Point2, typename Polyline, typename NumberComparisonPolicy>
    inline typename result_of::segment_polyline_distance_sqrd<Point1, Point2, Polyline>::type segment_polyline_distance_sqrd(const Point1& p1, const Point2& p2, const Polyline& poly, const NumberComparisonPolicy& cmp)
    {
        using access = point_sequence_traits<Polyline>;
        using point_t= typename geometric_traits<Polyline>::point_type;
        using distance_sqrd_t= typename result_of::segment_polyline_distance_sqrd<Point1, Point2, point_t, point_t>::type;
        auto distance = std::numeric_limits<distance_sqrd_t>::infinity();
        auto size = access::size(poly);
        for (std::size_t i = 0, j = 1; j < size; i = j++)
            distance = (std::min)(distance, segment_segment_distance_sqrd(p1, p2, access::get_point(poly, i), access::get_point(poly, j), cmp));

        return distance;
    }

    template <typename Segment, typename Polyline, typename NumberComparisonPolicy>
    inline typename result_of::segment_polyline_distance_sqrd<Segment, Polyline>::type segment_polyline_distance_sqrd(const Segment& seg, const Polyline& poly, const NumberComparisonPolicy& cmp)
    {
        return segment_polyline_distance_sqrd(get_start(seg), get_end(seg), poly, cmp); 
    }

    namespace result_of {
        template <typename Segment, typename Polyline, typename T3 = void, typename T4 = void>
        struct segment_polyline_distance
        {
		private:
            using point_t = typename geometric_traits<Polyline>::point_type;
            using seg_point_t = typename geometric_traits<Segment>::point_type;
		public:
            using type = typename result_of::segment_segment_distance<seg_point_t, seg_point_t, point_t, point_t>::type;
        };

        template <typename Point1, typename Point2, typename Polyline>
        struct segment_polyline_distance<Point1, Point2, Polyline, typename geometric_traits<Point2>::is_point>
        {
		private:
            using point_t = typename geometric_traits<Polyline>::point_type;
		public:
            using type = typename result_of::segment_segment_distance<Point1, Point2, point_t, point_t>::type;
        };
    }

    template <typename Segment, typename Polyline, typename NumberComparisonPolicy>
    inline typename result_of::segment_polyline_distance<Segment, Polyline>::type segment_polyline_distance(const Segment& seg, const Polyline& poly, const NumberComparisonPolicy& cmp)
    {
        using std::sqrt;
        return sqrt(segment_polyline_distance_sqrd(seg, poly, cmp));
    }

    template <typename Point1, typename Point2, typename Polyline, typename NumberComparisonPolicy>
    inline typename result_of::segment_polyline_distance<Point1, Point2, Polyline>::type segment_polyline_distance(const Point1& p1, const Point2& p2, const Polyline& poly, const NumberComparisonPolicy& cmp)
    {
        using std::sqrt;
        return sqrt(segment_polyline_distance_sqrd(p1, p2, poly, cmp));
    }
}//namespace geometrix;

#endif //GEOMETRIX_SEGMENT_POLYLINE_DISTANCE_HPP
