//
//! Copyright © 2008-2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_ALGORITHM_POINTSEQUENCE_IS_CONGRUENT_HPP
#define GEOMETRIX_ALGORITHM_POINTSEQUENCE_IS_CONGRUENT_HPP
#pragma once

#include <geometrix/primitive/point_sequence_traits.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/algebra/algebra.hpp>
#include <geometrix/algorithm/euclidean_distance.hpp>

#include <algorithm>

namespace geometrix {

	//! Test is two polylines are congruent.
    template <typename Polyline1, typename Polyline2, typename NumberComparisonPolicy>
    inline bool are_polylines_congruent( const Polyline1& poly1, const Polyline2& poly2, const NumberComparisonPolicy& cmp)
    {
        typedef point_sequence_traits<Polyline1> access1;
		typedef point_sequence_traits<Polyline2> access2;
		typedef typename point_sequence_traits<Polyline1>::point_type point_type1;		
		typedef typename point_sequence_traits<Polyline2>::point_type point_type2;
		typedef typename geometric_traits<point_type1>::arithmetic_type arithmetic_type1;
		typedef typename geometric_traits<point_type2>::arithmetic_type arithmetic_type2;
		typedef typename select_arithmetic_type_from_2<arithmetic_type1, arithmetic_type2>::type arithmetic_type;
		typedef vector<arithmetic_type, 2> vector_type;

		BOOST_CONCEPT_ASSERT((Point2DConcept<point_type1>));
		BOOST_CONCEPT_ASSERT((Point2DConcept<point_type2>));

		std::size_t size1 = access1::size(poly1);
		std::size_t size2 = access2::size(poly2);
		if (size1 != size2)
			return false;

		for( std::size_t i = 0, j=1; j < size1; i = j++ ){
			auto p1i = access1::get_point(poly1, i);
			auto p1j = access1::get_point(poly1, j);
			auto p2i = access2::get_point(poly2, i);
			auto p2j = access2::get_point(poly2, j);
			auto length1 = point_point_distance(p1i, p1j);
			auto length2 = point_point_distance(p2i, p2j);
			if (!cmp.equals(length1, length2))
				return false;

			if( i != 0 )
			{
				std::size_t h = i - 1;
				auto p1h = access1::get_point(poly1, h);
				auto p2h = access2::get_point(poly2, h);

				vector_type v1ih = p1h - p1i;
				vector_type v2ih = p2h - p2i;
				vector_type v1ij = p1j - p1i;
				vector_type v2ij = p2j - p2i;

				if (!cmp.equals(dot_product(v1ih, v1ij), dot_product(v2ih, v2ij)) || !cmp.equals(exterior_product_area(v1ih, v1ij), exterior_product_area(v2ih, v2ij)))
					return false;
			}
		}

		return true;
    }

	//! Test is two polygons are congruent. Does not search for different starting points in the loop cycle. 
	template <typename Polygon1, typename Polygon2, typename NumberComparisonPolicy>
	inline bool are_polygons_congruent(const Polygon1& poly1, const Polygon2& poly2, const NumberComparisonPolicy& cmp)
	{
		typedef point_sequence_traits<Polygon1> access1;
		typedef point_sequence_traits<Polygon2> access2;
		typedef typename point_sequence_traits<Polygon1>::point_type point_type1;
		typedef typename point_sequence_traits<Polygon2>::point_type point_type2;
		typedef typename geometric_traits<point_type1>::arithmetic_type arithmetic_type1;
		typedef typename geometric_traits<point_type2>::arithmetic_type arithmetic_type2;
		typedef typename select_arithmetic_type_from_2<arithmetic_type1, arithmetic_type2>::type arithmetic_type;
		typedef vector<arithmetic_type, 2> vector_type;

		BOOST_CONCEPT_ASSERT((Point2DConcept<point_type1>));
		BOOST_CONCEPT_ASSERT((Point2DConcept<point_type2>));

		std::size_t size1 = access1::size(poly1);
		std::size_t size2 = access2::size(poly2);
		if (size1 != size2)
			return false;

		for (std::size_t i = 0, j = 1; i < size1; i++, j = (j+1)%size1) {
			auto p1i = access1::get_point(poly1, i);
			auto p1j = access1::get_point(poly1, j);
			auto p2i = access2::get_point(poly2, i);
			auto p2j = access2::get_point(poly2, j);
			auto length1 = point_point_distance(p1i, p1j);
			auto length2 = point_point_distance(p2i, p2j);
			if (!cmp.equals(length1, length2))
				return false;

			if (i != 0)
			{
				std::size_t h = i - 1;
				auto p1h = access1::get_point(poly1, h);
				auto p2h = access2::get_point(poly2, h);

				vector_type v1ih = p1h - p1i;
				vector_type v2ih = p2h - p2i;
				vector_type v1ij = p1j - p1i;
				vector_type v2ij = p2j - p2i;

				if (!cmp.equals(dot_product(v1ih, v1ij), dot_product(v2ih, v2ij)) || !cmp.equals(exterior_product_area(v1ih, v1ij), exterior_product_area(v2ih, v2ij)))
					return false;
			}
		}

		return true;
	}

}//namespace geometrix;

#endif //! GEOMETRIX_ALGORITHM_POINTSEQUENCE_IS_CONGRUENT_HPP
