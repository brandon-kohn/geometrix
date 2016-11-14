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
		using std::abs;
		BOOST_CONCEPT_ASSERT( (Point2DConcept<Point1>) );
		BOOST_CONCEPT_ASSERT( (Point2DConcept<Point2>) );
		BOOST_CONCEPT_ASSERT( (Point2DConcept<Point3>) );
		BOOST_CONCEPT_ASSERT( (NumberComparisonPolicyConcept<NumberComparisonPolicy>) );

		using arithmetic_type = typename geometric_traits<Point1>::arithmetic_type;
		using dimensionless_type = typename geometric_traits<Point1>::dimensionless_type;
		using dimensionless_vector_t = vector<dimensionless_type, 2>;

		arithmetic_type p0, p1, p2;
		// Compute box center and extents (if not already given in that format) 
		vector<arithmetic_type, 2> c = (as_vector(box.get_lower_bound()) + as_vector(box.get_upper_bound())) * 0.5;
		
		arithmetic_type e0 = (get<0>( box.get_upper_bound() ) - get<0>( box.get_lower_bound() )) * 0.5;
		arithmetic_type e1 = (get<1>( box.get_upper_bound() ) - get<1>( box.get_lower_bound() )) * 0.5;

		// Translate triangle as conceptually moving AABB to origin 
		vector<arithmetic_type, 2> v0 = A - c;
		vector<arithmetic_type, 2> v1 = B - c;
		vector<arithmetic_type, 2> v2 = C - c;

		// Compute edge vectors for triangle
		auto f0 = normalize(v0 - v1);
		auto f1 = normalize(v1 - v2);
		auto f2 = normalize(v2 - v0);

		//! 5 axes to check:
		dimensionless_vector_t a0( get<1>( f0 ), get<0>( -f0 ) ), a1( get<1>( f1 ), get<0>( -f1 ) ), a2( get<1>( f2 ), get<0>( -f2 ) ), a3( constants::one<dimensionless_type>(), constants::zero<dimensionless_type>() ), a4( constants::zero<dimensionless_type>(), constants::one<dimensionless_type>() );

		//! a0
		{
			p0 = scalar_projection(v0, a0);
			p1 = scalar_projection(v1, a0);
			p2 = scalar_projection(v2, a0);
			auto r = get(e0 * abs(scalar_projection(a3, a0)) + e1 * abs(scalar_projection(a4, a0)));
			if( cmp.less_than((std::max)(p0, (std::max)(p1, p2)), get(-r)) || cmp.greater_than((std::min)(p0, (std::min)(p1, p2)), r) )
				return false;
		} 

		//! a1
		{
			p0 = scalar_projection(v0, a1);
			p1 = scalar_projection(v1, a1);
			p2 = scalar_projection(v2, a1);	
			auto r = get(e0 * abs(scalar_projection(a3, a1)) + e1 * abs(scalar_projection(a4, a1)));
			if( cmp.less_than((std::max)(p0, (std::max)(p1, p2)), get(-r)) || cmp.greater_than((std::min)(p0, (std::min)(p1, p2)), r) )
				return false;
		}

		//! a2
		{
			p0 = scalar_projection(v0, a2);
			p1 = scalar_projection(v1, a2);
			p2 = scalar_projection(v2, a2);
			auto r = get( e0 * abs(scalar_projection(a3, a2)) + e1 * abs(scalar_projection(a4, a2)) );
			if( cmp.less_than((std::max)(p0, (std::max)(p1, p2)), get(-r)) || cmp.greater_than((std::min)(p0, (std::min)(p1, p2)), r) )
				return false;
		}

		//! a3
		{
			p0 = scalar_projection(v0, a3);
			p1 = scalar_projection(v1, a3);
			p2 = scalar_projection(v2, a3);
			auto r = e0;
			if( cmp.less_than((std::max)(p0, (std::max)(p1, p2)), get(-r)) || cmp.greater_than((std::min)(p0, (std::min)(p1, p2)), r) )
				return false;
		}

		//! a4
		{
			p0 = scalar_projection(v0, a4);
			p1 = scalar_projection(v1, a4);
			p2 = scalar_projection(v2, a4);
			auto r = e1;
			if( cmp.less_than((std::max)(p0, (std::max)(p1, p2)), get(-r)) || cmp.greater_than((std::min)(p0, (std::min)(p1, p2)), r) )
				return false;
		}
		
		//! for the 2D case, it must intersect at this point.
		return true;
	}
}//! namespace geometrix;

#endif //! GEOMETRIX_EBERLY_TRIANGLE_AABB_INTERSECTION_HPP
