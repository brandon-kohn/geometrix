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

	struct segment_sphere_intersection_result
	{
		segment_sphere_intersection_result()
			: m_t0(0)
			, m_t1(0)
		{}

		segment_sphere_intersection_result(bool t0, bool t1)
			: m_t0(t0)
			, m_t1(t1)
		{}

		explicit operator bool() const { return m_t0 || m_t1; }
		operator std::uint32_t() const { return m_t0 + m_t1; }

		bool is_t0_intersecting() const { return m_t0 != 0; }
		bool is_t1_intersecting() const { return m_t1 != 0; }

	private:

		std::uint32_t m_t0;
		std::uint32_t m_t1;
	};

	//! From Real Time Collision Detection
	// Intersects segment s = p + ld, with sphere s and, if intersecting, 
	// returns number of intersections and through out parameters: 
	//! t[0], t[1] value of intersection(s) and intersection point(s) q[0], q[1]. 
	template <typename Point, typename Sphere, typename Dimensionless, typename NumberComparisonPolicy>
	inline segment_sphere_intersection_result segment_sphere_intersection(const Point& p0, const Point& p1, const Sphere& s, Dimensionless* t, Point* q, const NumberComparisonPolicy& cmp)
	{
		using length_t = typename geometric_traits<Point>::arithmetic_type;
		using vector_t = vector<length_t, dimension_of<Point>::value>;
		using dimensionless_t = typename geometric_traits<Point>::dimensionless_type;
		using area_t = decltype(std::declval<length_t>() * std::declval<length_t>());
		using std::sqrt;

		vector_t d = p1 - p0;
		vector_t f = p0 - get_center(s);

		area_t a = dot_product(d, d);
		area_t b = constants::two<dimensionless_t>() * dot_product(f, d);
		area_t c = dot_product(f, f) - get_radius(s) * get_radius(s);

		using discr_t = decltype(std::declval<area_t>() * std::declval<area_t>());
		discr_t discr = b*b - construct<dimensionless_t>(4) * a*c;
		if (discr < constants::zero<discr_t>())
			return segment_sphere_intersection_result(false, false);

		area_t sqrt_discr = sqrt(discr);

		area_t a2 = constants::two<dimensionless_t>() * a;
		t[0] = (-b - sqrt_discr) / a2;
		t[1] = (-b + sqrt_discr) / a2;

		assign(q[0], p0 + t[0] * d);
		assign(q[1], p0 + t[1] * d);
		
		return segment_sphere_intersection_result(t[0] >= constants::zero<dimensionless_t>() && t[0] <= constants::one<dimensionless_t>(), t[1] >= constants::zero<dimensionless_t>() && t[1] <= constants::one<dimensionless_t>());
	}

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
		using std::sqrt;
		t = -b - sqrt(discr);
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
		using std::sqrt;
		auto length = sqrt(dot_product(d, d));
		return segment_sphere_intersection(sa, d, length, s, t, q, cmp);
	}

	template <typename Segment, typename Sphere, typename ArithmeticType, typename Point, typename NumberComparisonPolicy>
	inline bool segment_sphere_intersection(const Segment& seg, const Sphere& s, ArithmeticType& t, Point &q, const NumberComparisonPolicy& cmp)
	{
		BOOST_CONCEPT_ASSERT((Segment2DConcept<Segment>));
		return segment_sphere_intersection(get_start(seg), get_end(seg), s, t, q, cmp);
	}
}//! namespace geometrix;

#endif//! GEOMETRIX_SEGMENT_SPHERE_INTERSECTION_HPP