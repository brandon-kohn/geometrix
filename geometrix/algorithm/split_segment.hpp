//
//! Copyright © 2008-2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_SPLIT_SEGMENT_HPP
#define GEOMETRIX_SPLIT_SEGMENT_HPP
#pragma once

#include <geometrix/primitive/segment_traits.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/algebra/algebra.hpp>
#include <vector>

namespace geometrix {

	namespace split_segment_detail
	{
		template <typename Point1, typename Point2, typename Segment, typename Area>
		inline void split_segment(const Point1& start, const Point2& end, const Area& maxSize, std::vector<Segment>& segs)
		{
			using length_t = typename geometric_traits<Point1>::arithmetic_type;
			if (point_point_distance_sqrd(start, end) > maxSize)
			{
				point<length_t, dimension_of<Point1>::value> mid = start + 0.5 * (end - start);
				split_segment(start, mid, maxSize, segs);
				split_segment(mid, end, maxSize, segs);
			}
			else
			{
				segs.push_back(construct<Segment>(start, end));
			}
		}
	}

    template <typename Segment, typename ArithmeticType>
    inline std::vector<Segment> split_segment( const Segment& seg, ArithmeticType maxSize )
    {
		std::vector<Segment> segs;
		split_segment_detail::split_segment(get_start(seg), get_end(seg), maxSize * maxSize, segs);
		return std::move(segs);
    }

}//namespace geometrix;

#endif //! GEOMETRIX_SPLIT_SEGMENT_HPP
