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
#include <geometrix/primitive/line.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/algebra/expression.hpp>
#include <geometrix/algebra/dot_product.hpp>
#include <geometrix/algorithm/intersection/moving_sphere_plane_intersection.hpp>
#include <geometrix/algorithm/euclidean_distance.hpp>

#include <cstdint>

/////////////////////////////////////////////////////////////////////////////
//
// NAMESPACE
//
/////////////////////////////////////////////////////////////////////////////
namespace geometrix {

	struct moving_sphere_segment_intersection_result
	{
		typedef void (moving_sphere_segment_intersection_result::*bool_type)() const;

		moving_sphere_segment_intersection_result()
		{}

		moving_sphere_segment_intersection_result( bool isIntersecting, bool isAlreadyOnLine, bool isEndpoint, bool isPenetrating, bool isMovingAway )
			: result( (isIntersecting ? e_is_intersecting : 0) | (isAlreadyOnLine ? e_is_on_line_at_start : 0) | (isEndpoint ? e_is_endpoint : 0) | (isPenetrating ? e_is_penetrating : 0) | (isMovingAway ? e_is_moving_away : 0) )
		{}

		bool is_intersecting() const { return (result & e_is_intersecting) != 0; }
		bool is_on_line_at_start() const { return (result & e_is_on_line_at_start) != 0; }
		bool is_endpoint() const { return (result & e_is_endpoint) != 0; }
		bool is_penetrating() const { return (result & e_is_penetrating) != 0; }
		bool is_touching() const { return is_intersecting() && !is_penetrating(); }
		bool is_moving_away() const { return (result & e_is_moving_away) != 0; }

		operator bool_type() const {
			return is_intersecting() ? &moving_sphere_segment_intersection_result::bool_type_mfn : 0;
		}

	private:

		void bool_type_mfn() const{}

		enum Flags
		{
			e_is_intersecting = 1
		  , e_is_on_line_at_start = 2
		  , e_is_endpoint = 4
		  , e_is_penetrating = 8
		  , e_is_moving_away = 16
		};

		std::uint32_t result{0};

	};

	namespace moving_sphere_segment_intersection_detail {

		template <typename Vector1, typename SpherePoint, typename Radius, typename Point, typename ArithmeticType, typename SegmentPoint, typename NumberComparisonPolicy>
		inline bool moving_sphere_toward_segment_endpoint_intersect( const Vector1& velocity, const SpherePoint& center, const Radius& radius, ArithmeticType &t, Point& q, const SegmentPoint& a, const NumberComparisonPolicy &cmp )
		{
			typedef vector<ArithmeticType, dimension_of<Point>::value> vector_t;
			typedef point<ArithmeticType, dimension_of<Point>::value> point_t;

			vector_t center_a = center - a;
			auto speed = magnitude( velocity );
			if( cmp.equals( speed, 0 ) )
			{
				t = 0;
				return false;
			}

			vector_t direction = velocity / speed;
			auto center_a_dot_direction = dot_product( center_a, direction );
			auto d_minus_r_sqrd = dot_product( center_a, center_a ) - radius * radius;

			// Exit if r’s origin outside s (d_minus_r_sqrd > 0) and r pointing away from s (center_a_dot_direction > 0)
			//! TODO: This might be redundant here as the direction is established already... right?
			if( cmp.greater_than_or_equal( d_minus_r_sqrd, 0 ) && cmp.greater_than_or_equal( center_a_dot_direction, 0 ) )//! Changed to >= on both tests to be optimistic on endpoints near the radius distance.
				return false;

			auto discr = center_a_dot_direction*center_a_dot_direction - d_minus_r_sqrd;

			// A negative discriminant corresponds to ray missing sphere 
			if( discr < 0 )
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
	}//! namespace moving_sphere_segment_intersection_detail;

	// Intersect sphere s with movement vector v with segment seg. If intersecting 
	// return time t of collision and point q at which sphere hits the segment.
	//! Precondition - velocity should be non-zero.
	template <typename Sphere, typename Vector, typename Segment, typename ArithmeticType, typename Point, typename NumberComparisonPolicy>
	inline moving_sphere_segment_intersection_result moving_sphere_segment_intersection( const Sphere& s, const Vector& velocity, const Segment& seg, ArithmeticType &t, Point& q, const NumberComparisonPolicy& cmp )
	{
		using namespace moving_sphere_segment_intersection_detail;
			
		typedef vector<ArithmeticType, dimension_of<Point>::value> vector_t;
		line<typename point_type_of<Segment>::type, vector_t> l( seg );
		
		//! Check if the intersection happens on the line formed by the segment.
		auto sphere_plane_result = moving_sphere_plane_intersection( s, velocity, l, t, q, cmp );
		if( !sphere_plane_result.is_intersecting() )
			return moving_sphere_segment_intersection_result( false, false, false, sphere_plane_result.is_penetrating(), sphere_plane_result.is_moving_away() );
				
		//! There is an intersection on the line.. check if the point is inside the segment.
		auto a = get_start( seg );
		auto b = get_end( seg );		
		bool alreadyOnLine = cmp.equals( t, 0 );
		if( is_between( a, b, q, true, cmp ) )
			return moving_sphere_segment_intersection_result( true, alreadyOnLine, false, sphere_plane_result.is_penetrating(), sphere_plane_result.is_moving_away() );//! we're done.
				
		//! Intersection is outside of segment. Find the side closest to q.
		auto q_distance_to_start_sqrd = point_point_distance_sqrd( q, a );
		auto q_distance_to_end_sqrd = point_point_distance_sqrd( q, b );

		auto center = get_center( s );
		auto radius = get_radius( s );
		
		//! Moving toward a segment end point. Check for intersection.
		if( moving_sphere_toward_segment_endpoint_intersect( velocity, center, radius, t, q, (q_distance_to_end_sqrd < q_distance_to_start_sqrd ? b : a), cmp ) )
			return moving_sphere_segment_intersection_result( true, alreadyOnLine, true, sphere_plane_result.is_penetrating(), sphere_plane_result.is_moving_away() );
		
		return moving_sphere_segment_intersection_result( false, alreadyOnLine, false, sphere_plane_result.is_penetrating(), sphere_plane_result.is_moving_away() );
	}
}//namespace geometrix;

#endif //GEOMETRIX_MOVING_SPHERE_SEGMENT_INTERSECTION_HPP
