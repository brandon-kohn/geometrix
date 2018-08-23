//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_POINT_SEGMENT_CLOSEST_POINT_HPP
#define GEOMETRIX_POINT_SEGMENT_CLOSEST_POINT_HPP

#include <geometrix/algorithm/distance/point_point_distance.hpp>
#include <geometrix/algebra/algebra.hpp>
#include <geometrix/primitive/segment.hpp>
#include <geometrix/numeric/constants.hpp>
#include <geometrix/utility/utilities.hpp>

namespace geometrix {
    namespace result_of
	{
        template <typename Point, typename T2, typename T3 = void, typename T4 = void, typename T5 = void>
		struct point_segment_closest_point;

        template <typename Point, typename Segment>
		struct point_segment_closest_point<Point, Segment, typename geometric_traits<Segment>::is_segment>
		{
			using length_t = typename select_arithmetic_type_from_sequences<typename geometric_traits<Segment>::point_type, Point>::type;
			using type = point<length_t, dimension_of<Point>::value>;
		};

		template <typename Point, typename SegPoint1, typename SegPoint2>
		struct point_segment_closest_point<Point, SegPoint1, SegPoint2, typename geometric_traits<SegPoint1>::is_point, typename geometric_traits<SegPoint2>::is_point>
		{
			using length_t = typename select_arithmetic_type_from_sequences<SegPoint1, Point>::type;
			using type = point<length_t, dimension_of<Point>::value>;
		};

		template <typename Point, typename SegPoint1, typename SegPoint2, typename Dimensionless>
		struct point_segment_closest_point_and_param
		{
			using length_t = typename select_arithmetic_type_from_sequences<SegPoint1, Point>::type;
			using type = point<length_t, dimension_of<Point>::value>;
		};
	}

    template <typename Point, typename Segment>
    inline typename result_of::point_segment_closest_point<Point, Segment>::type point_segment_closest_point(const Point& p, const Segment& seg)
    {
        using length_t = typename select_arithmetic_type_from_sequences<typename geometric_traits<Segment>::point_type, Point>::type;
        using point_t = point<length_t, dimension_of<Point>::value>;
        using vector_t = vector<length_t, dimension_of<Point>::value>;
        vector_t AP = p - get_start(seg);
        vector_t AB = get_end(seg) - get_start(seg);
        auto t = dot_product(AP, AB);
        if (t <= construct<decltype(t)>(0))
            return construct<point_t>(get_start(seg));
        auto denom = dot_product(AB, AB);
        if (t >= denom)
            return construct<point_t>(get_end(seg));
        return construct<point_t>(get_start(seg) + (t / denom) * AB);
    }

	template <typename Point, typename SegPoint1, typename SegPoint2>
	inline typename result_of::point_segment_closest_point<Point, SegPoint1, SegPoint2>::type point_segment_closest_point(const Point& p, const SegPoint1& A, const SegPoint2& B)
	{
		using point_t = typename result_of::point_segment_closest_point<Point, SegPoint1, SegPoint2>::type;
		using length_t = typename arithmetic_type_of<point_t>::type;
		using dimensionless_t = typename dimensionless_type_of<point_t>::type;
		using vector_t = vector<length_t, dimension_of<point_t>::value>;
		vector_t AP = p - A;
		vector_t AB = B - A;
		auto l2 = dot_product(AB, AB);
		auto t = dot_product(AP, AB) / l2;
		if (t <= constants::zero<dimensionless_t>())
            return construct<point_t>(A);
	    if (t >= constants::one<dimensionless_t>())
	    	return construct<point_t>(B);
        return construct<point_t>(A + t * AB);
	}

	template <typename Point, typename SegPoint1, typename SegPoint2, typename Dimensionless>
	inline typename result_of::point_segment_closest_point_and_param<Point, SegPoint1, SegPoint2, Dimensionless>::type point_segment_closest_point_and_param(const Point& p, const SegPoint1& A, const SegPoint2& B, Dimensionless& t)
	{
		using point_t = typename result_of::point_segment_closest_point_and_param<Point, SegPoint1, SegPoint2, Dimensionless>::type;
		using length_t = typename arithmetic_type_of<point_t>::type;
		using vector_t = vector<length_t, dimension_of<point_t>::value>;
		vector_t AP = p - A;
		vector_t AB = B - A;
		auto l2 = dot_product(AB, AB);
		t = dot_product(AP, AB) / l2;
		if (t <= constants::zero<Dimensionless>())
            return construct<point_t>(A);
	    if (t >= constants::one<Dimensionless>())
	    	return construct<point_t>(B);
        return construct<point_t>(A + t * AB);
	}

}//namespace geometrix;

#endif //GEOMETRIX_POINT_SEGMENT_CLOSEST_POINT_HPP
