//
//! Copyright � 2008-2016
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

namespace geometrix {

	//! From Real Time Collision Detection
	// Intersects ray r = p + td, |d| = 1,with AABB a and, if intersecting, 
	// returns t value of intersection and intersection point q 
	template <typename Point1, typename Vector, typename AABB, typename ArithmeticType, typename Point, typename NumberComparisonPolicy>
	inline bool ray_aabb_intersection(const Point1& p, const Vector& d, const AABB& a, ArithmeticType& tmin, Point &q, const NumberComparisonPolicy& cmp)
	{
		// Intersect ray R(t) = p + t*d against AABB a. When intersecting, 
		// return intersection distance tmin and point q of intersection 
		tmin = -(std::numeric_limits<ArithmeticType>::max)(); // set to -FLT_MAX to get first hit on line
		ArithmeticType tmax = (std::numeric_limits<ArithmeticType>::max)(); // set to max distance ray can travel (for segment)

		// For all sides.
		{
			if (cmp.equals(get<0>(d), 0) ) 
			{
				// Ray is parallel to slab. No hit if origin not within slab
				if (get<0>(p) < get<0>(a.get_lower_bound()) || get<0>(p) > get<0>(a.get_upper_bound())) 
					return false;
			}
			else
			{
				// Compute intersection t value of ray with near and far plane of slab 
				ArithmeticType ood = 1 / get<0>(d);
				ArithmeticType t1 = (get<0>(a.get_lower_bound()) - get<0>(p)) * ood;
				ArithmeticType t2 = (get<0>(a.get_upper_bound()) - get<0>(p)) * ood; 
				
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
			if (cmp.equals(get<1>(d), 0))
			{
				// Ray is parallel to slab. No hit if origin not within slab
				if (get<1>(p) < get<1>(a.get_lower_bound()) || get<1>(p) > get<1>(a.get_upper_bound()))
					return false;
			}
			else
			{
				// Compute intersection t value of ray with near and far plane of slab 
				ArithmeticType ood = 1 / get<1>(d);
				ArithmeticType t1 = (get<1>(a.get_lower_bound()) - get<1>(p)) * ood;
				ArithmeticType t2 = (get<1>(a.get_upper_bound()) - get<1>(p)) * ood;

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
		assign(q, p+d*tmin); 
		return true;
	}	
}//! namespace geometrix;

#endif//! GEOMETRIX_RAY_SPHERE_INTERSECTION_HPP