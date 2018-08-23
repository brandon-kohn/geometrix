//
//! Copyright © 2018
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_ALGORITHM_AABB_AABB_DISTANCE_HPP
#define GEOMETRIX_ALGORITHM_AABB_AABB_DISTANCE_HPP
#pragma once

#include <geometrix/algorithm/bounding_box_intersection.hpp>
#include <geometrix/arithmetic/arithmetic.hpp>
#include <geometrix/algorithm/euclidean_distance.hpp>

namespace geometrix { 
    namespace result_of {
		template <typename NumericSequence>
        class aabb_aabb_distance_sqrd
        {
            using length_t = typename arithmetic_type_of<NumericSequence>::type;
        public:

            using type = decltype(std::declval<length_t>() * std::declval<length_t>()); 
        };

		template <typename NumericSequence>
        class aabb_aabb_distance
        {
        public:
        
            using type = typename arithmetic_type_of<NumericSequence>::type;

        };
    }//! namespace result_of

	namespace detail{
		template <typename NumericSequence>
		inline typename result_of::aabb_aabb_distance_sqrd<NumericSequence>::type aabb_aabb_distance_sqrd( const axis_aligned_bounding_box<NumericSequence>& aabb1, const axis_aligned_bounding_box<NumericSequence>& aabb2, dimension<2> )
		{
			using result_type = typename result_of::aabb_aabb_distance_sqrd<NumericSequence>::type;

			auto sqDist = construct<result_type>(0);
            auto& lb1 = aabb1.get_lower_bound();
            auto& ub1 = aabb1.get_upper_bound();
            auto& lb2 = aabb2.get_lower_bound();
            auto& ub2 = aabb2.get_upper_bound();

            //! x
            {
                if(get<0>(ub2) < get<0>(lb1))
                    sqDist += square(get<0>(ub2) - get<0>(lb1));
                else if(get<0>(lb2) > get<0>(ub1))
                    sqDist += square(get<0>(lb2) - get<0>(ub1));
            }
			
            //! y
            {
                if(get<1>(ub2) < get<1>(lb1))
                    sqDist += square(get<1>(ub2) - get<1>(lb1));
                else if(get<1>(lb2) > get<1>(ub1))
                    sqDist += square(get<1>(lb2) - get<1>(ub1));
            }

            return sqDist;
		}

		template <typename Point, typename NumericSequence>
		inline typename result_of::aabb_aabb_distance_sqrd<NumericSequence>::type aabb_aabb_distance_sqrd( const axis_aligned_bounding_box<NumericSequence>& aabb1, const axis_aligned_bounding_box<NumericSequence>& aabb2, dimension<2> )
		{
			using result_type = typename result_of::aabb_aabb_distance_sqrd<NumericSequence>::type;

			auto sqDist = construct<result_type>(0);
            auto& lb1 = aabb1.get_lower_bound();
            auto& ub1 = aabb1.get_upper_bound();
            auto& lb2 = aabb2.get_lower_bound();
            auto& ub2 = aabb2.get_upper_bound();

            //! x
            {
                if(get<0>(ub2) < get<0>(lb1))
                    sqDist += square(get<0>(ub2) - get<0>(lb1));
                else if(get<0>(lb2) > get<0>(ub1))
                    sqDist += square(get<0>(lb2) - get<0>(ub1));
            }
			
            //! y
            {
                if(get<1>(ub2) < get<1>(lb1))
                    sqDist += square(get<1>(ub2) - get<1>(lb1));
                else if(get<1>(lb2) > get<1>(ub1))
                    sqDist += square(get<1>(lb2) - get<1>(ub1));
            }
			
            //! z
            {
                if(get<2>(ub2) < get<2>(lb1))
                    sqDist += square(get<2>(ub2) - get<1>(lb1));
                else if(get<2>(lb2) > get<2>(ub1))
                    sqDist += square(get<2>(lb2) - get<2>(ub1));
            }
            return sqDist;
		}
    }//! namespace detail;
	
	template <typename NumericSequence>
	inline typename result_of::aabb_aabb_distance_sqrd<NumericSequence>::type aabb_aabb_distance_sqrd( const axis_aligned_bounding_box<NumericSequence>& aabb1, const axis_aligned_bounding_box<NumericSequence>& aabb2 )
	{
		return detail::aabb_aabb_distance_sqrd( aabb1, aabb2, typename dimension_of<NumericSequence>::type() );
	}

	template <typename NumericSequence>
	inline typename result_of::aabb_aabb_distance<NumericSequence>::type aabb_aabb_distance( const axis_aligned_bounding_box<NumericSequence>& aabb1, const axis_aligned_bounding_box<NumericSequence>& aabb2 )
	{
		using std::sqrt;
		return sqrt(detail::aabb_aabb_distance_sqrd( aabb1, aabb2, typename dimension_of<NumericSequence>::type() ));
	}
}//namespace geometrix;

#endif //GEOMETRIX_ALGORITHM_AABB_AABB_DISTANCE_HPP
