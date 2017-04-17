//
//! Copyright © 2008-2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_SEGMENT_MIDPOINT_HPP
#define GEOMETRIX_SEGMENT_MIDPOINT_HPP
#pragma once

#include <geometrix/primitive/segment_traits.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/algebra/algebra.hpp>
#include <geometrix/arithmetic/arithmetic_promotion_policy.hpp>

namespace geometrix {

	namespace result_of {
		template <typename Point1, typename Point2>
		struct segment_mid_point
		{
		private:
			using length_t = typename select_arithmetic_type_from_sequences<Point1, Point2>::type;
		public:
			using type = point<length_t, dimension_of<Point1>::value>;
		};
	}//! namespace result_of;

	template <typename Point1, typename Point2>
	inline typename result_of::segment_mid_point<Point1, Point2>::type segment_mid_point(const Point1& a, const Point2& b)
	{
		return 0.5 * (as_vector(a) + as_vector(b));
	}

	namespace result_of {
		template <typename Segment>
		struct segment_mid_point<Segment, void>
		{
		private:
			using point_t = typename geometric_traits<Segment>::point_type;
			using length_t = typename geometric_traits<point_t>::arithmetic_type;

		public:
			using type = point<length_t, dimension_of<point_t>::value>;
		};
	}//! namespace result_of;

    template <typename Segment>
    inline typename result_of::segment_mid_point<Segment, void>::type segment_mid_point( const Segment& seg )
    {
		return 0.5 * (as_vector(get_start(seg)) + as_vector(get_end(seg)));
    }

}//namespace geometrix;

#endif //! GEOMETRIX_SEGMENT_MIDPOINT_HPP
