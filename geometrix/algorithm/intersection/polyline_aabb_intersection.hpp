//
//! Copyright © 2008-2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_POLYLINE_AABB_INTERSECTION_HPP
#define GEOMETRIX_POLYLINE_AABB_INTERSECTION_HPP

#include <geometrix/algorithm/intersection/segment_aabb_intersection.hpp>
#include <geometrix/primitive/polyline.hpp>

namespace geometrix {
    
    template <typename Polyline, typename AABB>
    inline bool polyline_aabb_intersection(const Polyline& pline, const AABB& b)
    {
        BOOST_CONCEPT_ASSERT((PointSequenceConcept<Polyline>));
        using access = point_sequence_traits<Polyline>;
        
        for (std::size_t i1 = 0, j1 = 1; j1 < access::size(pline); i1 = j1++)
        {
            auto p1 = access::get_point(pline, i1);
            auto p2 = access::get_point(pline, j1);
            if (segment_aabb_intersection(p1, p2, b))
                return true;
        }

        return false;
    }
}//! namespace geometrix;

#endif//! GEOMETRIX_POLYLINE_AABB_INTERSECTION_HPP
