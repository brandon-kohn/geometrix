//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_SEGMENT_PLANE_INTERSECTION_HPP
#define GEOMETRIX_SEGMENT_PLANE_INTERSECTION_HPP
#pragma once

#include <geometrix/numeric/constants.hpp>
#include <geometrix/primitive/segment.hpp>
#include <geometrix/primitive/hyperplane_traits.hpp>
#include <geometrix/algebra/dot_product.hpp>

namespace geometrix {

template <typename Point1, typename Point2, typename Plane, typename Dimensionless, typename XPoint>
inline bool segment_plane_intersection(const Point1& a, const Point2& b, const Plane& p, Dimensionless& t, XPoint& q)
{
    using length_t = typename geometric_traits<Point1>::arithmetic_type;
    using vector_t = vector<length_t, dimension_of<Point1>::value>;
    using access = hyperplane_access_traits<typename std::decay<Plane>::type>;
    
    vector_t ab = b - a;
    auto pd = access::get_distance_to_origin(p);
    auto pn = access::get_normal_vector(p);
    t = (pd - dot_product(pn, as_vector(a))) / dot_product(pn, ab);
    
    if( t >= constants::zero<Dimensionless>() && t <= constants::one<Dimensionless>() ) 
    {
        q = a + t * ab;
        return true;
    }
    
    return false;
}

}//! namespace geometrix;

#endif//! GEOMETRIX_SEGMENT_PLANE_INTERSECTION_HPP
