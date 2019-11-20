//
//! Copyright © 2018
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_ALGORITHM_POLYGON_POLYGON_DISTANCE_HPP
#define GEOMETRIX_ALGORITHM_POLYGON_POLYGON_DISTANCE_HPP
#pragma once

#include <geometrix/algorithm/distance/point_segment_distance.hpp>
#include <geometrix/algorithm/distance/aabb_aabb_distance.hpp>
#include <geometrix/primitive/point_sequence_utilities.hpp>
#include <queue>

namespace geometrix {

    namespace result_of {
        template <typename Polygon1, typename Polygon2>
        struct polygon_polygon_distance_sqrd
        {
            using point_t = typename point_sequence_traits<Polygon1>::point_type;
        private:
            using length_t = typename geometric_traits<point_t>::arithmetic_type;
        public:
            using type = decltype(std::declval<length_t>() * std::declval<length_t>());
        };

		template <typename Polygon1, typename Polygon2>
        struct polygon_polygon_distance
        {
        private:
            using point_t = typename point_sequence_traits<Polygon1>::point_type;
		public:
            using type = typename geometric_traits<point_t>::arithmetic_type;
        };
    }
   
    template <typename Point, typename Polygon, typename NumberComparisonPolicy>
    inline typename result_of::segment_polygon_distance_sqrd<Point, Point, Polygon>::type segment_subpolygon_distance_sqrd(const Point& p1, const Point& p2, const std::tuple<std::size_t, std::size_t>& subRange, const Polygon& poly, const NumberComparisonPolicy& cmp)
    {
        using access = point_sequence_traits<Polygon>;
		using dist_type = typename result_of::segment_polygon_closest_point<Point>::type;
        std::size_t i0 = std::get<0>(subRange), i1 = std::get<1>(subRange);
        GEOMETRIX_ASSERT(i0 < i1 || i0 == access::size(poly)-1);//! this doesn't support wrapping around except for the segment just before 0.
        auto distance = std::numeric_limits<dist_type>::infinity();
        for (std::size_t i = i0, j = i0 + 1; j <= i1; i = j++)
            distance = (std::min)(distance, segment_segment_distance_sqrd(p1, p2, access::get_point(poly, i), access::get_point(poly, j), cmp));

        return distance;
    }

	template <typename Polygon1, typename Polygon2, typename NumberComparisonPolicy>
	inline typename result_of::polygon_polygon_distance_sqrd<Polygon1, Polygon2>::type polygon_polygon_distance_sqrd_brute(const Polygon1& p1, const Polygon2& p2, const NumberComparisonPolicy& cmp)
	{
		using access1 = point_sequence_traits<Polygon1>;
		using point_t = typename point_sequence_traits<Polygon1>::point_type;
		using length_t = typename geometric_traits<point_t>::arithmetic_type;
		using area_t = decltype(std::declval<length_t>() * std::declval<length_t>());
		area_t minDist2 = constants::infinity<area_t>();

        auto size = access1::size(p1);
		for (std::size_t i = size - 1, j = 0; j < size; i = j++) 
            minDist2 = (std::min)(minDist2, segment_polygon_distance_sqrd(access1::get_point(p1, i), access1::get_point(p1, j), p2, cmp));

		return minDist2;
	}

	template <typename Polygon1, typename Polygon2, typename NumberComparisonPolicy>
	inline typename result_of::polygon_polygon_distance_sqrd<Polygon1, Polygon2>::type subpolygon_subpolygon_distance_sqrd_brute(std::tuple<std::size_t, std::size_t> const& subRange1, const Polygon1& p1, std::tuple<std::size_t, std::size_t> const& subRange2, const Polygon2& p2, const NumberComparisonPolicy& cmp)
	{
		using access1 = point_sequence_traits<Polygon1>;
		using point_t = typename point_sequence_traits<Polygon1>::point_type;
		using length_t = typename geometric_traits<point_t>::arithmetic_type;
		using area_t = decltype(std::declval<length_t>() * std::declval<length_t>());
		area_t minDist2 = constants::infinity<area_t>();
        std::size_t i0 = std::get<0>(subRange1), i1 = std::get<1>(subRange1);
        GEOMETRIX_ASSERT(i0 < i1 || i0 == access1::size(p1)-1);//! this doesn't support wrapping around except for the segment just before 0.
        for (std::size_t i = i0, j = i0 + 1; j <= i1; i = j++)
			minDist2 = (std::min)(minDist2, segment_subpolygon_distance_sqrd(access1::get_point(p1, i), access1::get_point(p1, j), subRange2, p2, cmp));

		return minDist2;
	}

    template <typename Polygon1, typename Polygon2, typename NumberComparisonPolicy>
	inline typename result_of::polygon_polygon_distance_sqrd<Polygon1, Polygon2>::type polygon_polygon_distance_sqrd(const Polygon1& p1,  const Polygon2& p2, const NumberComparisonPolicy& cmp)
	{
		using access1 = point_sequence_traits<Polygon1>;
		using access2 = point_sequence_traits<Polygon2>;
        if(access1::size(p1) < 6 || access2::size(p2) < 6)
			return polygon_polygon_distance_sqrd_brute(p1, p2, cmp);

		using point_t = typename point_sequence_traits<Polygon1>::point_type;
		using length_t = typename geometric_traits<point_t>::arithmetic_type;
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
				minDist2 = (std::min)(minDist2, subpolygon_subpolygon_distance_sqrd_brute(r1, p1, r2, p2, cmp));
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

    template <typename Polygon1, typename Polygon2, typename NumberComparisonPolicy>
	inline typename result_of::polygon_polygon_distance<Polygon1, Polygon2>::type polygon_polygon_distance(const Polygon1& p1, const Polygon2& p2, const NumberComparisonPolicy& cmp)
	{
		using std::sqrt;
		return sqrt(polygon_polygon_distance_sqrd(p1, p2, cmp));
	}

}//namespace geometrix;

#endif //GEOMETRIX_ALGORITHM_POLYGON_POLYGON_DISTANCE_HPP

