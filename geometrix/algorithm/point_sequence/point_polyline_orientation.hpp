//
//! Copyright © 2017
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_POINT_POLYLINE_ORIENTATION_HPP
#define GEOMETRIX_POINT_POLYLINE_ORIENTATION_HPP

#include <geometrix/algorithm/orientation.hpp>
#include <geometrix/primitive/point_sequence_traits.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/arithmetic/arithmetic_promotion_policy.hpp>
#include <geometrix/algorithm/distance/point_segment_closest_point.hpp>
#include <geometrix/utility/utilities.hpp>

/////////////////////////////////////////////////////////////////////////////
//
// NAMESPACE
//
/////////////////////////////////////////////////////////////////////////////
namespace geometrix {
	
	template <typename Point, typename Polyline, typename Length, typename NumberComparisonPolicy>
	inline orientation_type point_polyline_orientation(const Point& p, const Polyline& pline, const Length& minDistance, std::size_t segIndex, const NumberComparisonPolicy& cmp)
	{
		BOOST_CONCEPT_ASSERT((PointSequenceConcept<Polyline>));
		BOOST_CONCEPT_ASSERT((NumberComparisonPolicyConcept<NumberComparisonPolicy>));

		using access = point_sequence_traits<Polyline>;
		GEOMETRIX_ASSERT(access::size(pline) > 1);

		using length_t = typename select_arithmetic_type_from_sequences<Point, typename access::point_type>::type;
		using area_t = decltype(std::declval<length_t>() * std::declval<length_t>());
		using dimensionless_t = decltype(std::declval<length_t>() / std::declval<length_t>());
		using vector_t = vector<length_t, dimension_of<Point>::value>;

		if (cmp.greater_than(minDistance, constants::zero<area_t>()))
		{
			GEOMETRIX_ASSERT(minDistance < (std::numeric_limits<area_t>::max)());
			auto pi = access::get_point(pline, segIndex);
			auto pj = access::get_point(pline, segIndex + 1);
			dimensionless_t t;
			point_segment_closest_point_and_param(p, pi, pj, t);
			if (t > constants::zero<dimensionless_t>() && t < constants::one<dimensionless_t>())
				return get_orientation(pi, pj, p, cmp);
			else if (cmp.equals(t, constants::zero<dimensionless_t>()))
			{
				//! Closest point is a vertex i.
				if (segIndex != 0)
				{
					auto ph = access::get_point(pline, segIndex - 1);
					auto o = get_orientation(ph, pi, pj, cmp);
					if (o == oriented_left)
					{
						vector_t vpi = p - pi;
						vector_t lo = pj - pi;
						vector_t hi = ph - pi;
						if (is_vector_between(lo, hi, vpi, false, cmp))
							return oriented_left;
						else
							return oriented_right;
					}
					else if (o == oriented_right)
					{
						vector_t vpi = p - pi;
						vector_t hi = pj - pi;
						vector_t lo = ph - pi;
						if (is_vector_between(lo, hi, vpi, false, cmp))
							return oriented_right;
						else
							return oriented_left;
					}
					else
						return get_orientation(pi, pj, p, cmp);
				}
				else
					return get_orientation(pi, pj, p, cmp);
			}
			else
			{
				//! pj is the closest.
				if ((segIndex + 2) < access::size(pline))
				{
					auto pk = access::get_point(pline, segIndex + 2);
					auto o = get_orientation(pi, pj, pk, cmp);
					if (o == oriented_left)
					{
						vector_t vpj = p - pj;
						vector_t lo = pk - pj;
						vector_t hi = pi - pj;
						if (is_vector_between(lo, hi, vpj, false, cmp))
							return oriented_left;
						else
							return oriented_right;
					}
					else if (o == oriented_right)
					{
						vector_t vpj = p - pj;
						vector_t hi = pk - pj;
						vector_t lo = pi - pj;
						if (is_vector_between(lo, hi, vpj, false, cmp))
							return oriented_right;
						else
							return oriented_left;
					}
					else
						return get_orientation(pi, pj, p, cmp);
				}
				else
					return get_orientation(pi, pj, p, cmp);
			}
		}

		return oriented_collinear;
	}
	
	template <typename Point, typename Polyline, typename NumberComparisonPolicy>
	inline orientation_type point_polyline_orientation(const Point& p, const Polyline& pline, const NumberComparisonPolicy& cmp)
	{
		BOOST_CONCEPT_ASSERT((PointSequenceConcept<Polyline>));
		BOOST_CONCEPT_ASSERT((NumberComparisonPolicyConcept<NumberComparisonPolicy>));

		using access = point_sequence_traits<Polyline>;
		GEOMETRIX_ASSERT(access::size(pline) > 1);

		using length_t = typename select_arithmetic_type_from_sequences<Point, typename access::point_type>::type;
		using area_t = decltype(std::declval<length_t>() * std::declval<length_t>());
		using dimensionless_t = decltype(std::declval<length_t>() / std::declval<length_t>());
		using vector_t = vector<length_t, dimension_of<Point>::value>;

		auto minDistance = (std::numeric_limits<area_t>::max)();
		auto segIndex = std::size_t{};
		for (std::size_t i = 0, j = 1; j < access::size(pline); i = j++)
		{
			auto d2 = point_segment_distance_sqrd(p, access::get_point(pline, i), access::get_point(pline, j));
			if (d2 < minDistance)
			{
				minDistance = d2;
				segIndex = i;
			}
		}

		return point_polyline_orientation(p, pline, minDistance, segIndex, cmp);
	}

}//namespace geometrix;

#endif //GEOMETRIX_POINT_POLYLINE_ORIENTATION_HPP
