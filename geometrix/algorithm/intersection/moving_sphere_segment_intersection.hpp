//
//! Copyright © 2008-2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_MOVING_SPHERE_SEGMENT_INTERSECTION_HPP
#define GEOMETRIX_MOVING_SPHERE_SEGMENT_INTERSECTION_HPP

#include <geometrix/primitive/sphere_traits.hpp>
#include <geometrix/primitive/segment_traits.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/algebra/expression.hpp>
#include <geometrix/algebra/dot_product.hpp>
#include <geometrix/algorithm/intersection/ray_sphere_intersection.hpp>

/////////////////////////////////////////////////////////////////////////////
//
// NAMESPACE
//
/////////////////////////////////////////////////////////////////////////////
namespace geometrix {

	namespace intersect_moving_sphere_segment_detail {

		template <typename Vector1, typename Vector2, typename SpherePoint, typename Radius, typename Point, typename ArithmeticType, typename SegmentPoint, typename NumberComparisonPolicy>
		inline bool moving_sphere_toward_segment_endpoint_intersect( const Vector1& velocity, const Vector2& center_a, const SpherePoint& center, const Radius& radius, ArithmeticType &t, Point& q, const SegmentPoint& a, const NumberComparisonPolicy &cmp )
		{
			typedef vector<ArithmeticType, dimension_of<Point>::value> vector_t;
			typedef point<ArithmeticType, dimension_of<Point>::value> point_t;

			auto speed = magnitude( velocity );
			vector_t direction = velocity / speed;
			auto center_a_dot_direction = dot_product( center_a, direction );
			auto d_minus_r_sqrd = dot_product( center_a, center_a ) - radius * radius;

			// Exit if r’s origin outside s (d_minus_r_sqrd > 0) and r pointing away from s (center_a_dot_direction > 0)
			//! TODO: This might be redundant here as the direction is established already... right?
			if( cmp.greater_than( d_minus_r_sqrd, 0 ) && cmp.greater_than( center_a_dot_direction, 0 ) )
				return false;

			auto discr = center_a_dot_direction*center_a_dot_direction - d_minus_r_sqrd;

			// A negative discriminant corresponds to ray missing sphere 
			if( cmp.less_than( discr, 0 ) )
				return false;

			// Ray now found to intersect sphere, compute smallest t value of intersection 
			t = -center_a_dot_direction - math::sqrt( discr );

			// If t is negative, ray started inside sphere so clamp t to zero 
			if( cmp.less_than( t, 0 ) )
				t = 0;

			//! t is now the distance along direction where the center will be when it intersects a.
			point_t newCenter = center + t * direction;

			//! Now find the intersection point on the sphere.
			assign( q, newCenter + radius * normalize<vector_t>( a - newCenter ) );

			//! Finally, t is a distance... change it to a time.
			t = t / speed;

			return true;
		}
	}//! namespace intersect_moving_sphere_segment_detail;

	// Intersect sphere s with movement vector v with segment seg. If intersecting 
	// return time t of collision and point q at which sphere hits the segment.
	template <typename Sphere, typename Vector, typename Segment, typename ArithmeticType, typename Point, typename NumberComparisonPolicy>
	inline bool intersect_moving_sphere_segment(const Sphere& s, const Vector& velocity, const Segment& seg, ArithmeticType &t, Point& q, const NumberComparisonPolicy& cmp)
	{
		using namespace intersect_moving_sphere_segment_detail;

		//! get the dot product with the endpoints of the segment.
		auto center = get_center( s );
		auto radius = get_radius( s );
		auto a = get_start( seg );
		auto b = get_end( seg );
		typedef vector<ArithmeticType, dimension_of<Point>::value> vector_t;
		typedef point<ArithmeticType, dimension_of<Point>::value> point_t;
		line<typename point_type_of<Segment>::type, vector_t> l( seg );
		const vector_t& parallel = l.get_parallel_vector();
		const vector_t& normal = l.get_normal_vector();

		//! Check if the sphere is already on the line formed by seg.
		
		// Compute distance of sphere center to plane 
		ArithmeticType ndist = dot_product( normal, center ) - l.get_distance_to_origin();
		if( cmp.less_than_or_equal( math::abs( ndist ), radius ) )
		{
			//! The sphere is already overlapping the line.
			//! Check if it is already interior to the segment.
			vector_t center_a = center - a;			
			auto distca = dot_product( center_a, parallel );			
			if( cmp.greater_than_or_equal(distca, 0) )
			{
				vector_t center_b = center - b;
				auto distcb = dot_product( center_b, -parallel );
				if( cmp.greater_than_or_equal( distcb, 0 ) )
				{
					t = 0;
					assign( q, center );
					return true;
				}

				//! Starting position projects outside segment end point along line.
				//! Need to check intersection with segment end point along velocity.
				auto denom = dot_product( -parallel, velocity );
				if( cmp.less_than_or_equal( denom, 0 ) )
				{
					//! Moving away from segment.
					return false;
				}

				//! Moving toward the segment endpoint. Check for intersection.
				return moving_sphere_toward_segment_endpoint_intersect( velocity, center_b, center, radius, t, q, b, cmp );
			}
			
			//! Starting position projects outside segment start point along line.
			//! Need to check intersection with segment start point along velocity.
			auto denom = dot_product( parallel, velocity );
			if( cmp.less_than_or_equal(denom, 0) )
			{
				//! Moving away from segment.
				return false;
			}

			//! Moving toward the segment endpoint. Check for intersection.
			return moving_sphere_toward_segment_endpoint_intersect( velocity, center_a, center, radius, t, q, a, cmp );
		}
		else
		{
			auto denom = dot_product( normal, velocity );
			if( cmp.greater_than_or_equal( denom * ndist, 0 ) )
			{
				// No intersection as sphere moving parallel to or away from plane 
				return false;
			}
			else
			{
				// Sphere is moving towards the line
				// Use +r in computations if sphere in front of line, else -r 
				auto r = ndist > 0 ? radius : -radius;
				t = (r - ndist) / denom;
				assign( q, center + t * velocity - r * normal );
				return true;
			}
		}

		return false;
	}
}//namespace geometrix;

#endif //GEOMETRIX_MOVING_SPHERE_SEGMENT_INTERSECTION_HPP
