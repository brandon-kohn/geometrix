//
//! Copyright © 2017
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include <geometrix/primitive/polygon_with_holes.hpp>

namespace geometrix {

    template <typename Segment, typename Point>
    inline std::vector<Segment> polygon_with_holes_as_segment_range(const geometrix::polygon_with_holes<Point>& p)
    {
        using namespace geometrix;

        typedef point_sequence_traits<polygon<Point>> access;
        std::vector<Segment> segments;
        auto size = access::size(p.get_outer());
        for (std::size_t i = 0, j = 1; i < size; ++i, j = (j + 1) % size) 
        {
            auto segment = construct<Segment>(access::get_point(p.get_outer(), i), access::get_point(p.get_outer(), j));
            segments.push_back(segment);
        }

        for (const auto& hole : p.get_holes())
        {
            size = access::size(hole);
            for (std::size_t i = 0, j = 1; i < size; ++i, j = (j + 1) % size) 
            {
                auto segment = construct<Segment>(access::get_point(hole, i), access::get_point(hole, j));
                segments.push_back(segment);
            }
        }

        return std::move(segments);
    }

}//! namespace geometrix;

