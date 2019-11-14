//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_EBERLY_SEGMENT_SEGMENT_DISTANCE_HPP
#define GEOMETRIX_EBERLY_SEGMENT_SEGMENT_DISTANCE_HPP

#include <geometrix/algorithm/distance/point_point_distance.hpp>

namespace geometrix {

    namespace result_of
    {
        template <typename T1, typename T2>
        struct eberly_segment_segment_distance
            : point_point_distance
            <
              typename geometric_traits<typename geometrix::remove_const_ref<T1>::type>::point_type
            , typename geometric_traits<typename geometrix::remove_const_ref<T2>::type>::point_type
            >
        {};

        template <typename T1, typename T2>
        struct eberly_segment_segment_distance_sqrd
            : point_point_distance_sqrd
            <
              typename geometric_traits<typename geometrix::remove_const_ref<T1>::type>::point_type
            , typename geometric_traits<typename geometrix::remove_const_ref<T2>::type>::point_type
            >
        {};

    }//namespace result_of;

    template <typename Segment1, typename Segment2>
    inline typename result_of::eberly_segment_segment_distance_sqrd<Segment1, Segment2>::type eberly_segment_segment_distance_sqrd(const Segment1& s1, const Segment2& s2)
    {
        auto p1 = get_start(s1);
        auto p2 = get_end(s1);

        auto p3 = get_start(s2);
        auto p4 = get_end(s2);

        auto v1 = p2 - p1;
        auto v2 = p4 - p3;

        auto a = point_point_distance_sqrd(p1, p2);
        auto b = dot_product(-v1, v2);
        auto c = point_point_distance_sqrd(p3, p4);

        auto vd = p1 - p3;
        auto d = dot_product(v1, vd);
        auto e = dot_product(-v2, vd);
        auto det = exterior_product_area(v1, v2);
        auto s = b*e - c*d;
        auto t = b*d - a*e;

        if (det == 0)
        {
            if (b > 0)
            {
                if (d >= 0)
                {
                    s = 0;
                    t = 0;
                }
                else if (-d <= a)
                {
                    s = -d / a;
                    t = 0;
                }
                else
                {
                    s = 1;
                    auto tmp= a + d;
                    if (-tmp >= b)
                        t = 1;
                    else
                        t = -tmp / b;
                }
            }
            else
            {
                if (-d >= a)
                {
                    s = 1;
                    t = 0;
                }
                else if (d <= 0)
                {
                    s = -d / a;
                    t = 0;
                }
                else
                {
                    s = 0;
                    auto tmp= a + d;
                    if (-tmp >= b)
                        t = 1;
                    else
                        t = -tmp / b;
                }
            }
        }
        else
        {
            if (s >= 0)
            {
                if (s <= det)
                {
                    if (t >= 0)
                    {
                        if (t <= det)
                        {
                            //! Region 0
                            auto invDet= 1.0 / det;
                            s *= invDet;
                            t *= invDet;
                        }
                        else
                        {
                            //Region 3
                            s = 1.0;
                            auto tmp= b + e;
                            if (tmp > 0)
                                t = 0;
                            else if (-tmp > c)
                                t = 1;
                            else
                                t = -tmp / c;
                        }
                    }
                    else
                    {
                        //Region 7
                        s = 1.0;
                        auto tmp= b + e;
                        if (tmp > 0)
                            t = 0;
                        else if (-tmp > c)
                            t = 1;
                        else
                            t = -tmp / c;
                    }
                }
                else
                {
                    if (t >= 0)
                    {
                        if (t <= det)
                        {
                            //Region 1
                            s = 1.0;
                            auto tmp= b + e;
                            if (tmp > 0)
                                t = 0;
                            else if (-tmp > c)
                                t = 1;
                            else
                                t = -tmp / c;
                        }
                        else
                        {
                            //Region 2
                            auto tmp= b + d;
                            if (-tmp < a)
                            {
                                t = 1;
                                if (tmp > 0)
                                    s = 0;
                                else
                                    s = -tmp / a;
                            }
                            else
                            {
                                s = 1;
                                auto tmp2= b + e;
                                if (-tmp2 < c)
                                {
                                    if (tmp2 > 0)
                                        t = 0;
                                    else
                                        t = -tmp2 / c;
                                }
                                else
                                    t = 1;
                            }
                        }
                    }
                    else
                    {
                        //Region 8
                        auto tmp= b + d;
                        if (-tmp < a)
                        {
                            t = 1;
                            if (tmp > 0)
                                s = 0;
                            else
                                s = -tmp / a;
                        }
                        else
                        {
                            s = 1;
                            auto tmp2= b + e;
                            if (-tmp2 < c)
                            {
                                if (tmp2 > 0)
                                    t = 0;
                                else
                                    t = -tmp2 / c;
                            }
                            else
                                t = 1;
                        }
                    }
                }
            }
            else
            {
                if (t >= 0)
                {
                    if (t <= det)
                    {
                        //Region 5
                        s = 1.0;
                        auto tmp= b + e;
                        if (tmp > 0)
                            t = 0;
                        else if (-tmp > c)
                            t = 1;
                        else
                            t = -tmp / c;
                    }
                    else
                    {
                        //Region 4
                        auto tmp= b + d;
                        if (-tmp < a)
                        {
                            t = 1;
                            if (tmp > 0)
                                s = 0;
                            else
                                s = -tmp / a;
                        }
                        else
                        {
                            s = 1;
                            auto tmp2= b + e;
                            if (-tmp2 < c)
                            {
                                if (tmp2 > 0)
                                    t = 0;
                                else
                                    t = -tmp2 / c;
                            }
                            else
                                t = 1;
                        }
                    }
                }
                else
                {
                    //Region 6
                    auto tmp = b + d;
                    if (-tmp < a)
                    {
                        t = 1;
                        if (tmp > 0)
                            s = 0;
                        else
                            s = -tmp / a;
                    }
                    else
                    {
                        s = 1;
                        auto tmp2 = b + e;
                        if (-tmp2 < c)
                        {
                            if (tmp2 > 0)
                                t = 0;
                            else
                                t = -tmp2 / c;
                        }
                        else
                            t = 1;
                    }
                }
            }
        }

        if (s == 0)
        {
            if (t == 0)
                return magnitude_sqrd(p1 - p3);
            else if (t == 1)
                return magnitude_sqrd(p1 - p4);
            else
                return magnitude_sqrd(p1 - (p3 + t * v2));
        }
        else if (s == 1)
        {
            if (t == 0)
                return magnitude_sqrd(p2 - p3);
            else if (t == 1)
                return magnitude_sqrd(p2 - p4);
            else
                return magnitude_sqrd(p2 - (p3 + t * v2));
        }
        else if (t == 0)
            return magnitude_sqrd(p3 - (p1 + s * v1));
        else if (t == 1)
            return magnitude_sqrd(p4 - (p1 + s * v1));
        else
            return magnitude_sqrd((p1 + s * v1) - (p3 + t * v2));
    }
}//namespace geometrix;

#endif //! GEOMETRIX_EBERLY_SEGMENT_SEGMENT_DISTANCE_HPP
