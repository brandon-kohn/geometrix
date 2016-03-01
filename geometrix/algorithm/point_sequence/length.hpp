//
//! Copyright © 2008-2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_ALGORITHM_POINTSEQUENCE_LENGTH_HPP
#define GEOMETRIX_ALGORITHM_POINTSEQUENCE_LENGTH_HPP
#pragma once

#include <geometrix/primitive/point_sequence_traits.hpp>
#include <geometrix/utility/utilities.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/algorithm/euclidean_distance.hpp>

#include <algorithm>

namespace geometrix {

    template <typename Polyline>
    inline typename geometric_traits<typename point_sequence_traits<Polyline>::point_type>::arithmetic_type polyline_length( const Polyline& poly )
    {
        typedef point_sequence_traits<Polyline> access;
		typedef typename point_sequence_traits<Polyline>::point_type point_type;
		typedef typename geometric_traits<point_type>::arithmetic_type arithmetic_type;
		arithmetic_type length = 0;
        std::size_t size = access::size( poly );
		for( std::size_t i = 0, j=1; j < size; i = j++ ){
			length += point_point_distance(access::get_point(poly, i), access::get_point(poly, j));
		}

		return length;
    }

	template <typename Polygon>
	inline typename geometric_traits<typename point_sequence_traits<Polygon>::point_type>::arithmetic_type polygon_length(const Polygon& poly)
	{
		typedef point_sequence_traits<Polygon> access;
		typedef typename point_sequence_traits<Polygon>::point_type point_type;
		typedef typename geometric_traits<point_type>::arithmetic_type arithmetic_type;
		arithmetic_type length = 0;
		std::size_t size = access::size(poly);
		for (std::size_t i = 0, j = 1; i < size; ++i, j = (j+1)%size) {
			length += point_point_distance(access::get_point(poly, i), access::get_point(poly, j));
		}

		return length;
	}

}//namespace geometrix;

#endif //! GEOMETRIX_ALGORITHM_POINTSEQUENCE_LENGTH_HPP
