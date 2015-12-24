//
//! Copyright © 2015
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_EBERLY_TRIANGLE_AABB_INTERSECTION_HPP
#define GEOMETRIX_EBERLY_TRIANGLE_AABB_INTERSECTION_HPP

#include <geometrix/algorithm/bounding_box_intersection.hpp>
#include <geometrix/algebra/expression.hpp>
#include <geometrix/primitive/point_traits.hpp>
#include <geometrix/tensor/vector.hpp>
#include <boost/concept_check.hpp>

namespace geometrix {

	//! Implementation from Real Time Collisions.
	template <typename Point1, typename Point2, typename Point3, typename AABB, typename NumberComparisonPolicy>
	inline bool eberly_triangle_aabb_intersection_2d( const Point1& A, const Point2& B, const Point3& C, const AABB& box, const NumberComparisonPolicy& cmp )
	{
		BOOST_CONCEPT_ASSERT( (Point2DConcept<Point1>) );
		BOOST_CONCEPT_ASSERT( (Point2DConcept<Point2>) );
		BOOST_CONCEPT_ASSERT( (Point2DConcept<Point3>) );
		BOOST_CONCEPT_ASSERT( (NumberComparisonPolicyConcept<NumberComparisonPolicy>) );

		typedef geometric_traits<Point1>::arithmetic_type arithmetic_type;

		arithmetic_type p0, p1, p2, r;
		// Compute box center and extents (if not already given in that format) 
		vector<arithmetic_type, 2> c = (as_vector(box.get_lower_bound()) + as_vector(box.get_upper_bound())) * 0.5;
		
		arithmetic_type e0 = (get<0>( box.get_upper_bound() ) - get<0>( box.get_lower_bound() )) * 0.5;
		arithmetic_type e1 = (get<1>( box.get_upper_bound() ) - get<1>( box.get_lower_bound() )) * 0.5;

		// Translate triangle as conceptually moving AABB to origin 
		vector<arithmetic_type, 2> v0 = A - c;
		vector<arithmetic_type, 2> v1 = B - c;
		vector<arithmetic_type, 2> v2 = C - c;

		// Compute edge vectors for triangle
		vector<arithmetic_type, 2> f0 = v0 - v1, f1 = v1 - v2, f2 = v2 - v0;

		//! 5 axes to check:
		vector<arithmetic_type, 2> a0( get<1>( f0 ), -get<0>( f0 ) ), a1( get<1>( f1 ), -get<0>( f1 ) ), a2( get<1>( f2 ), -get<0>( f2 ) ), a3( 1, 0 ), a4( 0, 1 );

		//! a0
		{
			p0 <<= v0 * a0;
			p1 <<= v1 * a0;
			p2 <<= v2 * a0;
			r = e0 * math::abs( dot_product( a3, a0 ) ) + e1 * math::abs( dot_product( a4, a0 ) );
			if( cmp.less_than((std::max)(p0, (std::max)(p1, p2)), -r) || cmp.greater_than((std::min)(p0, (std::min)(p1, p2)), r) )
				return false;
		} 

		//! a1
		{
			p0 <<= v0 * a1;
			p1 <<= v1 * a1;
			p2 <<= v2 * a1;
			r = e0 * math::abs( dot_product( a3, a1 ) ) + e1 * math::abs( dot_product( a4, a1 ) );
			if( cmp.less_than((std::max)(p0, (std::max)(p1, p2)), -r) || cmp.greater_than((std::min)(p0, (std::min)(p1, p2)), r) )
				return false;
		}

		//! a2
		{
			p0 <<= v0 * a2;
			p1 <<= v1 * a2;
			p2 <<= v2 * a2;
			r = e0 * math::abs( dot_product( a3, a2 ) ) + e1 * math::abs( dot_product( a4, a2 ) );
			if( cmp.less_than((std::max)(p0, (std::max)(p1, p2)), -r) || cmp.greater_than((std::min)(p0, (std::min)(p1, p2)), r) )
				return false;
		}

		//! a3
		{
			p0 <<= v0 * a3;
			p1 <<= v1 * a3;
			p2 <<= v2 * a3;
			r = e0;
			if( cmp.less_than((std::max)(p0, (std::max)(p1, p2)), -r) || cmp.greater_than((std::min)(p0, (std::min)(p1, p2)), r) )
				return false;
		}

		//! a4
		{
			p0 <<= v0 * a4;
			p1 <<= v1 * a4;
			p2 <<= v2 * a4;
			r = e1;
			if( cmp.less_than((std::max)(p0, (std::max)(p1, p2)), -r) || cmp.greater_than((std::min)(p0, (std::min)(p1, p2)), r) )
				return false;
		}
		
		//! for the 2D case, it must intersect at this point.
		return true;
	}
}//! namespace geometrix;

#endif //! GEOMETRIX_EBERLY_TRIANGLE_AABB_INTERSECTION_HPP
