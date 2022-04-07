//
//! Copyright © 2021
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_CONVEX_POLYGON_POLYGON_INTERSECTION_HPP
#define GEOMETRIX_CONVEX_POLYGON_POLYGON_INTERSECTION_HPP
#pragma once

#include <geometrix/algorithm/intersection/segment_segment_intersection.hpp>
#include <geometrix/algorithm/orientation/point_segment_orientation.hpp>
#include <geometrix/primitive/point_sequence_traits.hpp>
#include <geometrix/primitive/polygon.hpp>
#include <geometrix/algorithm/point_in_polygon.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/arithmetic/arithmetic_promotion_policy.hpp>
#include <geometrix/utility/utilities.hpp>

#include <boost/container/flat_set.hpp>
#include <boost/container/small_vector.hpp>
#include <boost/container/new_allocator.hpp>
#include <boost/concept_check.hpp>

/////////////////////////////////////////////////////////////////////////////
//
// NAMESPACE
//
/////////////////////////////////////////////////////////////////////////////
namespace geometrix {
    enum class polygon_intersection_type
    {
        none = 0,
        face = 1,
        vertex = 2,
        overlapping = 3
    };

    //! From Computational Geometry in C, O'Rourke.
    template <typename Polygon1, typename Polygon2, typename Visitor, typename NumberComparisonPolicy>
    inline polygon_intersection_type convex_polygon_polygon_intersection(const Polygon1& pgon1, const Polygon2& pgon2, Visitor&& visitor, const NumberComparisonPolicy& cmp)
    {
        BOOST_CONCEPT_ASSERT((PointSequenceConcept<Polygon1>));
        BOOST_CONCEPT_ASSERT((PointSequenceConcept<Polygon2>));
        BOOST_CONCEPT_ASSERT((NumberComparisonPolicyConcept<NumberComparisonPolicy>));
        using access1 = point_sequence_traits<Polygon1>;
        using access2 = point_sequence_traits<Polygon2>;
        using length_t = typename select_arithmetic_type_from_sequences<typename access1::point_type, typename access2::point_type>::type;
        using area_t = decltype(std::declval<length_t>() * std::declval<length_t>());
        using point_t = point<length_t, 2>;
        using vector_t = vector<length_t, 2>;
        enum in_out_state 
        { 
            pgon1_in,
            pgon2_in,
            unknown
        };
		auto pointCmp = [&]( const point_t& lhs, const point_t& rhs )
		{
			return lexicographically_less_than( lhs, rhs, cmp );
		};
	#if BOOST_VERSION >= 107000
		using small_flat_set = boost::container::flat_set<point_t, decltype(pointCmp), boost::container::small_vector<point_t, 20, boost::container::new_allocator<point_t>>>;
		auto visited = small_flat_set(pointCmp);
    #else
		auto visited = boost::container::flat_set<point_t, decltype( pointCmp )>( pointCmp );
    #endif
		auto shouldVisit = [&]( const point_t& p )
		{
			auto it = visited.lower_bound( p );
			if( it != visited.end() && !( visited.key_comp()( p, *it ) ) )
				return;

			visited.insert( it, p );
			visitor( p );
		};
        auto get_point1 = [&](std::size_t i) { return access1::get_point(pgon1, i); };
        auto get_point2 = [&](std::size_t i) { return access2::get_point(pgon2, i); };
        auto advance = [](std::size_t& a, std::size_t& aa, std::size_t n) { ++aa; ++a %= n; };
        auto a = std::size_t {};
        auto b = std::size_t {};
        auto aa = std::size_t {};
        auto ba = std::size_t {};
        auto inflag = unknown;

        auto n = access1::size(pgon1);
        auto n2 = n * 2;
        auto m = access2::size(pgon2);
        auto m2 = m * 2;

        auto nVisited = 0;
        do
        {
            auto a1 = (a + n - 1) % n;
            auto b1 = (b + m - 1) % m;

            vector_t A = get_point1(a) - get_point1(a1);
            vector_t B = get_point2(b) - get_point2(b1);

            auto ABOrientation = get_orientation(A, B, cmp);
            auto aHB = get_orientation(get_point2(b1), get_point2(b), get_point1(a), cmp);
            auto bHA = get_orientation(get_point1(a1), get_point1(a), get_point2(b), cmp);

            point_t xpoints[2];
            auto iType = segment_segment_intersection(get_point1(a1), get_point1(a), get_point2(b1), get_point2(b), xpoints, cmp);
            if (iType == e_crossing || iType == e_endpoint)
            {
                shouldVisit(xpoints[0]);
                ++nVisited;

                if (inflag == unknown && nVisited == 1)
                    aa = ba = 0;
                if (aHB == oriented_left)
                    inflag = pgon1_in;
                else if (bHA == oriented_left)
                    inflag = pgon2_in;
            } 
            else if (iType == e_overlapping && dot_product(A, B) < area_t {})
            {
                //++nVisited;
                shouldVisit(xpoints[0]);
                shouldVisit(xpoints[1]);
                return polygon_intersection_type::face;
            }

            if (ABOrientation == oriented_collinear && aHB == oriented_right && bHA == oriented_right)
                return polygon_intersection_type::none;
            else if (ABOrientation == oriented_collinear && aHB == oriented_collinear && bHA == oriented_collinear)
            {
                if (inflag == pgon1_in)
                    advance(b, ba, m);
                else
                    advance(a, aa, n);
            } 
            else if (ABOrientation != oriented_right)
            {
                if (bHA == oriented_left)
                {
                    if (inflag == pgon1_in)
                    {
                        ++nVisited;
                        shouldVisit(get_point1(a));
                    }
                    advance(a, aa, n);
                } 
                else
                {
                    if (inflag == pgon2_in)
                    {
                        ++nVisited;
                        shouldVisit(get_point2(b));
                    }
                    advance(b, ba, m);
                }
            }
            else
            {
                if (aHB == oriented_left)
                {
                    if (inflag == pgon2_in)
                    {
                        ++nVisited;
                        shouldVisit(get_point2(b));
                    }
                    advance(b, ba, m);
                }
                else
                {
                    if (inflag == pgon1_in)
                    {
                        ++nVisited;
                        shouldVisit(get_point1(a));
                    }
                    advance(a, aa, n);
                }
            }
        } while((aa < n || ba < m) && aa < n2 && ba < m2);

        if (inflag == unknown) 
        {
            if (visited.size() == 1) {
				return polygon_intersection_type::vertex;
            }
            //! Check if one vertex of either is inside.
            if (point_in_polygon(get_point1(0), pgon2)) 
            {
                for (auto i = 0ULL; i < m; ++i)
                    shouldVisit(get_point1(i));
                return polygon_intersection_type::overlapping;
            }
            if (point_in_polygon(get_point2(0), pgon1)) 
            {
                for (auto i = 0ULL; i < m; ++i)
                    shouldVisit(get_point2(i));
                return polygon_intersection_type::overlapping;
            }
        }
        
        return nVisited == 0 ? polygon_intersection_type::none : polygon_intersection_type::overlapping;
    }

}//! namespace geometrix;

#endif//! GEOMETRIX_CONVEX_POLYGON_POLYGON_INTERSECTION_HPP

