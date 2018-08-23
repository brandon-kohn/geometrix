//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_SEGMENT_SEGMENT_DISTANCE_HPP
#define GEOMETRIX_SEGMENT_SEGMENT_DISTANCE_HPP

#include <geometrix/algorithm/distance/segment_segment_closest_point.hpp>

namespace geometrix {

    namespace result_of
    {
        template <typename T1, typename T2, typename T3 = void, typename T4 = void, typename T5 = void, typename T6 = void, typename T7 = void, typename T8 = void>
        struct segment_segment_distance
            : point_point_distance
            <
              typename geometric_traits<typename geometrix::remove_const_ref<T1>::type>::point_type
            , typename geometric_traits<typename geometrix::remove_const_ref<T2>::type>::point_type
            >
        {};

        template <typename T1, typename T2, typename T3 = void, typename T4 = void, typename T5 = void, typename T6 = void, typename T7 = void, typename T8 = void>
        struct segment_segment_distance_sqrd
            : point_point_distance_sqrd
            <
              typename geometric_traits<typename geometrix::remove_const_ref<T1>::type>::point_type
            , typename geometric_traits<typename geometrix::remove_const_ref<T2>::type>::point_type
            >
        {};

        template <typename Point1, typename Point2, typename Point3, typename Point4>
        struct segment_segment_distance<Point1, Point2, Point3, Point4, typename geometric_traits<Point1>::is_point, typename geometric_traits<Point2>::is_point, typename geometric_traits<Point3>::is_point, typename geometric_traits<Point4>::is_point>
            : point_point_distance
            <
              typename geometric_traits<typename geometrix::remove_const_ref<Point1>::type>::point_type
            , typename geometric_traits<typename geometrix::remove_const_ref<Point3>::type>::point_type
            >
        {};

        template <typename Point1, typename Point2, typename Point3, typename Point4>
        struct segment_segment_distance_sqrd<Point1, Point2, Point3, Point4, typename geometric_traits<Point1>::is_point, typename geometric_traits<Point2>::is_point, typename geometric_traits<Point3>::is_point, typename geometric_traits<Point4>::is_point>
            : point_point_distance_sqrd
            <
              typename geometric_traits<typename geometrix::remove_const_ref<Point1>::type>::point_type
            , typename geometric_traits<typename geometrix::remove_const_ref<Point3>::type>::point_type
            >
        {};
    }//namespace result_of;

    template <typename Segment1, typename Segment2, typename NumberComparisonPolicy>
    inline typename result_of::segment_segment_distance_sqrd<Segment1, Segment2>::type segment_segment_distance_sqrd(const Segment1& s1, const Segment2& s2, const NumberComparisonPolicy& cmp)
    {
        auto p1 = get_start(s1);
        auto p2 = get_end(s1);
        auto p3 = get_start(s2);
        auto p4 = get_end(s2);

        typedef typename geometric_traits<Segment1>::point_type point_type;
        typedef typename geometric_traits<point_type>::dimensionless_type dimensionless_t;
        dimensionless_t s, t;
        point_type c1, c2;
        return segment_segment_closest_point(p1, p2, p3, p4, s, t, c1, c2, cmp);
    }

    template <typename Segment1, typename Segment2, typename NumberComparisonPolicy>
    inline typename result_of::segment_segment_distance<Segment1, Segment2>::type segment_segment_distance(const Segment1& s1, const Segment2& s2, const NumberComparisonPolicy& cmp)
    {
        auto p1 = get_start(s1);
        auto p2 = get_end(s1);
        auto p3 = get_start(s2);
        auto p4 = get_end(s2);

        typedef typename geometric_traits<Segment1>::point_type point_type;
        typedef typename geometric_traits<point_type>::dimensionless_type dimensionless_t;
        dimensionless_t s, t;
        point_type c1, c2;
        using std::sqrt;
        return sqrt(segment_segment_closest_point(p1, p2, p3, p4, s, t, c1, c2, cmp));
    }

    template <typename Point1, typename Point2, typename Point3, typename Point4, typename NumberComparisonPolicy>
    inline typename result_of::segment_segment_distance_sqrd<Point1, Point2, Point3, Point4>::type segment_segment_distance_sqrd(const Point1& p1, const Point2& p2, const Point3& p3, const Point4& p4, const NumberComparisonPolicy& cmp)
    {
        using length_t = typename arithmetic_type_of<Point1>::type;
        using point_t = point<length_t, dimension_of<Point1>::value>;
        using dimensionless_t = typename dimensionless_type_of<Point1>::type;
        dimensionless_t s, t;
        point_t c1, c2;
        return segment_segment_closest_point(p1, p2, p3, p4, s, t, c1, c2, cmp);
    }

    template <typename Point1, typename Point2, typename Point3, typename Point4, typename NumberComparisonPolicy>
    inline typename result_of::segment_segment_distance<Point1, Point2, Point3, Point4>::type segment_segment_distance(const Point1& p1, const Point2& p2, const Point3& p3, const Point4& p4, const NumberComparisonPolicy& cmp)
    {
        using std::sqrt;
        using length_t = typename arithmetic_type_of<Point1>::type;
        using point_t = point<length_t, dimension_of<Point1>::value>;
        using dimensionless_t = typename dimensionless_type_of<Point1>::type;
        dimensionless_t s, t;
        point_t c1, c2;
        return sqrt(segment_segment_closest_point(p1, p2, p3, p4, s, t, c1, c2, cmp));
    }

}//namespace geometrix;

#endif //GEOMETRIX_SEGMENT_SEGMENT_DISTANCE_HPP
