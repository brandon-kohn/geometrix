//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_POINT_SEGMENT_DISTANCE_HPP
#define GEOMETRIX_POINT_SEGMENT_DISTANCE_HPP

#include <geometrix/algorithm/distance/point_point_distance.hpp>
#include <geometrix/algebra/algebra.hpp>
#include <geometrix/primitive/segment.hpp>
#include <geometrix/numeric/constants.hpp>
#include <geometrix/utility/utilities.hpp>

namespace geometrix {
    namespace result_of
    {
        template <typename T1, typename T2>
        struct point_segment_distance
            : point_point_distance
            <
            T1
            , typename geometric_traits<typename geometrix::remove_const_ref<T2>::type>::point_type
            >
        {};

        template <typename T1, typename T2>
        struct point_segment_distance_sqrd
            : point_point_distance_sqrd
            <
            T1
            , typename geometric_traits<typename geometrix::remove_const_ref<T2>::type>::point_type
            >
        {};

    }//namespace result_of;

    template <typename Point, typename PointA, typename PointB>
    inline typename result_of::point_point_distance_sqrd<Point, PointA>::type point_segment_distance_sqrd(const Point& p, const PointA& A, const PointB& B)
    {
        typedef typename select_arithmetic_type_from_sequences<PointA, Point>::type arithmetic_type;
        typedef vector<arithmetic_type, dimension_of<Point>::value> vector_t;
        vector_t ab = B - A;
        vector_t ac = p - A;
        vector_t bc = p - B;
        auto e = dot_product(ac, ab);
        if (e <= construct<decltype(e)>(0))
            return dot_product(ac, ac);
        auto f = dot_product(ab, ab);
        if (e >= f)
            return dot_product(bc, bc);
        return dot_product(ac, ac) - e * e / f;
    }

    template <typename Point, typename Segment>
    inline typename result_of::point_segment_distance_sqrd<Point, Segment>::type point_segment_distance_sqrd(const Point& p, const Segment& seg)
    {
        return point_segment_distance_sqrd(p, get_start(seg), get_end(seg));
    }

    template <typename Point, typename Segment>
    inline typename result_of::point_segment_distance<Point, Segment>::type point_segment_distance(const Point& p, const Segment& s)
    {
        using std::sqrt;
        return sqrt(point_segment_distance_sqrd(p, get_start(s), get_end(s)));
    }

}//namespace geometrix;

#endif //GEOMETRIX_POINT_SEGMENT_DISTANCE_HPP
