//
//! Copyright © 2008-2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_MOVING_SPHERE_SPHERE_INTERSECTION_HPP
#define GEOMETRIX_MOVING_SPHERE_SPHERE_INTERSECTION_HPP

#include <geometrix/primitive/sphere_traits.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/algebra/expression.hpp>
#include <geometrix/algebra/dot_product.hpp>

template <typename Sphere, typename Vector, typename ArithmeticType, typename Point, typename NumberComparisonPolicy>
inline bool moving_sphere_sphere_intersection( const Sphere& s0, const Sphere& s1, const Vector& v0, const Vector& v1, ArithmeticType& t, Point& q, const NumberComparisonPolicy& cmp )
{
	typedef vector<ArithmeticType, dimension_of<Vector>::value> vector_t;
	vector_t s = get_center(s1) - get_center(s0); // Vector between sphere centers 
	vector_t v = v1 - v0; // Relative motion of s1 with respect to stationary s0 
	auto r = get_radius(s1) + get_radius(s0); // Sum of sphere radii 
	auto c = dot_product(s, s) - r * r; 
	if (cmp.less_than(c, 0)) 
	{ 
		// Spheres initially overlapping so exit directly 
		t = 0;
		return true;
	} 
	
	auto a = dot_product(v, v);
	if (cmp.less_than(a, 0)) 
		return false; // Spheres not moving relative each other 
	
	auto b = dot_product(v, s); 
	if (cmp.greater_than_or_equal(b,0)) 
		return false; // Spheres not moving towards each other 
	
	auto d = b * b - a * c; 
	if (cmp.less_than(d, 0)) 
		return false; // No real-valued root, spheres do not intersect

	t = (-b - math::sqrt( d )) / a;

	Point cnew0 = get_center( s0 ) + t * v0;
	Point cnew1 = get_center( s1 ) + t * v1;
	q = cnew0 + get_radius(s0) * normalize<vector_t>(cnew1 - cnew0);
	return true;
}

#endif //! GEOMETRIX_MOVING_SPHERE_SPHERE_INTERSECTION_HPP
