//
//! Copyright © 2008-2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_SPHERE_POLYLINE_INTERSECTION_HPP
#define GEOMETRIX_SPHERE_POLYLINE_INTERSECTION_HPP

#include <geometrix/algorithm/intersection/segment_sphere_intersection.hpp>
#include <geometrix/primitive/point_sequence_traits.hpp>
#include <geometrix/primitive/point.hpp>

/////////////////////////////////////////////////////////////////////////////
//
// NAMESPACE
//
/////////////////////////////////////////////////////////////////////////////
namespace geometrix {

template <typename Sphere, typename Polyline, typename Visitor, typename NumberComparisonPolicy>
inline bool sphere_polyline_intersect( const Sphere& s, const Polyline& pline, Visitor&& visitor, const NumberComparisonPolicy& cmp )
{
    BOOST_CONCEPT_ASSERT((PointSequenceConcept<Polyline>));
    BOOST_CONCEPT_ASSERT((NumberComparisonPolicyConcept<NumberComparisonPolicy>));
    using access = point_sequence_traits<Polyline>;
    using length_t = arithmetic_type_of<typename access::point_type>::type;
    using dimensionless_t = decltype(std::declval<length_t>() / std::declval<length_t>());
    using point_type = point<length_t, 2>;

    bool intersected = false;
    for (std::size_t i1 = 0, j1 = 1; j1 < access::size(pline); i1 = j1++)
    {
        point_type q[2];
        dimensionless_t t[2];
        auto result = segment_sphere_intersection(access::get_point(pline, i1), access::get_point(pline, j1), s, t, q, cmp);
        if (result)
        {
            intersected = true;
            auto stop = visitor(result, i1, j1, t[0], q[0], t[1], q[1]);
            if (stop)
                return intersected;
        }
    }

    return intersected;
}

}//namespace geometrix;

#endif //GEOMETRIX_SPHERE_POLYLINE_INTERSECTION_HPP
