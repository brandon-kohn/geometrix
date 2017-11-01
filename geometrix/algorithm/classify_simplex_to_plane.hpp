//
//! Copyright © 2017
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_CLASSIFY_SIMPLEX_TO_PLANE_HPP
#define GEOMETRIX_CLASSIFY_SIMPLEX_TO_PLANE_HPP
#pragma once

#include <geometrix/primitive/hyperplane_traits.hpp>

namespace geometrix {

    enum class plane_orientation
    {
        coplanar_with_plane
      , in_front_of_plane
      , in_back_of_plane
      , straddling_plane
    };

    // Classify point p to a plane thickened by a given thickness epsilon
    template <typename Point, typename Hyperplane, typename NumberComparisonPolicy>
    inline plane_orientation classify_point_to_plane(const Point& p, const Hyperplane& plane, const NumberComparisonPolicy& cmp)
    {
        using access = hyperplane_access_traits<typename std::decay<Hyperplane>::type>;
        
        // Compute signed distance of point from plane
        auto dist = scalar_projection(as_vector(p), access::get_normal_vector(plane)) - access::get_distance_to_origin(plane);
        
        // Classify p based on the signed distance
        using number_t = typename std::decay<decltype(dist)>::type;
        auto zero = constants::zero<number_t>();
        if (cmp.greater_than(dist, zero))
            return plane_orientation::in_front_of_plane;
        if (cmp.less_than(dist, zero))
            return plane_orientation::in_back_of_plane;
        
        return plane_orientation::coplanar_with_plane;
    }

    // Return value specifying whether the polygon ‘poly’ lies in front of,
    // behind of, on, or straddles the plane ‘plane’
    template <typename Simplex, typename Hyperplane, typename NumberComparisonPolicy>
    inline plane_orientation classify_simplex_to_plane(const Simplex& smplx, const Hyperplane& plane, const NumberComparisonPolicy& cmp)
    {
        // Loop over all polygon vertices and count how many vertices
        // lie in front of and how many lie behind of the thickened plane
        auto numInFront = 0, numBehind = 0;
        auto numVerts = number_vertices(smplx);
        for (auto i = 0; i < numVerts; ++i)
        {
            auto p = get_vertex(smplx, i);
            switch (classify_point_to_plane(p, plane, cmp))
            {
            case plane_orientation::in_front_of_plane:
                ++numInFront;
                break;
            case plane_orientation::in_back_of_plane:
                ++numBehind;
                break;
            }
        }

        // If vertices on both sides of the plane, the polygon is straddling
        if (numBehind != 0 && numInFront != 0)
            return plane_orientation::straddling_plane;
        // If one or more vertices in front of the plane and no vertices behind
        // the plane, the polygon lies in front of the plane
        if (numInFront != 0)
            return plane_orientation::in_front_of_plane;
        // Ditto, the polygon lies behind the plane if no vertices in front of
        // the plane, and one or more vertices behind the plane
        if (numBehind != 0)
            return plane_orientation::in_back_of_plane;
        // All vertices lie on the plane so the polygon is coplanar with the plane
        return plane_orientation::coplanar_with_plane;
    }
}//! namespace geometrix;

#endif//! GEOMETRIX_CLASSIFY_SIMPLEX_TO_PLANE_HPP
