//
//! Copyright © 2018
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_ALGORITHM_POLYLINE_POLYLINE_CLOSEST_POINT_HPP
#define GEOMETRIX_ALGORITHM_POLYLINE_POLYLINE_CLOSEST_POINT_HPP
#pragma once

#include <geometrix/algorithm/distance/segment_polyline_closest_point.hpp>
#include <geometrix/algorithm/distance/point_point_distance.hpp>
#include <geometrix/algorithm/distance/aabb_aabb_distance.hpp>
#include <geometrix/primitive/point_sequence_utilities.hpp>

namespace geometrix {
    namespace result_of {
        template <typename Polyline1, typename Polyline2>
        struct polyline_polyline_closest_point
        {
		private:
            using point1_t = typename geometric_traits<Polyline1>::point_type;
            using point2_t = typename geometric_traits<Polyline2>::point_type;
		public:

			using type = typename result_of::point_point_distance_sqrd<point1_t, point2_t>::type;
        };
    }

    template <typename Polyline1, typename Polyline2, typename Dimensionless, typename Point, typename NumberComparisonPolicy>
    inline typename result_of::polyline_polyline_closest_point<Polyline1, Polyline2>::type polyline_polyline_closest_point_brute(const Polyline1& p1, const Polyline2& p2, std::size_t& i1, std::size_t& i2, Dimensionless& s, Dimensionless& t, Point& c1, Point&c2, const NumberComparisonPolicy& cmp)
    {
        using access1 = point_sequence_traits<Polyline1>;
        using access2 = point_sequence_traits<Polyline2>;
        using point_t = typename point_sequence_traits<Polyline1>::point_type;
        using length_t = typename geometric_traits<point_t>::arithmetic_type;
        using area_t = decltype(std::declval<length_t>() * std::declval<length_t>());
        area_t minDist2 = constants::infinity<area_t>();

        for (std::size_t i = 0, j = 1; j < access1::size(p1); ++i, ++j)
        {
            auto sl = Dimensionless{};
            auto tl = Dimensionless{};
            Point c1l, c2l;
			std::size_t i2l;
            auto d2 = segment_polyline_closest_point(access1::get_point(p1, i), access1::get_point(p1, j), p2, i2l, sl, tl, c1l, c2l, cmp);
            if(d2 < minDist2)
            {
                minDist2 = d2;
				i1 = i;
				i2 = i2l;
                s = sl;
                t = tl;
                c1 = c1l;
                c2 = c2l;
            }
        }

        return minDist2;
    }

    template <typename Polyline1, typename Polyline2, typename Dimensionless, typename Point, typename NumberComparisonPolicy>
    inline typename result_of::polyline_polyline_closest_point<Polyline1, Polyline2>::type polyline_polyline_closest_point_brute(std::tuple<std::size_t, std::size_t> const& subRange1, const Polyline1& p1, std::tuple<std::size_t, std::size_t> const& subRange2, const Polyline2& p2, std::size_t& i1, std::size_t& i2, Dimensionless& s, Dimensionless& t, Point& c1, Point&c2, const NumberComparisonPolicy& cmp)
    {
        using access1 = point_sequence_traits<Polyline1>;
        using point_t = typename point_sequence_traits<Polyline1>::point_type;
        using length_t = typename geometric_traits<point_t>::arithmetic_type;
        using area_t = decltype(std::declval<length_t>() * std::declval<length_t>());
        area_t minDist2 = constants::infinity<area_t>();

        for (std::size_t i = std::get<0>(subRange1), j = std::get<0>(subRange1) + 1; j <= std::get<1>(subRange1); ++i, ++j)
        {
            auto sl = Dimensionless{};
            auto tl = Dimensionless{};
            Point c1l, c2l;
			std::size_t i2l;
            auto d2 = segment_polyline_closest_point(access1::get_point(p1, i), access1::get_point(p1, j), subRange2, p2, i2l, sl, tl, c1l, c2l, cmp);
            if(d2 < minDist2)
            {
                minDist2 = d2;
				i1 = i;
				i2 = i2l;
                s = sl;
                t = tl;
                c1 = c1l;
                c2 = c2l;
            }
        }

        return minDist2;
    }

    template <typename Polyline1, typename Polyline2, typename Dimensionless, typename Point, typename NumberComparisonPolicy>
    inline typename result_of::polyline_polyline_closest_point<Polyline1, Polyline2>::type polyline_polyline_closest_point(const Polyline1& p1,  const Polyline2& p2, std::size_t& i1, std::size_t& i2, Dimensionless& s, Dimensionless& t, Point& c1, Point& c2, const NumberComparisonPolicy& cmp)
    {
        using access1 = point_sequence_traits<Polyline1>;
        using access2 = point_sequence_traits<Polyline2>;
        if(access1::size(p1) < 6 || access2::size(p2) < 6)
            return polyline_polyline_closest_point_brute(p1, p2, i1, i2, s, t, c1, c2, cmp);

        using point_t = typename point_sequence_traits<Polyline1>::point_type;
        using length_t = typename arithmetic_type_of<point_t>::type;
        using area_t = decltype(std::declval<length_t>() * std::declval<length_t>());
        area_t minDist2 = point_point_distance_sqrd(access1::get_point(p1, 0), access2::get_point(p2, 0));

        using index_range = std::tuple<std::size_t, std::size_t>;
        using work_item = std::tuple<index_range, index_range, area_t>;

        auto split = [](const index_range& r) -> std::tuple<index_range, index_range>
        {
            auto m = (std::get<1>(r) + std::get<0>(r)) / 2;
            return std::tuple<index_range, index_range>{ index_range{std::get<0>(r), m}, index_range{m, std::get<1>(r)} };
        };

        struct priority_cmp
        {
            bool operator()(const work_item& lhs, const work_item& rhs){ return std::get<2>(lhs) > std::get<2>(rhs); }
        };

        auto Q = std::priority_queue<work_item, std::vector<work_item>, priority_cmp>{};
        Q.emplace( index_range{ 0, access1::size(p1)-1 }, index_range{ 0, access2::size(p2)-1 }, aabb_aabb_distance_sqrd(make_aabb<point_t>(p1, cmp), make_aabb<point_t>(p2, cmp)) );

        std::size_t rl1, rh1, rl2, rh2;
        area_t d2;
        index_range r1, r2;
        while(!Q.empty())
        {
            std::tie(r1, r2, d2) = Q.top();
            Q.pop();

            if(d2 >= minDist2)
                break;

            std::tie(rl1, rh1) = r1;
            std::tie(rl2, rh2) = r2;

            if ((rh1 - rl1) < 5 || (rh2 -rl2) < 5)
            {
                auto sl = Dimensionless{};
                auto tl = Dimensionless{};
                Point c1l, c2l;
				std::size_t i1l, i2l;
                auto dl = polyline_polyline_closest_point_brute(r1, p1, r2, p2, i1l, i2l, sl, tl, c1l, c2l, cmp);
                if(dl < minDist2)
                {
                    minDist2 = dl;
					i1 = i1l;
					i2 = i2l;
                    s = sl;
                    t = tl;
                    c1 = c1l;
                    c2 = c2l;
                }
                continue;
            }

            //! Divide and conquer.
            index_range r1A, r1B, r2A, r2B;
            std::tie(r1A, r1B) = split(r1);
            std::tie(r2A, r2B) = split(r2);
            auto r1Abox = make_aabb<point_t>(p1, r1A, cmp);
            auto r1Bbox = make_aabb<point_t>(p1, r1B, cmp);
            auto r2Abox = make_aabb<point_t>(p2, r2A, cmp);
            auto r2Bbox = make_aabb<point_t>(p2, r2B, cmp);

            d2 = aabb_aabb_distance_sqrd(r1Abox, r2Abox);
            if( d2 <= minDist2 )
                Q.emplace(r1A, r2A, d2);

            d2 = aabb_aabb_distance_sqrd(r1Abox, r2Bbox);
            if( d2 <= minDist2 )
                Q.emplace(r1A, r2B, d2);

            d2 = aabb_aabb_distance_sqrd(r1Bbox, r2Abox);
            if( d2 <= minDist2 )
                Q.emplace(r1B, r2A, d2);

            d2 = aabb_aabb_distance_sqrd(r1Bbox, r2Bbox);
            if( d2 <= minDist2 )
                Q.emplace(r1B, r2B, d2);
        }

        return minDist2;
    }

}//namespace geometrix;

#endif //GEOMETRIX_ALGORITHM_POLYLINE_POLYLINE_CLOSEST_POINT_HPP

