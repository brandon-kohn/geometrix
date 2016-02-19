//
//! Copyright © 2008-2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_MOVING_SPHERE_PLANE_INTERSECTION_HPP
#define GEOMETRIX_MOVING_SPHERE_PLANE_INTERSECTION_HPP

#include <geometrix/primitive/sphere_traits.hpp>
#include <geometrix/primitive/plane_traits.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/algebra/expression.hpp>
#include <geometrix/algebra/dot_product.hpp>

/////////////////////////////////////////////////////////////////////////////
//
// NAMESPACE
//
/////////////////////////////////////////////////////////////////////////////
namespace geometrix {

	// Intersect sphere s with movement vector v with plane p. If intersecting 
	// return time t of collision and point q at which sphere hits plane 
	template <typename Sphere, typename Vector, typename Plane, typename ArithmeticType, typename Point, typename NumberComparisonPolicy>
	inline bool intersect_moving_sphere_plane(const Sphere& s, const Vector& velocity, const Plane& p, ArithmeticType &t, Point& q, const NumberComparisonPolicy& cmp)
	{
		// Compute distance of sphere center to plane 
		ArithmeticType dist = dot_product(p.get_normal_vector(), get_center(s)) - p.get_distance_to_origin(); 
		if( cmp.less_than_or_equal(math::abs( dist ), get_radius(s)) )
		{
			// The sphere is already overlapping the plane. Set time of 
			// intersection to zero and q to sphere center 
			t = 0;
			assign( q, get_center(s) );
			return true;
		}
		else 
		{
			ArithmeticType denom = dot_product( p.get_normal_vector(), velocity ); 
			if( cmp.greater_than_or_equal(denom * dist, 0) )
			{
				// No intersection as sphere moving parallel to or away from plane 
				return false; 
			}
			else 
			{
				// Sphere is moving towards the plane
				// Use +r in computations if sphere in front of plane, else -r 
				ArithmeticType r = dist > 0 ? get_radius(s) : -get_radius(s);
				t = (r - dist) / denom;
				assign(q, get_center(s) + t * velocity - r * p.get_normal_vector());
				return true;
			}
		}
	}
}//namespace geometrix;

#endif //GEOMETRIX_MOVING_SPHERE_PLANE_INTERSECTION_HPP
