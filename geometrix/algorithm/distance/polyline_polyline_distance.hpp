//
//! Copyright © 2018
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_ALGORITHM_POLYLINE_POLYLINE_DISTANCE_HPP
#define GEOMETRIX_ALGORITHM_POLYLINE_POLYLINE_DISTANCE_HPP
#pragma once

#include <geometrix/algorithm/distance/point_segment_distance.hpp>
#include <geometrix/algorithm/distance/aabb_aabb_distance.hpp>
#include <geometrix/primitive/point_sequence_utilities.hpp>
#include <queue>

namespace geometrix {

    namespace result_of {
        template <typename Polyline1, typename Polyline2>
        struct polyline_polyline_distance_sqrd
        {
            using point_t = typename point_sequence_traits<Polyline1>::point_type;
        private:
            using length_t = typename geometric_traits<point_t>::arithmetic_type;
        public:
            using type = decltype(std::declval<length_t>() * std::declval<length_t>());
        };

		template <typename Polyline1, typename Polyline2>
        struct polyline_polyline_distance
        {
        private:
            using point_t = typename point_sequence_traits<Polyline1>::point_type;
		public:
            using type = typename geometric_traits<point_t>::arithmetic_type;
        };
    }
   
    template <typename Point, typename Polyline, typename NumberComparisonPolicy>
    inline typename result_of::segment_polyline_distance_sqrd<Point, Point, Polyline>::type segment_subpolyline_distance_sqrd(const Point& p1, const Point& p2, const std::tuple<std::size_t, std::size_t>& subRange, const Polyline& poly, const NumberComparisonPolicy& cmp)
    {
        using access = point_sequence_traits<Polyline>;
        using point_t = typename geometric_traits<Polyline>::point_type;
        using distance_sqrd_type = typename result_of::segment_polyline_distance_sqrd<Point, Point, Polyline>::type;
        auto distance = std::numeric_limits<distance_sqrd_type>::infinity();
        auto size = access::size(poly);
        for (std::size_t i = std::get<0>(subRange), j = std::get<0>(subRange) + 1; j <= std::get<1>(subRange); ++i, ++j)
            distance = (std::min)(distance, segment_segment_distance_sqrd(p1, p2, access::get_point(poly, i), access::get_point(poly, j), cmp));

        return distance;
    }

	template <typename Polyline1, typename Polyline2, typename NumberComparisonPolicy>
	inline typename result_of::polyline_polyline_distance_sqrd<Polyline1, Polyline2>::type polyline_polyline_distance_sqrd_brute(const Polyline1& p1, const Polyline2& p2, const NumberComparisonPolicy& cmp)
	{
		using access1 = point_sequence_traits<Polyline1>;
		using point_t = typename point_sequence_traits<Polyline1>::point_type;
		using length_t = typename geometric_traits<point_t>::arithmetic_type;
		using area_t = decltype(std::declval<length_t>() * std::declval<length_t>());
		area_t minDist2 = constants::infinity<area_t>();

		for (std::size_t i = 0, j = 1; j < access1::size(p1); ++i, ++j) 
			minDist2 = (std::min)(minDist2, segment_polyline_distance_sqrd(access1::get_point(p1, i), access1::get_point(p1, j), p2, cmp));

		return minDist2;
	}

	template <typename Polyline1, typename Polyline2, typename NumberComparisonPolicy>
	inline typename result_of::polyline_polyline_distance_sqrd<Polyline1, Polyline2>::type subpolyline_subpolyline_distance_sqrd_brute(std::tuple<std::size_t, std::size_t> const& subRange1, const Polyline1& p1, std::tuple<std::size_t, std::size_t> const& subRange2, const Polyline2& p2, const NumberComparisonPolicy& cmp)
	{
		using access1 = point_sequence_traits<Polyline1>;
		using point_t = typename point_sequence_traits<Polyline1>::point_type;
		using length_t = typename geometric_traits<point_t>::arithmetic_type;
		using area_t = decltype(std::declval<length_t>() * std::declval<length_t>());
		area_t minDist2 = constants::infinity<area_t>();

        for (std::size_t i = std::get<0>(subRange1), j = std::get<0>(subRange1) + 1; j <= std::get<1>(subRange1); ++i, ++j)
			minDist2 = (std::min)(minDist2, segment_subpolyline_distance_sqrd(access1::get_point(p1, i), access1::get_point(p1, j), subRange2, p2, cmp));

		return minDist2;
	}

    template <typename Polyline1, typename Polyline2, typename NumberComparisonPolicy>
	inline typename result_of::polyline_polyline_distance_sqrd<Polyline1, Polyline2>::type polyline_polyline_distance_sqrd(const Polyline1& p1,  const Polyline2& p2, const NumberComparisonPolicy& cmp)
	{
		using access1 = point_sequence_traits<Polyline1>;
		using access2 = point_sequence_traits<Polyline2>;
        if(access1::size(p1) < 6 || access2::size(p2) < 6)
			return polyline_polyline_distance_sqrd_brute(p1, p2, cmp);

		using point_t = typename point_sequence_traits<Polyline1>::point_type;
		using length_t = typename geometric_traits<point_t>::arithmetic_type;
		using area_t = decltype(std::declval<length_t>() * std::declval<length_t>());
		area_t minDist2 = point_point_distance_sqrd(access1::get_point(p1, 0), access2::get_point(p2, 0));

		using index_range = std::tuple<std::size_t, std::size_t>;
		using work_item = std::tuple<index_range, index_range, area_t>;

		auto is_seg = [](const index_range& r)
		{
			return std::get<1>(r) - std::get<0>(r) == 1;
		};
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
				minDist2 = (std::min)(minDist2, subpolyline_subpolyline_distance_sqrd_brute(r1, p1, r2, p2, cmp));
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

    template <typename Polyline1, typename Polyline2, typename NumberComparisonPolicy>
	inline typename result_of::polyline_polyline_distance<Polyline1, Polyline2>::type polyline_polyline_distance(const Polyline1& p1, const Polyline2& p2, const NumberComparisonPolicy& cmp)
	{
		using std::sqrt;
		return sqrt(polyline_polyline_distance_sqrd(p1, p2, cmp));
	}

}//namespace geometrix;

#endif //GEOMETRIX_ALGORITHM_POLYLINE_POLYLINE_DISTANCE_HPP

