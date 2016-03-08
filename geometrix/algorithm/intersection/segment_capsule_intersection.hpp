//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_ALGORITHM_INTERSECTION_SEGMENT_CAPSULE_INTERSECTION_HPP
#define GEOMETRIX_ALGORITHM_INTERSECTION_SEGMENT_CAPSULE_INTERSECTION_HPP

#include <geometrix/arithmetic/vector.hpp>
#include <geometrix/arithmetic/arithmetic.hpp>
#include <geometrix/algebra/algebra.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/primitive/sphere.hpp>
#include <geometrix/algorithm/intersection/segment_sphere_intersection.hpp>

namespace geometrix {
	   
    template <typename Point1, typename Point2, typename Point3, typename Point4, typename RadiusType, typename ArithmeticType, typename XPoint, typename NumberComparisonPolicy>
    inline bool segment_capsule_intersection( const Point1& sa, const Point2& sb, const Point3& p, const Point4& q, const RadiusType& r, ArithmeticType& t, XPoint& xPoint, const NumberComparisonPolicy& cmp )
    {
		BOOST_CONCEPT_ASSERT( (PointConcept<Point1>) );
		BOOST_CONCEPT_ASSERT( (PointConcept<Point2>) );
		BOOST_CONCEPT_ASSERT( (PointConcept<Point3>) );
		BOOST_CONCEPT_ASSERT( (PointConcept<Point4>) );
		BOOST_CONCEPT_ASSERT( (NumberComparisonPolicyConcept<NumberComparisonPolicy>) );
		
		// Intersect segment S(t)=sa+t(sb-sa), 0<=t<=1 against capsule specifiedby p, q and r 
		vector<ArithmeticType, 2> d = q - p, m = sa - p, n = sb - sa;
		ArithmeticType md = dot_product(m, d); 
		ArithmeticType nd = dot_product(n, d);
		ArithmeticType dd = dot_product(d, d);
		ArithmeticType nn = dot_product(n, n); 

		// Test if segment fully outside either endcap of capsule 
		if (md < 0 && md + nd < 0)
			return segment_sphere_intersection(sa, n, math::sqrt(nn), make_sphere(p, r), t, xPoint, cmp); // Segment outside 'p' side of capsule
		if (md > dd && md + nd > dd)
			return segment_sphere_intersection(sa, n, math::sqrt(nn), make_sphere(q, r), t, xPoint, cmp); // Segment outside 'q' side of capsule 
				
		ArithmeticType mn = dot_product(m, n);
		ArithmeticType a = dd * nn - nd * nd; 
		ArithmeticType k = dot_product(m, m) - r * r; 
		ArithmeticType c = dd * k - md * md; 
		if (cmp.less_than(math::abs(a), 0))
		{
			// Segment runs parallel to capsule axis
			if (c > 0)
				return false;// 'a' and thus the segment lie outside capsule

			// Now known that segment intersects capsule; figure out how it intersects 
			if (md < 0)
			{
				//t = -mn / nn;// Intersect segment against 'p' endcap 
				segment_sphere_intersection(sa, n, math::sqrt(nn), make_sphere(p, r), t, xPoint, cmp); // Segment outside 'p' side of capsule
			}
			else if (md > dd)
			{
				//t = (nd - mn) / nn;// Intersect segment against 'q' endcap 
				segment_sphere_intersection(sa, n, math::sqrt(nn), make_sphere(q, r), t, xPoint, cmp);
			}
			else
			{
				t = 0; // 'a' lies inside capsule 
				assign(xPoint, sa);
			}

			return true;
		}
		
		ArithmeticType b = dd * mn - nd * md;
		ArithmeticType discr = b * b - a * c;
		if (discr < 0) 
			return false; // No real roots; no intersection
		
		ArithmeticType t0 = t = (-b - math::sqrt(discr)) / a;
		if (md + t * nd < 0) 
		{
			// Intersection outside capsule on 'p' side
			// Exit if r’s origin outside s (k > 0) and r pointing away from s (mn > 0) 
			if (cmp.greater_than(k, 0) && cmp.greater_than(mn, 0))
				return false;

			return segment_sphere_intersection(sa, n, math::sqrt(nn), make_sphere(p, r), t, xPoint, cmp);
		}
		else if (md + t * nd > dd) {
			// Intersection outside capsule on 'q' side
			// Exit if r’s origin outside s (k > 0) and r pointing away from s (mn > 0) 
			return segment_sphere_intersection(sa, n, math::sqrt(nn), make_sphere(q, r), t, xPoint, cmp);
		}

		t = t0;
		assign(xPoint, sa + t * n);

		// Intersection if segment intersects capsule between the end-caps
		return t >= 0 && t <= 1;
    }
	
}//namespace geometrix;

#endif //GEOMETRIX_ALGORITHM_INTERSECTION_SEGMENT_SEGMENT_INTERSECTION_HPP
