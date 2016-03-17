//
//! Copyright © 2008-2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_CLIP_POLYLINE_ENDS_HPP
#define GEOMETRIX_CLIP_POLYLINE_ENDS_HPP
#pragma once

#include <geometrix/primitive/point_sequence_traits.hpp>
#include <geometrix/utility/utilities.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/algorithm/euclidean_distance.hpp>

#include <algorithm>

namespace geometrix {

	template <typename Polyline>
	inline std::vector<typename geometric_traits<typename point_sequence_traits<Polyline>::point_type>::arithmetic_type> polyline_segment_lengths(const Polyline& poly)
	{
		typedef point_sequence_traits<Polyline> access;
		typedef typename point_sequence_traits<Polyline>::point_type point_type;
		typedef typename geometric_traits<point_type>::arithmetic_type arithmetic_type;
		arithmetic_type length = 0;
		std::size_t size = access::size(poly);
		std::vector<arithmetic_type> lengths;
		
		for (std::size_t i = 0, j = 1; j < size; i = j++) 
		{
			length += point_point_distance(access::get_point(poly, i), access::get_point(poly, j));
			lengths.push_back(length);
		}

		return std::move(lengths);
	}

	template <typename PolylineIter, typename ArithmeticType>
	inline std::vector<ArithmeticType> polyline_segment_lengths(PolylineIter first, PolylineIter last, const ArithmeticType& cutoff)
	{	
		ArithmeticType length = 0;
		std::vector<ArithmeticType> lengths;
		if (first == last)
			return std::move(lengths);

		for (PolylineIter i = first, j = std::next(first); j != last; ++i, ++j)
		{
			length += point_point_distance(*i, *j);
			lengths.push_back(length);
			if (lengths.back() > cutoff)
				break;
		}

		return std::move(lengths);
	}

	template <typename Polyline, typename ArithmeticType>
	inline std::vector<typename geometric_traits<typename point_sequence_traits<Polyline>::point_type>::arithmetic_type> polyline_segment_lengths(const Polyline& poly, const ArithmeticType& cutoff)
	{
		typedef point_sequence_traits<Polyline> access;
		typedef typename point_sequence_traits<Polyline>::point_type point_type;
		typedef typename geometric_traits<point_type>::arithmetic_type arithmetic_type;
		arithmetic_type length = 0;
		std::size_t size = access::size(poly);		
		std::vector<arithmetic_type> lengths;
		
		for (std::size_t i = 0, j = 1; j < size; i = j++)
		{
			length += point_point_distance(access::get_point(poly, i), access::get_point(poly, j));
			lengths.push_back(length);
			if (lengths.back() > cutoff)
				break;
		}

		return std::move(lengths);
	}
	
	template <typename Polyline, typename PolylineIter, typename ArithmeticType, typename NumberComparisonPolicy>
	inline Polyline clip_polyline_front(PolylineIter first, PolylineIter last, ArithmeticType l, const NumberComparisonPolicy& cmp)
	{
		typedef typename point_sequence_traits<Polyline>::point_type point_type;
		typedef vector<ArithmeticType, 2> vector_type;

		std::vector<ArithmeticType> lengths = polyline_segment_lengths(first, last, l);
		auto it = std::lower_bound(lengths.begin(), lengths.end(), l);

		Polyline newPoly;
		if (it == lengths.end())
			return std::move(newPoly);
				
		l = *it - l;

		auto index = std::distance(lengths.begin(), it);
		PolylineIter pit = std::next(first, index), pnext = std::next(first, index + 1);
		GEOMETRIX_ASSERT(pnext != last);

		if (cmp.greater_than(l, 0))
		{
			point_type newP{ *pnext + l * normalize<vector_type>(*pit - *pnext) };
			if (!numeric_sequence_equals(newP, *pnext, cmp))
				newPoly.emplace_back(newP);
		}
		else if (++pnext != last)
			newPoly.push_back(*std::prev(pnext));

		for (; pnext != last; ++pnext)
			newPoly.push_back(*pnext);

		return std::move(newPoly);
	}

	template <typename Polyline, typename ArithmeticType, typename NumberComparisonPolicy>
	inline Polyline clip_polyline_front(const Polyline& polyline, ArithmeticType l, const NumberComparisonPolicy& cmp)
	{
		typedef point_sequence_traits<Polyline> access;
		typedef typename point_sequence_traits<Polyline>::point_type point_type;
		typedef vector<ArithmeticType, 2> vector_type;
		
		std::vector<ArithmeticType> lengths = polyline_segment_lengths(polyline, l);
		auto it = std::lower_bound(lengths.begin(), lengths.end(), l);
		
		Polyline newPoly;
		if (it == lengths.end())
			return std::move(newPoly);

		auto index = std::distance(lengths.begin(), it);
		auto size = access::size(polyline);
		GEOMETRIX_ASSERT(index + 1 < size);

		l = *it - l;
		auto next = index + 1;

		if (cmp.greater_than(l, 0))
		{
			point_type newP{ access::get_point(polyline, next) + l * normalize<vector_type>(access::get_point(polyline, index) - access::get_point(polyline, next)) };
			if (!numeric_sequence_equals(newP, access::get_point(polyline, next), cmp))
				newPoly.emplace_back(newP);
		}
		else if( ++next < size )
			newPoly.push_back(access::get_point(polyline, next - 1));
		
		for (; next < size; ++next)
			newPoly.push_back(access::get_point(polyline, next));

		return std::move(newPoly);
	}
	
	template <typename Polyline, typename ArithmeticType, typename NumberComparisonPolicy>
	inline Polyline clip_polyline_back(const Polyline& polyline, ArithmeticType l, const NumberComparisonPolicy& cmp)
	{
		typedef point_sequence_traits<Polyline> access;
		typedef typename point_sequence_traits<Polyline>::point_type point_type;
		typedef vector<ArithmeticType, 2> vector_type;
		auto poly = clip_polyline_front<Polyline>(access::rbegin(polyline), access::rend(polyline), l, cmp);
		std::reverse(access::begin(poly), access::end(poly));
		return std::move(poly);
	}

    template <typename Polyline, typename ArithmeticType, typename NumberComparisonPolicy>
    inline Polyline clip_polyline_ends( const Polyline& poly, const ArithmeticType& distanceFromEndpointToClip, const NumberComparisonPolicy& cmp)
    {
		return clip_polyline_back(clip_polyline_front(poly, distanceFromEndpointToClip, cmp), distanceFromEndpointToClip, cmp);
    }

}//namespace geometrix;

#endif //! GEOMETRIX_CLIP_POLYLINE_ENDS_HPP
