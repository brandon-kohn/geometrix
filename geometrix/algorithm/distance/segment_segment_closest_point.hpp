//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_SEGMENT_SEGMENT_CLOSEST_POINT_HPP
#define GEOMETRIX_SEGMENT_SEGMENT_CLOSEST_POINT_HPP

#include <geometrix/algorithm/distance/point_point_distance.hpp>
#include <geometrix/algebra/algebra.hpp>
#include <geometrix/primitive/segment.hpp>
#include <geometrix/numeric/constants.hpp>
#include <geometrix/utility/utilities.hpp>

namespace geometrix {
    //! From Real-Time Collision Detection.
    // Computes closest points C1 and C2 of S1(s)=P1+s*(Q1-P1) and
    // S2(t)=P2+t*(Q2-P2), returning s and t. Function result is squared 
    // distance between between S1(s) and S2(t)
    namespace result_of
    {
        template <typename Point>
        struct segment_segment_closest_point
            : point_point_distance_sqrd<Point, Point>
        {};
    }//! namespace result_of
    template <typename Point, typename NumberComparisonPolicy>
    inline typename result_of::segment_segment_closest_point<Point>::type segment_segment_closest_point(const Point& p1, const Point& q1, const Point& p2, const Point& q2, typename geometric_traits<Point>::dimensionless_type& s, typename geometric_traits<Point>::dimensionless_type& t, Point &c1, Point &c2, const NumberComparisonPolicy& cmp)
    {
        using dimensionless_t = typename geometric_traits<Point>::dimensionless_type;
        using length_t = typename geometric_traits<Point>::arithmetic_type;
        using area_t = decltype(length_t() * length_t());
        using vector_t = vector<length_t, 2>;

        const auto zeroArea = constants::zero<area_t>();
        const auto zero = constants::zero<dimensionless_t>();
        const auto one = constants::one<dimensionless_t>();

        vector_t d1 = q1 - p1; // Direction vector of segment S1 
        vector_t d2 = q2 - p2; // Direction vector of segment S2 
        vector_t r = p1 - p2;
        area_t a = dot_product(d1, d1);
        // Squared length of segment S1, always nonnegative 
        area_t e = dot_product(d2, d2); // Squared length of segment S2, always nonnegative 
        area_t f = dot_product(d2, r);
        // Check if either or both segments degenerate into points 
        if (cmp.less_than_or_equal(a, zeroArea) && cmp.less_than_or_equal(e, zeroArea))
        {
            // Both segments degenerate into points 
            s = t = zero;
            c1 = p1;
            c2 = p2;
            return dot_product(c1 - c2, c1 - c2);
        }

        if (cmp.less_than_or_equal(a, zeroArea))
        {
            // First segment degenerates into a point 
            s = zero;
            t = f / e; // s = 0 => t =(b*s + f) / e = f / e 
            t = clamp<dimensionless_t>(t, zero, one);
        }
        else
        {
            area_t c = dot_product(d1, r);
            if (cmp.less_than_or_equal(e, zeroArea))
            {
                // Second segment degenerates into a point 
                t = zero;
                s = clamp<dimensionless_t>(construct<dimensionless_t>(-c / a), zero, one); // t = 0 => s =(b*t - c) / a = -c / a 
            }
            else
            {
                // The general nondegenerate case starts here 
                area_t b = dot_product(d1, d2);
                auto denom = get(a*e - b*b); // Always nonnegative
                                             // If segments not parallel, compute closest point on L1 to L2 and 
                                             // clamp to segment S1. Else pick arbitrary s (here 0) 
                if (denom != construct<decltype(denom)>(0))
                    s = clamp<dimensionless_t>(construct<dimensionless_t>((b*f - c*e) / denom), zero, one);
                else
                    s = zero;
                // Compute point on L2 closest to S1(s) using 
                // t = dot_product((P1 + D1*s) - P2,D2) / dot_product(D2,D2) = (b*s + f) / e 
                //t = (b*s + f) / e;
                // If t in [0,1] done. Else clamp t, recompute s for the new value 
                // of t using s = dot_product((P2 + D2*t) - P1,D1) / dot_product(D1,D1)= (t*b - c) / a 
                // and clamp s to [0, 1] 
                area_t tnom = b*s + f;
                if (tnom < construct<area_t>(0))
                {
                    t = zero;
                    s = clamp<dimensionless_t>(construct<dimensionless_t>(-c / a), zero, one);
                }
                else if (tnom > e)
                {
                    t = one;
                    s = clamp<dimensionless_t>(construct<dimensionless_t>((b - c) / a), zero, one);
                }
                else
                {
                    t = tnom / e;
                }
            }
        }

        assign(c1, p1 + d1 * s);
        assign(c2, p2 + d2 * t);
        return dot_product(c1 - c2, c1 - c2);
    }

}//namespace geometrix;

#endif //GEOMETRIX_SEGMENT_SEGMENT_CLOSEST_POINT_HPP
