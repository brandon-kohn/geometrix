//
//! Copyright © 2008-2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_SPHERE_OBB_INTERSECTION_HPP
#define GEOMETRIX_SPHERE_OBB_INTERSECTION_HPP

#include <geometrix/algorithm/distance/point_obb_distance.hpp>
#include <geometrix/arithmetic/vector/magnitude.hpp>
#include <geometrix/primitive/sphere_traits.hpp>

namespace geometrix {

template <typename Sphere, typename OBB>
inline bool sphere_obb_intersection( const Sphere& s, const OBB& obb )
{
    using point_t = typename geometric_traits<Sphere>::point_type;
    using length_t = typename geometric_traits<point_t>::arithmetic_type;
    using vector_t = vector<length_t, dimension_of<point_t>::value>;

    auto p = closest_point_point_obb(get_center(s), obb);

    //! Sphere and OBB intersect if the distance from sphere center to point p is less than the sphere radius.
    auto v = vector_t{ p - get_center(s) };
    return magnitude_sqrd(v) <= get_radius(s) * get_radius(s);
}

}//namespace geometrix;

#endif //GEOMETRIX_SPHERE_OBB_INTERSECTION_HPP
