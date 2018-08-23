//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_POINT_POINT_DISTANCE_HPP
#define GEOMETRIX_POINT_POINT_DISTANCE_HPP

#include <geometrix/tensor/vector_traits.hpp>
#include <geometrix/arithmetic/vector.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/arithmetic/arithmetic_promotion_policy.hpp>
#include <geometrix/algebra/algebra.hpp>
#include <geometrix/primitive/line.hpp>
#include <geometrix/primitive/segment.hpp>
#include <geometrix/primitive/point_sequence_traits.hpp>
#include <geometrix/numeric/constants.hpp>
#include <geometrix/utility/utilities.hpp>

#include <boost/fusion/algorithm/query/all.hpp>

namespace geometrix {

    namespace result_of
    {
        template <typename T1, typename T2>
        struct point_point_distance
            : magnitude
            <
                GEOMETRIX_TYPEOF_BINARY_EXPR_TPL('-', T2, T1)
            >
        {};

        template <typename T1, typename T2>
        struct point_point_distance_sqrd
            : magnitude_sqrd
            <
                GEOMETRIX_TYPEOF_BINARY_EXPR_TPL('-', T2, T1)
            >
        {};
    }//namespace result_of;

    namespace detail
    {
        template <typename Point1, typename Point2, std::size_t Index>
        struct distance_sqrd
        {
            typedef typename result_of::point_point_distance_sqrd<Point1, Point2>::type result_type;

            static result_type eval(const Point1& v1, const Point2& v2)
            {
                BOOST_AUTO(a, get<Index - 1>(v2) - get<Index - 1>(v1));
                return a * a + distance_sqrd<Point1, Point2, Index - 1>::eval(v1, v2);
            }
        };

        template <typename Point1, typename Point2>
        struct distance_sqrd<Point1, Point2, 0>
        {
            typedef typename result_of::point_point_distance_sqrd<Point1, Point2>::type result_type;
            static result_type eval(const Point1& v1, const Point2& v2)
            {
                BOOST_AUTO(a, get<0>(v2) - get<0>(v1));
                return a * a;
            }
        };

        template <typename Point1, typename Point2>
        struct distance_sqrd<Point1, Point2, 1>
        {
            typedef typename result_of::point_point_distance_sqrd<Point1, Point2>::type result_type;
            static result_type eval(const Point1& v1, const Point2& v2)
            {
                BOOST_AUTO(a, get<0>(v2) - get<0>(v1));
                BOOST_AUTO(b, get<1>(v2) - get<1>(v1));
                return a * a + b * b;
            }
        };

        template <typename Point1, typename Point2>
        struct distance_sqrd<Point1, Point2, 2>
        {
            typedef typename result_of::point_point_distance_sqrd<Point1, Point2>::type result_type;
            static result_type eval(const Point1& v1, const Point2& v2)
            {
                BOOST_AUTO(a, get<0>(v2) - get<0>(v1));
                BOOST_AUTO(b, get<1>(v2) - get<1>(v1));
                BOOST_AUTO(c, get<2>(v2) - get<2>(v1));
                return a * a + b * b + c * c;
            }
        };

        template <typename Point1, typename Point2>
        struct distance_sqrd<Point1, Point2, 3>
        {
            typedef typename result_of::point_point_distance_sqrd<Point1, Point2>::type result_type;
            static result_type eval(const Point1& v1, const Point2& v2)
            {
                BOOST_AUTO(a, get<0>(v2) - get<0>(v1));
                BOOST_AUTO(b, get<1>(v2) - get<1>(v1));
                BOOST_AUTO(c, get<2>(v2) - get<2>(v1));
                BOOST_AUTO(d, get<3>(v2) - get<3>(v1));
                return a * a + b * b + c * c + d * d;
            }
        };

    }//namespace detail;

    template <typename CoordinateSequence1, typename CoordinateSequence2>
    inline typename result_of::point_point_distance
        <
        CoordinateSequence1
        , CoordinateSequence2
        >::type
        point_point_distance(const CoordinateSequence1& a, const CoordinateSequence2& b)
    {
        using std::sqrt;
        return sqrt(detail::distance_sqrd<CoordinateSequence1, CoordinateSequence2, dimension_of<CoordinateSequence1>::value - 1>::eval(a, b));
    }

    template <typename CoordinateSequence1, typename CoordinateSequence2>
    inline typename result_of::point_point_distance_sqrd
        <
        CoordinateSequence1
        , CoordinateSequence2
        >::type
        point_point_distance_sqrd(const CoordinateSequence1& a, const CoordinateSequence2& b)
    {
        return detail::distance_sqrd<CoordinateSequence1, CoordinateSequence2, dimension_of<CoordinateSequence1>::value - 1>::eval(a, b);
    }

}//namespace geometrix;

#endif //GEOMETRIX_POINT_SEGMENT_DISTANCE_HPP
