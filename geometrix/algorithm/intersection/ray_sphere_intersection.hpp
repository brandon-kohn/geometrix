//
//! Copyright © 2008-2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_RAY_SPHERE_INTERSECTION_HPP
#define GEOMETRIX_RAY_SPHERE_INTERSECTION_HPP

#include <geometrix/primitive/sphere_traits.hpp>
#include <geometrix/primitive/segment_traits.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/algebra/expression.hpp>
#include <geometrix/algebra/dot_product.hpp>

//! From Real Time Collision Detection
// Intersects ray r = p + td, |d| = 1,with sphere s and, if intersecting, 
// returns t value of intersection and intersection point q 
template <typename Point, typename Vector, typename Sphere, typename ArithmeticType, typename NumberComparisonPolicy>
inline bool ray_sphere_intersection(const Point& p, const Vector& d, const Sphere& s, ArithmeticType& t, Point &q, const NumberComparisonPolicy& cmp) 
{
	BOOST_ASSERT( cmp.equals( magnitude_sqrd( d ), 1 ) );

	Vector m = p - get_center(s);
	auto b = dot_product(m, d); 
	auto c = dot_product(m, m) - get_radius(s) * get_radius(s);
	
	// Exit if r’s origin outside s (c > 0) and r pointing away from s (b > 0) 
	if (cmp.greater_than(c,0) && cmp.greater_than(b,0)) 
		return false; 
	
	auto discr = b*b - c;
	
	// A negative discriminant corresponds to ray missing sphere 
	if (cmp.less_than(discr,0)) 
		return false; 

	// Ray now found to intersect sphere, compute smallest t value of intersection 
	t = -b - math::sqrt(discr); 
	
	// If t is negative, ray started inside sphere so clamp t to zero 
	if (cmp.less_than(t,0)) 
		t = 0; 

	assign(q, p + t * d);
	return true;
}

#endif//! GEOMETRIX_RAY_SPHERE_INTERSECTION_HPP