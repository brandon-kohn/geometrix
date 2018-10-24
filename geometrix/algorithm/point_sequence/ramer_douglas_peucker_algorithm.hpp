//
//! Copyright © 2008-2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_ALGORITHM_POINTSEQUENCE_RAMER_DOUGLAS_PEUCKER_ALGORITHM_HPP
#define GEOMETRIX_ALGORITHM_POINTSEQUENCE_RAMER_DOUGLAS_PEUCKER_ALGORITHM_HPP
#pragma once

#include <geometrix/primitive/point.hpp>
#include <geometrix/primitive/point_sequence_traits.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/primitive/line.hpp>
#include <geometrix/algorithm/distance/point_line_distance.hpp>

#include <boost/optional.hpp>
#include <boost/range/join.hpp>

namespace geometrix {

	namespace detail {
		template <typename Polyline, typename Length>
		inline void ramer_douglas_peucker_algorithm(const Polyline& poly, Polyline& nPoly, std::size_t start, std::size_t end, const Length& epsilon)
		{
			using access = point_sequence_traits<Polyline>;
			using point_t = typename access::point_type;
			using length_t = typename geometric_traits<point_t>::arithmetic_type;
			using dimensionless_t = typename geometric_traits<point_t>::dimensionless_type;
			using vector_t = vector<dimensionless_t, dimension_of<point_t>::value>;
			using line_t = line<point_t>;

			// Find the point with the maximum distance
			auto dmax = constants::zero<length_t>();
			boost::optional<std::size_t> index;
			if ((start + 2) < end)
			{
				line_t l(access::get_point(poly, start), access::get_point(poly, end - 1));
				for (std::size_t i = start + 1; (i + 1) < end; ++i)
				{
					auto d = point_line_distance(access::get_point(poly, i), l);
					if (d > dmax)
					{
						index = i;
						dmax = d;
					}
				}
			}

			// If max distance is greater than epsilon, recursively simplify
			if (dmax > epsilon)
			{
				GEOMETRIX_ASSERT(index);
				ramer_douglas_peucker_algorithm(poly, nPoly, start, *index, epsilon);
				access::pop_back(nPoly);
				ramer_douglas_peucker_algorithm(poly, nPoly, *index, end, epsilon);
			}
			else
			{
				GEOMETRIX_ASSERT((start + 1) != end);
				access::push_back(nPoly, access::get_point(poly,start));
				access::push_back(nPoly, access::get_point(poly, end-1));
			}
		}
	}//! namespace detail;

	template <typename Polyline, typename Length>
	inline Polyline ramer_douglas_peucker_algorithm(const Polyline& poly, const Length& epsilon)
	{
		using access = point_sequence_traits<Polyline>;
		Polyline nPoly;
		::geometrix::detail::ramer_douglas_peucker_algorithm(poly, nPoly, 0, access::size(poly), epsilon);
		return nPoly;
	}

}//namespace geometrix;

#endif //! GEOMETRIX_ALGORITHM_POINTSEQUENCE_RAMER_DOUGLAS_PEUCKER_ALGORITHM_HPP
