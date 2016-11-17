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

namespace geometrix {

	template <typename Sphere, typename Velocity, typename Time, typename Point, typename NumberComparisonPolicy>
	inline bool moving_sphere_sphere_intersection(const Sphere& s0, const Sphere& s1, const Velocity& v0, const Velocity& v1, Time& t, Point& q, const NumberComparisonPolicy& cmp)
	{
		using point_t = typename geometric_traits<Sphere>::point_type;
		using length_t = typename geometric_traits<point_t>::arithmetic_type;
		using area_t = decltype(std::declval<length_t>() * std::declval<length_t>());

		using vector_t = vector<length_t, dimension_of<point_t>::value>;
		using velocity_t = Velocity;
		//using speed_t = typename geometric_traits<Velocity>::arithmetic_type;

		vector_t s = get_center(s1) - get_center(s0); // Vector between sphere centers 
		velocity_t v = v1 - v0; // Relative motion of s1 with respect to stationary s0 
		auto r = get_radius(s1) + get_radius(s0); // Sum of sphere radii 
		auto c = dot_product(s, s) - r * r;
		if (cmp.less_than(c, constants::zero<decltype(c)>()))
		{
			// Spheres initially overlapping so exit directly 
			t = constants::zero<Time>();
			return true;
		}

		auto a = dot_product(v, v);
		if (cmp.less_than(a, constants::zero<decltype(a)>()))
			return false; // Spheres not moving relative each other 

		auto b = dot_product(v, s);
		if (cmp.greater_than_or_equal(b, constants::zero<decltype(b)>()))
			return false; // Spheres not moving towards each other 

		auto d = b * b - a * c;
		if (cmp.less_than(d, constants::zero<decltype(d)>()))
			return false; // No real-valued root, spheres do not intersect

		using std::sqrt;
		t = geometrix::get((-b - sqrt(d)) / a);

		Point cnew0 = get_center(s0) + t * v0;
		Point cnew1 = get_center(s1) + t * v1;
		assign(q, cnew0 + get_radius(s0) * normalize(cnew1 - cnew0));
		return true;
	}

	template <typename Sphere, typename Velocity, typename NumberComparisonPolicy>
	inline bool moving_sphere_sphere_intersection(const Sphere& s0, const Sphere& s1, const Velocity& v0, const Velocity& v1, const NumberComparisonPolicy& cmp)
	{
		using sphere_point_t = typename geometric_traits<Sphere>::point_type;
		using length_t = typename geometric_traits<sphere_point_t>::arithmetic_type;
		using time_t = typename arithmetic_type_of<Velocity>::type;
		using point_t = point<length_t, dimension_of<sphere_point_t>::value>;

		time_t t;
		point_t q;
		return moving_sphere_sphere_intersection(s0, s1, v0, v1, t, q, cmp);
	}

}//! namespace geometrix;

#endif //! GEOMETRIX_MOVING_SPHERE_SPHERE_INTERSECTION_HPP
