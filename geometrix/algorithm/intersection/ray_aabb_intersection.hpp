//
//! Copyright © 2008-2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_RAY_AABB_INTERSECTION_HPP
#define GEOMETRIX_RAY_AABB_INTERSECTION_HPP

#include <geometrix/primitive/sphere_traits.hpp>
#include <geometrix/primitive/segment_traits.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/algebra/expression.hpp>
#include <geometrix/algebra/dot_product.hpp>
#include <cstdint>

namespace geometrix {
	
	struct ray_aabb_intersection_result
	{
		typedef void (ray_aabb_intersection_result::*bool_type)() const;

		ray_aabb_intersection_result() = default;

		ray_aabb_intersection_result(bool isIntersecting, bool isOriginInside = false)
			: result((isIntersecting ? e_is_intersecting : 0) | (isOriginInside ? e_origin_inside : 0))
		{}

		bool is_intersecting() const { return (result & e_is_intersecting) != 0; }
		bool is_origin_inside() const { return (result & e_origin_inside) != 0; }

		operator bool_type() const
		{
			return is_intersecting() ? &ray_aabb_intersection_result::bool_type_mfn : 0;
		}

		bool operator == (bool b) const { return b == is_intersecting(); }

	private:

		void bool_type_mfn() const {}

		enum Flags
		{
			e_is_intersecting = 1
			, e_origin_inside = 2
		};

		std::uint32_t result{ 0 };

	};


	//! From Real Time Collision Detection
	// Intersects ray r = p + td, |d| = 1,with AABB a and, if intersecting, 
	// returns t value of intersection and intersection point q.
	//! In the case where p is inside the box, tmin is negative and q is the point on the opposite directed ray where the box intersects.
	template <typename Point1, typename Vector, typename AABB, typename Scalar, typename Point, typename NumberComparisonPolicy>
	inline ray_aabb_intersection_result ray_aabb_intersection(const Point1& p, const Vector& d, const AABB& a, Scalar& tmin, Point &q, const NumberComparisonPolicy& cmp)
	{
		// Intersect ray R(t) = p + t*d against AABB a. When intersecting, 
		// return intersection distance tmin and point q of intersection 
#if BOOST_VERSION >= 105700
		tmin = std::numeric_limits<Scalar>::lowest(); // set to -FLT_MAX to get first hit on lineb
#else
		tmin = construct<Scalar>(-(std::numeric_limits<double>::max)());
#endif
		Scalar tmax = (std::numeric_limits<Scalar>::max)(); // set to max distance ray can travel (for segment)

		// For all sides.
		{
			if (cmp.equals(get<0>(d), constants::zero<typename type_at<Vector, 0>::type>()) ) 
			{
				// Ray is parallel to slab. No hit if origin not within slab
				if (get<0>(p) < get<0>(a.get_lower_bound()) || get<0>(p) > get<0>(a.get_upper_bound())) 
					return false;
			}
			else
			{
				// Compute intersection t value of ray with near and far plane of slab 
				using factor_t = decltype((Scalar() * typename type_at<Vector, 0>::type()) / (typename type_at<Point1, 0>::type()));
				auto ood = constants::one<factor_t>() / get<0>(d);
				Scalar t1 = (get<0>(a.get_lower_bound()) - get<0>(p)) * ood;
				Scalar t2 = (get<0>(a.get_upper_bound()) - get<0>(p)) * ood; 
				
				// Make t1 be intersection with near plane, t2 with far plane 
				if (t1 > t2) 
					std::swap(t1, t2); 
				
				// Compute the intersection of slab intersection intervals 
				tmin = (std::max)(tmin, t1); // Rather than: if (t1 > tmin) tmin = t1;
				tmax = (std::min)(tmax, t2); // Rather than: if (t2 < tmax) tmax = t2;
				// Exit with no collision as soon as slab intersection becomes empty 
				if (tmin > tmax) 
					return false;
			} 
		} 

		{
			if (cmp.equals(get<1>(d), constants::zero<typename type_at<Vector, 1>::type>()))
			{
				// Ray is parallel to slab. No hit if origin not within slab
				if (get<1>(p) < get<1>(a.get_lower_bound()) || get<1>(p) > get<1>(a.get_upper_bound()))
					return false;
			}
			else
			{
				// Compute intersection t value of ray with near and far plane of slab 
				using factor_t = decltype((Scalar() * typename type_at<Vector, 1>::type()) / (typename type_at<Point1, 1>::type()));
				auto ood = constants::one<factor_t>() / get<1>(d);
				Scalar t1 = (get<1>(a.get_lower_bound()) - get<1>(p)) * ood;
				Scalar t2 = (get<1>(a.get_upper_bound()) - get<1>(p)) * ood;

				// Make t1 be intersection with near plane, t2 with far plane 
				if (t1 > t2)
					std::swap(t1, t2);

				// Compute the intersection of slab intersection intervals 
				if (t1 > tmin)
					tmin = t1;
				if (t2 > tmax)
					tmax = t2;
				// Exit with no collision as soon as slab intersection becomes empty 
				if (tmin > tmax)
					return false;
			}
		}
		
		// Ray intersects all 2 slabs. Return point (q) and intersection t value (tmin) 
		auto result = ray_aabb_intersection_result(true, tmin < constants::zero<Scalar>());		
		
		assign(q, p+d*tmin);		
		return result;
	}	
	
	//! From Real Time Collision Detection
	// Intersects ray r = p + td, |d| = 1,with AABB a and, if intersecting, 
	// returns t0 and t1 values of both intersection distances and intersection points q0, q1. If intersecting at a corner both will be the same value.
	//! In the case where p is inside the box, tmin is negative and q is the point on the opposite directed ray where the box intersects.
	template <typename Point1, typename Vector, typename AABB, typename Scalar, typename Point, typename NumberComparisonPolicy>
	inline ray_aabb_intersection_result ray_aabb_intersection(const Point1& p, const Vector& d, const AABB& a, Scalar& tentrance, Point& q0, Scalar& texit, Point& q1, const NumberComparisonPolicy& cmp)
	{
		// Intersect ray R(t) = p + t*d against AABB a. When intersecting, 
		// return intersection distance tmin and point q of intersection 
#if BOOST_VERSION >= 105700
		auto tmin = std::numeric_limits<Scalar>::lowest(); // set to -FLT_MAX to get first hit on lineb
#else
		auto tmin = construct<Scalar>(-(std::numeric_limits<double>::max)());
#endif
		auto tmax = (std::numeric_limits<Scalar>::max)(); // set to max distance ray can travel (for segment)
													 // For all sides.

		{
			if (cmp.equals(get<0>(d), constants::zero<typename type_at<Vector, 0>::type>()))
			{
				// Ray is parallel to slab. No hit if origin not within slab
				if (get<0>(p) < get<0>(a.get_lower_bound()) || get<0>(p) > get<0>(a.get_upper_bound()))
					return false;
			}
			else 
			{
				// Compute intersection t value of ray with near and far plane of slab 
				using factor_t = decltype((Scalar() * typename type_at<Vector, 0>::type()) / (typename type_at<Point1, 0>::type()));
				auto ood = constants::one<factor_t>() / get<0>(d);
				Scalar t1 = (get<0>(a.get_lower_bound()) - get<0>(p)) * ood;
				Scalar t2 = (get<0>(a.get_upper_bound()) - get<0>(p)) * ood;

				// Make t1 be intersection with near plane, t2 with far plane 
				if (t1 > t2)
					std::swap(t1, t2);

				// Compute the intersection of slab intersection intervals 
				if (t1 > tmin)
					tmin = t1;
				if (t2 < tmax)
					tmax = t2;
				
				// Exit with no collision as soon as slab intersection becomes empty 
				if (tmin > tmax)
					return false;
			}
		}

		texit = tmax;
		{
			if (cmp.equals(get<1>(d), constants::zero<typename type_at<Vector, 1>::type>()))
			{
				// Ray is parallel to slab. No hit if origin not within slab
				if (get<1>(p) < get<1>(a.get_lower_bound()) || get<1>(p) > get<1>(a.get_upper_bound()))
					return false;
			}
			else
			{
				// Compute intersection t value of ray with near and far plane of slab 
				using factor_t = decltype((Scalar() * typename type_at<Vector, 1>::type()) / (typename type_at<Point1, 1>::type()));
				auto ood = constants::one<factor_t>() / get<1>(d);
				Scalar t1 = (get<1>(a.get_lower_bound()) - get<1>(p)) * ood;
				Scalar t2 = (get<1>(a.get_upper_bound()) - get<1>(p)) * ood;

				// Make t1 be intersection with near plane, t2 with far plane 
				if (t1 > t2)
					std::swap(t1, t2);

				// Compute the intersection of slab intersection intervals 
				if (t1 > tmin)
					tmin = t1;
				if (t2 > tmax)
					tmax = t2;
				
				// Exit with no collision as soon as slab intersection becomes empty 
				if (tmin > tmax)
					return false;

				texit = (std::min)(t2, texit);
			}
		}

		// Ray intersects all 2 slabs. Return point (q) and intersection t value (tmin) 
		tentrance = tmin;
		assign(q0, p + d*tentrance);
		assign(q1, p + d*texit);

		return ray_aabb_intersection_result(true, tmin < constants::zero<Scalar>());
	}
}//! namespace geometrix;

#endif//! GEOMETRIX_RAY_SPHERE_INTERSECTION_HPP