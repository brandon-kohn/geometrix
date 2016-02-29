//
//! Copyright © 2008-2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_POLYLINE_MIDPOINT_HPP
#define GEOMETRIX_POLYLINE_MIDPOINT_HPP
#pragma once

#include <geometrix/primitive/point_sequence_traits.hpp>
#include <geometrix/utility/utilities.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/algorithm/euclidean_distance.hpp>

#include <algorithm>

namespace geometrix {

    template <typename Polyline>
    inline point<typename geometric_traits<typename point_sequence_traits<Polyline>::point_type>::arithmetic_type, dimension_of<typename point_sequence_traits<Polyline>::point_type>::value>
		polyline_mid_point( const Polyline& poly )
    {
        typedef point_sequence_traits<Polyline> access;
		typedef typename point_sequence_traits<Polyline>::point_type point_type;
		typedef typename geometric_traits<point_type>::arithmetic_type arithmetic_type;
		typedef vector<arithmetic_type, dimension_of<point_type>::value> vector_type;
		arithmetic_type length = 0;
        std::size_t size = access::size( poly );
		std::vector<arithmetic_type> lengths;
        for( std::size_t i = 0, j=1; j < size; i = j++ ){
			auto l = point_point_distance(access::get_point(poly, i), access::get_point(poly, j));			
			length += l;
			lengths.push_back(length);
        }

		auto pos = length * 0.5;
		auto it = std::lower_bound(lengths.begin(), lengths.end(), pos);
		auto l = *it - pos;
		auto index = std::distance(lengths.begin(), it);
		GEOMETRIX_ASSERT(index + 1 < size);
		auto next = index + 1;
		return access::get_point(poly, index) + l * normalize<vector_type>(access::get_point(poly, next) - access::get_point(poly, index));
    }

}//namespace geometrix;

#endif //! GEOMETRIX_POLYLINE_MIDPOINT_HPP
