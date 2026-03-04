//
//! Copyright ? 2008-2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include <geometrix/algorithm/intersection/segment_segment_intersection.hpp>
#include <geometrix/primitive/point_sequence_traits.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/algorithm/point_in_polygon.hpp>
#include <geometrix/arithmetic/arithmetic_promotion_policy.hpp>
#include <geometrix/tensor/numeric_sequence_compare.hpp>
#include <vector>

/////////////////////////////////////////////////////////////////////////////
//
// NAMESPACE
//
/////////////////////////////////////////////////////////////////////////////
namespace geometrix {   
    
    template<typename PointA, typename PointB, typename Polygon, typename Visitor, typename NumberComparisonPolicy>
    inline bool segment_polygon_intersection(const PointA& a, const PointB& b, const Polygon& poly, Visitor&& visitor, const NumberComparisonPolicy& cmp)
    {
        typedef typename select_arithmetic_type_from_sequences<PointA, PointB>::type length_t;
        typedef typename point<length_t, dimension_of<PointA>::value> point_type;
        using segment_t = segment<point_type>;
        typedef point_sequence_traits<Polygon> access;

        bool startInside = (point_polygon_containment_or_on_border(a, poly, cmp) != polygon_containment::exterior);
        bool endInside   = (point_polygon_containment_or_on_border(b, poly, cmp) != polygon_containment::exterior);

        std::vector<point_type> intersections;
        intersections.reserve(access::size(poly) * 2 + 2);

        std::size_t size = access::size(poly);
        for (std::size_t i = 0; i < size; ++i)
        {
            std::size_t j = (i + 1) % size;
            point_type xpoints[2];
            auto itype = segment_segment_intersection(a, b, access::get_point(poly, i), access::get_point(poly, j), xpoints, cmp);

            if (itype == e_crossing || itype == e_endpoint)
            {
                intersections.push_back(xpoints[0]);
            }
            else if (itype == e_overlapping)
            {
                intersections.push_back(xpoints[0]);
                intersections.push_back(xpoints[1]);
            }
        }

        if (!intersections.empty())
        {
            intersections.emplace_back(a);
            intersections.emplace_back(b);

            //! Sort points in order along the segment a->b (strict ordering).
            //! Key is dot(P-a, D) where D = (b-a). This is monotone with the segment parameter.
            using key_t = decltype(length_t()*length_t());

            auto param_key = [&](const point_type& p)
            {
                const auto dx = get<0>(b) - get<0>(a);
                const auto dy = get<1>(b) - get<1>(a);
                const auto px = get<0>(p) - get<0>(a);
                const auto py = get<1>(p) - get<1>(a);
                return px * dx + py * dy;
            };

            auto strict_lex_less = lexicographical_comparer<direct_comparison_policy>(direct_comparison_policy());

            std::sort(intersections.begin(), intersections.end(),
                [&](const point_type& p, const point_type& q)
                {
                    const auto kp = param_key(p);
                    const auto kq = param_key(q);

                    if (kp < kq) return true;
                    if (kq < kp) return false;

                    //! Tie-breaker to keep the comparator a strict weak ordering.
                    return strict_lex_less(p, q);
                });

            //! Merge near-duplicates using tolerance policy.
            intersections.erase(
                std::unique(intersections.begin(), intersections.end(),
                    [&](const point_type& p0, const point_type& p1)
                    {
                        return numeric_sequence_equals(p0, p1, cmp);
                    }),
                intersections.end());

            for (std::size_t i = 0; i + 1 < intersections.size(); ++i)
            {
                const auto& p0 = intersections[i];
                const auto& p1 = intersections[i + 1];

                if (point_polygon_containment_or_on_border(segment_mid_point(p0, p1), poly, cmp) != polygon_containment::exterior)
                {
                    auto keepGoing = visitor(p0, p1);
                    if (!keepGoing)
                        return true;
                }
            }

            return true;
        }
        else if (startInside && endInside &&
                point_polygon_containment_or_on_border(segment_mid_point(a, b), poly, cmp) != polygon_containment::exterior)
        {
            visitor(a, b);
            return true;
        }

        return false;
    }

    template<typename Segment, typename Polygon, typename Visitor, typename NumberComparisonPolicy>
    inline bool segment_polygon_intersection(const Segment& seg, const Polygon& poly, Visitor&& visitor, const NumberComparisonPolicy& cmp)
    {
        typedef typename geometric_traits<Segment>::point_type point_type;
        typedef point_sequence_traits<Polygon> access;

        const auto& A = get_start(seg);
        const auto& B = get_end(seg);

        bool startInside = (point_polygon_containment_or_on_border(A, poly, cmp) != polygon_containment::exterior);
        bool endInside   = (point_polygon_containment_or_on_border(B, poly, cmp) != polygon_containment::exterior);

        std::vector<point_type> intersections;
        intersections.reserve(access::size(poly) * 2 + 2);

        std::size_t size = access::size(poly);
        for (std::size_t i = 0; i < size; ++i)
        {
            std::size_t j = (i + 1) % size;
            point_type xpoints[2];
            auto itype = segment_segment_intersection(A, B, access::get_point(poly, i), access::get_point(poly, j), xpoints, cmp);

            if (itype == e_crossing || itype == e_endpoint)
            {
                intersections.push_back(xpoints[0]);
            }
            else if (itype == e_overlapping)
            {
                intersections.push_back(xpoints[0]);
                intersections.push_back(xpoints[1]);
            }
        }

        if (!intersections.empty())
        {
            intersections.emplace_back(A);
            intersections.emplace_back(B);

            //! Sort points in order along the segment A->B (strict ordering).
            //! Key is dot(P-A, D) where D = (B-A). This is monotone with the segment parameter.
            using key_t = typename select_arithmetic_type_from_sequences<point_type, point_type>::type;

			auto param_key = [&]( const point_type& p )
			{
				const auto dx = get<0>( B ) - get<0>( A );
				const auto dy = get<1>( B ) - get<1>( A );
				const auto px = get<0>( p ) - get<0>( A );
				const auto py = get<1>( p ) - get<1>( A );
				return px * dx + py * dy;
			};

            auto strict_lex_less = lexicographical_comparer<direct_comparison_policy>(direct_comparison_policy());

            std::sort(intersections.begin(), intersections.end(),
                [&](const point_type& p, const point_type& q)
                {
                    const auto kp = param_key(p);
                    const auto kq = param_key(q);

                    if (kp < kq) return true;
                    if (kq < kp) return false;

                    //! Tie-breaker to keep the comparator a strict weak ordering.
                    return strict_lex_less(p, q);
                });

            //! Merge near-duplicates using tolerance policy.
            intersections.erase(
                std::unique(intersections.begin(), intersections.end(),
                    [&](const point_type& p, const point_type& q)
                    {
                        return numeric_sequence_equals(p, q, cmp);
                    }),
                intersections.end());

            for (std::size_t i = 0; i + 1 < intersections.size(); ++i)
            {
                auto test = construct<Segment>(intersections[i], intersections[i + 1]);
                if (point_polygon_containment_or_on_border(segment_mid_point(test), poly, cmp) != polygon_containment::exterior)
                {
                    auto keepGoing = visitor(test);
                    if (!keepGoing)
                        return true;
                }
            }

            return true;
        }
        else if (startInside && endInside &&
                point_polygon_containment_or_on_border(segment_mid_point(seg), poly, cmp) != polygon_containment::exterior)
        {
            visitor(seg);
            return true;
        }

        return false;
    }

    //! \brief Compute whether the segment defined by A->B intersects a polygon.
    //! The type Visitor must define bool operator()(intersection_type, size_t i, size_t j, point<arithmetic_type_of<PointA,PointB>::type, 2>[2]) which processes 
    //! an intersection event between the segment and the i-j segment of the polygon 
    //! along with the resulting intersection points. The return value is whether the algorithm should cease.
    template <typename PointA, typename PointB, typename Polygon, typename Visitor, typename NumberComparisonPolicy>
    inline bool segment_polygon_border_intersect(const PointA& A, const PointB& B, const Polygon& poly, Visitor&& visitor, const NumberComparisonPolicy& cmp)
    {
        BOOST_CONCEPT_ASSERT((Point2DConcept<PointA>));
        BOOST_CONCEPT_ASSERT((Point2DConcept<PointB>));
        BOOST_CONCEPT_ASSERT((PointSequenceConcept<Polygon>));
        BOOST_CONCEPT_ASSERT((NumberComparisonPolicyConcept<NumberComparisonPolicy>));
        typedef point_sequence_traits<Polygon> access;
        typedef typename select_arithmetic_type_from_sequences<PointA, PointB>::type arithmetic_type;
        typedef point<arithmetic_type, 2> point_type;
        bool intersected = false;
        auto size = access::size(poly);
        for (std::size_t i = 0, j = 1; i < size; ++i, j = (j + 1) % size)
        {
            point_type xPoints[2];
            auto iType = segment_segment_intersection(A, B, access::get_point(poly, i), access::get_point(poly, j), xPoints, cmp);
            if (iType != e_non_crossing)
            {
                bool stop = visitor(iType, i, j, xPoints[0], xPoints[1]);
                if (stop)
                    return true;
                intersected = true;
            }
        }

        return intersected;
    }

    //! \brief Compute whether the segment defined by A->B intersects the polygon defined by t0, t1, t2.
    template <typename Segment, typename Polygon, typename Visitor, typename NumberComparisonPolicy>
    inline bool segment_polygon_border_intersect(const Segment& segment, const Polygon& poly, Visitor&& visitor, const NumberComparisonPolicy& cmp)
    {
        return segment_polygon_border_intersect(get_start(segment), get_end(segment), poly, std::forward<Visitor>(visitor), cmp);
    }
    
}//namespace geometrix;
