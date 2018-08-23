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
#include <geometrix/algorithm/distance/point_point_distance.hpp>

#include <algorithm>

namespace geometrix {

    template <typename Polyline>
    inline point<typename geometric_traits<typename point_sequence_traits<Polyline>::point_type>::arithmetic_type, dimension_of<typename point_sequence_traits<Polyline>::point_type>::value>
        polyline_mid_point( const Polyline& poly )
    {
        using access = point_sequence_traits<Polyline>;
        using point_t = typename access::point_type;
        using length_t = typename geometric_traits<point_t>::arithmetic_type;
        using dimensionless_t = typename geometric_traits<point_t>::dimensionless_type;
        length_t length = construct<length_t>(0);
        std::size_t size = access::size( poly );
        std::vector<length_t> lengths;
        for( std::size_t i = 0, j=1; j < size; i = j++ )
        {
            auto l = point_point_distance(access::get_point(poly, i), access::get_point(poly, j));          
            length += l;
            lengths.push_back(length);
        }

        auto pos = length * constants::one_half<dimensionless_t>();
        auto it = std::lower_bound(lengths.begin(), lengths.end(), pos);
        auto l = *it - pos;
        auto index = std::distance(lengths.begin(), it);
        GEOMETRIX_ASSERT(index + 1 < size);
        auto next = index + 1;
        return access::get_point(poly, next) + l * normalize(access::get_point(poly, index) - access::get_point(poly, next));
    }

}//namespace geometrix;

#endif //! GEOMETRIX_POLYLINE_MIDPOINT_HPP
