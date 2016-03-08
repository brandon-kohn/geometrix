//
//! Copyright © 2008-2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_SEGMENT_SPHERE_INTERSECTION_HPP
#define GEOMETRIX_SEGMENT_SPHERE_INTERSECTION_HPP

#include <geometrix/primitive/sphere_traits.hpp>
#include <geometrix/primitive/segment_traits.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/algebra/expression.hpp>
#include <geometrix/algebra/dot_product.hpp>

namespace geometrix {
	//! From Real Time Collision Detection
	// Intersects segment s = p + td, with sphere s and, if intersecting, 
	// returns t value of intersection and intersection point q 
	template <typename Point1, typename Vector, typename LengthType, typename Sphere, typename ArithmeticType, typename Point, typename NumberComparisonPolicy>
	inline bool segment_sphere_intersection(const Point1& sa, Vector d, const LengthType& length, const Sphere& s, ArithmeticType& t, Point &q, const NumberComparisonPolicy& cmp)
	{
		typedef typename geometric_traits<Point1>::arithmetic_type arithmetic_type;
		typedef vector<arithmetic_type, dimension_of<Point1>::value> vector_type;
		vector_type m = sa - get_center(s);
		auto c = dot_product(m, m) - get_radius(s) * get_radius(s);
		
		if (cmp.equals(length, 0))
		{
			t = 0;
			return cmp.less_than_or_equal(c, 0);
		}

		assign(d, d / length);
		auto b = dot_product(m, d);

		// Exit if r’s origin outside s (c > 0) and r pointing away from s (b > 0) 
		if (cmp.greater_than(c, 0) && cmp.greater_than(b, 0))
			return false;

		auto discr = b*b - c;

		// A negative discriminant corresponds to ray missing sphere 
		if (cmp.less_than(discr, 0))
			return false;

		// segment now found to intersect sphere, compute smallest t value of intersection 
		t = -b - math::sqrt(discr);
		if (cmp.greater_than(t, length))
			return false;

		// If t is negative, segment started inside sphere so clamp t to zero 
		if (cmp.less_than(t, 0))
			t = 0;

		assign(q, sa + t * d);
		t /= length;

		return true;
	}

	//! From Real Time Collision Detection
	// Intersects segment s = p + td, with sphere s and, if intersecting, 
	// returns t value of intersection and intersection point q 
	template <typename Point1, typename Point2, typename Sphere, typename ArithmeticType, typename Point, typename NumberComparisonPolicy>
	inline bool segment_sphere_intersection(const Point1& sa, const Point2& sb, const Sphere& s, ArithmeticType& t, Point &q, const NumberComparisonPolicy& cmp)
	{
		BOOST_CONCEPT_ASSERT((Point2DConcept<Point2>));
		typedef typename geometric_traits<Point1>::arithmetic_type arithmetic_type;
		typedef vector<arithmetic_type, dimension_of<Point1>::value> vector_type;
		vector_type d = sb - sa;
		auto length = math::sqrt(dot_product(d, d));
		return segment_sphere_intersection(sa, d, length, s, t, q, cmp);
	}
}//! namespace geometrix;

#endif//! GEOMETRIX_SEGMENT_SPHERE_INTERSECTION_HPP