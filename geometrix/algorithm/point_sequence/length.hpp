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
#include <geometrix/algorithm/distance/point_point_distance.hpp>
#include <geometrix/utility/tagged_quantity.hpp>

#include <algorithm>
#include <tuple>
#include <boost/limits.hpp>

namespace geometrix {

    template <typename Polyline>
    inline typename geometric_traits<typename point_sequence_traits<Polyline>::point_type>::arithmetic_type polyline_length( const Polyline& poly )
    {
        typedef point_sequence_traits<Polyline> access;
        typedef typename point_sequence_traits<Polyline>::point_type point_type;
        typedef typename geometric_traits<point_type>::arithmetic_type length_t;
        length_t length = constants::zero<length_t>();
        std::size_t size = access::size( poly );
        for( std::size_t i = 0, j=1; j < size; i = j++ )
        {
            length += point_point_distance(access::get_point(poly, i), access::get_point(poly, j));
        }

        return length;
    }

    template <typename Polygon>
    inline typename geometric_traits<typename point_sequence_traits<Polygon>::point_type>::arithmetic_type polygon_length(const Polygon& poly)
    {
        typedef point_sequence_traits<Polygon> access;
        typedef typename point_sequence_traits<Polygon>::point_type point_type;
        typedef typename geometric_traits<point_type>::arithmetic_type length_t;
        length_t length = constants::zero<length_t>();
        std::size_t size = access::size(poly);
        for (std::size_t i = 0, j = 1; i < size; ++i, j = (j+1)%size)
        {
            length += point_point_distance(access::get_point(poly, i), access::get_point(poly, j));
        }

        return length;
    }

    GEOMETRIX_STRONG_TYPEDEF_TPL(distance_from_start_to_point);
    GEOMETRIX_STRONG_TYPEDEF_TPL(distance_from_point_to_end);
    GEOMETRIX_STRONG_TYPEDEF(std::size_t, polyline_segment_index);
    template <typename Polyline, typename Point, typename NumberComparisonPolicy>
    inline std::tuple
        <
            distance_from_start_to_point<typename arithmetic_type_of<Polyline>::type>
          , distance_from_point_to_end<typename arithmetic_type_of<Polyline>::type>
          , polyline_segment_index
        > point_on_polyline_length_characteristics(const Polyline& P, const Point& p, const NumberComparisonPolicy& cmp)
    {
        using access = point_sequence_traits<Polyline>;
        using length_t = typename arithmetic_type_of<Polyline>::type;
        length_t length = constants::zero<length_t>(), postLength = constants::zero<length_t>();
        std::size_t size = access::size(P), index = (std::numeric_limits<std::size_t>::max)();
        for (std::size_t i = 0, j = 1; j < size; ++i, ++j)
        {
            auto A = access::get_point(P, i);
            auto B = access::get_point(P, j);
            if (index == (std::numeric_limits<std::size_t>::max)())
            {
                if (!is_between(A, B, p, true, cmp))
                    length += point_point_distance(A, B);
                else
                {
                    index = i;
                    length += point_point_distance(A, p);
                    postLength += point_point_distance(p, B);
                }
            }
            else
                postLength += point_point_distance(A, B);
        }

        return std::make_tuple(distance_from_start_to_point<length_t>(length), distance_from_point_to_end<length_t>(postLength), polyline_segment_index(index));
    }

}//namespace geometrix;

#endif //! GEOMETRIX_ALGORITHM_POINTSEQUENCE_LENGTH_HPP
