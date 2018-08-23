//
//! Copyright © 2008-2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_ALGORITHM_POINT_AABB_DISTANCE_HPP
#define GEOMETRIX_ALGORITHM_POINT_AABB_DISTANCE_HPP
#pragma once

#include <geometrix/algorithm/bounding_box_intersection.hpp>
#include <geometrix/algorithm/euclidean_distance.hpp>

namespace geometrix {

    namespace result_of {

		template <typename Point, typename NumericSequence>
        class point_aabb_closest_point
        {
            using length_t = typename arithmetic_type_of<Point>::type;

            public:

            using type = point<length_t, dimension_of<Point>::value>;
        };

    }//! namespace result_of

	namespace detail{
		template <typename Point, typename NumericSequence>
		inline typename result_of::point_point_distance_sqrd<Point, NumericSequence>::type point_aabb_distance_sqrd( const Point& p, const axis_aligned_bounding_box<NumericSequence>& aabb, dimension<2> )
		{
			typedef typename result_of::point_point_distance_sqrd<Point, NumericSequence>::type result_type;

			result_type sqDist = construct<result_type>(0);
			auto x = get<0>( p );
			auto y = get<1>( p );
			
			if( x < get<0>( aabb.get_lower_bound() ) )
				sqDist += (get<0>( aabb.get_lower_bound() ) - x) * (get<0>( aabb.get_lower_bound() ) - x);
			if( x > get<0>( aabb.get_upper_bound() ) )
				sqDist += (x - get<0>( aabb.get_upper_bound() )) * (x - get<0>( aabb.get_upper_bound() ));

			if( y < get<1>( aabb.get_lower_bound() ) )
				sqDist += (get<1>( aabb.get_lower_bound() ) - y) * (get<1>( aabb.get_lower_bound() ) - y);
			if( y > get<1>( aabb.get_upper_bound() ) )
				sqDist += (y - get<1>( aabb.get_upper_bound() )) * (y - get<1>( aabb.get_upper_bound() ));

			return sqDist;
		}

		template <typename Point, typename NumericSequence>
		inline typename result_of::point_point_distance_sqrd<Point, NumericSequence>::type point_aabb_distance_sqrd( const Point& p, const axis_aligned_bounding_box<NumericSequence>& aabb, dimension<3> )
		{
			typedef typename result_of::point_point_distance_sqrd<Point, NumericSequence>::type result_type;

			result_type sqDist = construct<result_type>(0);
			auto x = get<0>( p );
			auto y = get<1>( p );
			auto z = get<2>( p );

			if( x < get<0>( aabb.get_lower_bound() ) )
				sqDist += (get<0>( aabb.get_lower_bound() ) - x) * (get<0>( aabb.get_lower_bound() ) - x);
			if( x > get<0>( aabb.get_upper_bound() ) )
				sqDist += (x - get<0>( aabb.get_upper_bound() )) * (x - get<0>( aabb.get_upper_bound() ));

			if( y < get<1>( aabb.get_lower_bound() ) )
				sqDist += (get<1>( aabb.get_lower_bound() ) - y) * (get<1>( aabb.get_lower_bound() ) - y);
			if( y > get<1>( aabb.get_upper_bound() ) )
				sqDist += (y - get<1>( aabb.get_upper_bound() )) * (y - get<1>( aabb.get_upper_bound() ));

			if( z < get<2>( aabb.get_lower_bound() ) )
				sqDist += (get<2>( aabb.get_lower_bound() ) - z) * (get<2>( aabb.get_lower_bound() ) - z);
			if( z > get<2>( aabb.get_upper_bound() ) )
				sqDist += (z - get<2>( aabb.get_upper_bound() )) * (z - get<2>( aabb.get_upper_bound() ));

			return sqDist;
		}

        template <typename Point, typename NumericSequence>
		inline typename result_of::point_aabb_closest_point<Point, NumericSequence>::type point_aabb_closest_point( const Point& p, const axis_aligned_bounding_box<NumericSequence>& aabb, dimension<2> )
		{
			using point_t = typename result_of::point_aabb_closest_point<Point, NumericSequence>::type;

			auto x = get<0>( p );
			auto y = get<1>( p );
			
			if( x < get<0>( aabb.get_lower_bound() ) )
				x = get<0>( aabb.get_lower_bound() );
			if( x > get<0>( aabb.get_upper_bound() ) )
				x = get<0>( aabb.get_upper_bound() );

			if( y < get<1>( aabb.get_lower_bound() ) )
				y = get<1>( aabb.get_lower_bound() );
			if( y > get<1>( aabb.get_upper_bound() ) )
				y = get<1>( aabb.get_upper_bound() );

			return point_t{ x, y };
		}

        template <typename Point, typename NumericSequence>
		inline typename result_of::point_aabb_closest_point<Point, NumericSequence>::type point_aabb_closest_point( const Point& p, const axis_aligned_bounding_box<NumericSequence>& aabb, dimension<3> )
		{
			using point_t = typename result_of::point_aabb_closest_point<Point, NumericSequence>::type;

			auto x = get<0>( p );
			auto y = get<1>( p );
			auto z = get<2>( p );
			
			if( x < get<0>( aabb.get_lower_bound() ) )
				x = get<0>( aabb.get_lower_bound() );
			if( x > get<0>( aabb.get_upper_bound() ) )
				x = get<0>( aabb.get_upper_bound() );

			if( y < get<1>( aabb.get_lower_bound() ) )
				y = get<1>( aabb.get_lower_bound() );
			if( y > get<1>( aabb.get_upper_bound() ) )
				y = get<1>( aabb.get_upper_bound() );

			if( z < get<2>( aabb.get_lower_bound() ) )
				z = get<2>( aabb.get_lower_bound() );
			if( z > get<2>( aabb.get_upper_bound() ) )
				z = get<2>( aabb.get_upper_bound() );

			return point_t{ x, y, z };
		}
	}
	
	template <typename Point, typename NumericSequence>
	inline typename result_of::point_point_distance_sqrd<Point, NumericSequence>::type point_aabb_distance_sqrd( const Point& p, const axis_aligned_bounding_box<NumericSequence>& aabb )
	{
		static_assert(dimension_of<Point>::value == dimension_of<NumericSequence>::value, "Calls to point_aabb_distance_sqrd must have parameters with the same dimensionality.");
		return detail::point_aabb_distance_sqrd( p, aabb, typename dimension_of<Point>::type() );
	}

	template <typename Point, typename NumericSequence>
	inline typename result_of::point_point_distance<Point, NumericSequence>::type point_aabb_distance( const Point& p, const axis_aligned_bounding_box<NumericSequence>& aabb )
	{
		using std::sqrt;
		static_assert(dimension_of<Point>::value == dimension_of<NumericSequence>::value, "Calls to point_aabb_distance_sqrd must have parameters with the same dimensionality.");
		return sqrt(detail::point_aabb_distance_sqrd( p, aabb, typename dimension_of<Point>::type() ));
	}

    template <typename Point, typename Point2>
    inline typename result_of::point_aabb_closest_point<Point, Point2>::type point_aabb_closest_point(const Point& p, const axis_aligned_bounding_box<Point2>& aabb)
    {
		static_assert(dimension_of<Point>::value == dimension_of<Point2>::value, "Calls to point_aabb_closest_point must have parameters with the same dimensionality.");
		return detail::point_aabb_closest_point( p, aabb, typename dimension_of<Point>::type() );
    }

}//namespace geometrix;

#endif //GEOMETRIX_ALGORITHM_POINT_AABB_DISTANCE_HPP
