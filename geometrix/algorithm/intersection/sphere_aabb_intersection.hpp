//
//! Copyright © 2008-2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_SPHERE_AABB_INTERSECTION_HPP
#define GEOMETRIX_SPHERE_AABB_INTERSECTION_HPP

#include <geometrix/algorithm/distance/point_aabb_distance.hpp>
#include <geometrix/arithmetic/vector/magnitude.hpp>
#include <geometrix/primitive/sphere_traits.hpp>

namespace geometrix {

template <typename Sphere, typename AABB>
inline bool sphere_aabb_intersection( const Sphere& s, const AABB& aabb )
{
    using point_t = typename geometric_traits<Sphere>::point_type;
    using length_t = typename geometric_traits<point_t>::arithmetic_type;

    auto d2 = point_aabb_distance_sqrd(get_center(s), aabb);
    return d2 <= get_radius(s) * get_radius(s);
}

template <typename Sphere, typename AABB, typename Point>
inline bool sphere_aabb_intersection( const Sphere& s, const AABB& aabb, Point& p )
{
    using point_t = typename geometric_traits<Sphere>::point_type;
    using length_t = typename geometric_traits<point_t>::arithmetic_type;
    using vector_t = vector<length_t, dimension_of<point_t>::value>;

    assign(p, point_aabb_closest_point(get_center(s), aabb));

    //! Sphere and AABB intersect if the distance from sphere center to point p is less than the sphere radius.
    auto v = vector_t{ p - get_center(s) };
    return magnitude_sqrd(v) <= get_radius(s) * get_radius(s);
}

}//namespace geometrix;

#endif //GEOMETRIX_SPHERE_AABB_INTERSECTION_HPP
