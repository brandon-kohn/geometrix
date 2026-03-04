//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_IS_SEGMENT_IN_RANGE_HPP
#define GEOMETRIX_IS_SEGMENT_IN_RANGE_HPP

#include <geometrix/primitive/point_traits.hpp>
#include <geometrix/primitive/segment_traits.hpp>
#include <geometrix/tensor/vector_traits.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/algebra/exterior_product.hpp>
#include <geometrix/algebra/dot_product.hpp>
#include <geometrix/algebra/expression.hpp>
#include <geometrix/algorithm/line_intersection.hpp>
#include <geometrix/algorithm/intersection/ray_segment_intersection.hpp>
#include <geometrix/algorithm/orientation/point_segment_orientation.hpp>

namespace geometrix {
	
	template <typename Vector1, typename Vector2, typename Point1, typename Point2, typename Point3, typename NumberComparisonPolicy>
	BOOST_FORCEINLINE bool is_segment_in_range_2d( const Point1& segStart,
		const Point2&                                            segEnd,
		const Vector1&                                           lo,
		const Vector2&                                           hi,
		const Point3&                                            origin,
		const NumberComparisonPolicy&                            cmp )
	{
		using namespace geometrix;

		BOOST_CONCEPT_ASSERT( (Vector2DConcept<Vector1>));
		BOOST_CONCEPT_ASSERT( (Vector2DConcept<Vector2>));
		BOOST_CONCEPT_ASSERT( (Point2DConcept<Point1>));
		BOOST_CONCEPT_ASSERT( (Point2DConcept<Point2>));
		BOOST_CONCEPT_ASSERT( (Point2DConcept<Point3>));
		BOOST_CONCEPT_ASSERT( (NumberComparisonPolicyConcept<NumberComparisonPolicy>));

		using length_t = typename select_arithmetic_type_from_sequences<Point1, Point2, Point3>::type;
		using area_t = decltype( length_t() * length_t() );
		using vector_t = vector<length_t, 2>;

		vector_t vA = segStart - origin;
		vector_t vB = segEnd - origin;

#ifdef GEOMETRIX_DEBUG_SEGMENT_IN_RANGE
		using point_t = point<length_t, 2>;
		using segment_t = segment<point_t>;
		auto seg = segment_t( point_t( segStart ), point_t( segEnd ) );
		auto vASeg = segment_t( point_t( origin ), point_t( segStart ) );
		auto vBSeg = segment_t( point_t( origin ), point_t( segEnd ) );
		auto d2 = std::max( magnitude( vA ), magnitude( vB ) );
		auto segLo = segment_t( point_t( origin ), point_t( origin + d2 * normalize( lo ) ) );
		auto segHi = segment_t( point_t( origin ), point_t( origin + d2 * normalize( hi ) ) );
#endif

		const auto detLoA = exterior_product_area( lo, vA );
		const auto detHiA = exterior_product_area( hi, vA );
		const auto detLoB = exterior_product_area( lo, vB );
		const auto detHiB = exterior_product_area( hi, vB );

		const auto zero = constants::zero<area_t>();

		auto in_range = [&]( const auto& detLo, const auto& detHi )
		{
			return cmp.greater_than_or_equal( detLo, zero ) && cmp.less_than_or_equal( detHi, zero );
		};

		//! Either endpoint inside the cone?
		if( in_range( detLoA, detHiA ) || in_range( detLoB, detHiB ) )
			return true;

		//! Both endpoints strictly outside on the same side?
		if( ( cmp.greater_than( detHiA, zero ) && cmp.greater_than( detHiB, zero ) ) || ( cmp.less_than( detLoA, zero ) && cmp.less_than( detLoB, zero ) ) )
			return false;

		//! Special case: one endpoint on hi-ray and the other on lo-ray
		//! (Use cmp-based zero comparisons)
		if( cmp.equals( detHiA, zero ) && cmp.equals( detLoB, zero ) )
			return get_orientation( segStart, segEnd, origin, cmp ) != oriented_left;

		if( cmp.equals( detHiB, zero ) && cmp.equals( detLoA, zero ) )
			return get_orientation( segStart, segEnd, origin, cmp ) != oriented_right;

		//! Otherwise, it intersects the cone iff it intersects either boundary ray (or hits the apex,
		//! depending on how ray_segment_intersection treats origin-contact).
		return ray_segment_intersection( origin, lo, segStart, segEnd, cmp ) != e_non_crossing || ray_segment_intersection( origin, hi, segStart, segEnd, cmp ) != e_non_crossing;
	}

	//! Test if a segment intersects the cone defined by two rays from a common origin.
	template <typename Point1, typename Point2, typename Vector1, typename Vector2, typename Point3>
	BOOST_FORCEINLINE bool is_segment_in_range_2d_direct_cmp( const Point1& a, const Point2& b, const Vector1& lo, const Vector2& hi, const Point3& origin )
	{
		using namespace geometrix;
		static direct_comparison_policy directCmp;
		return is_segment_in_range_2d( a, b, lo, hi, origin, directCmp );
	}
	
	//! Test if a segment intersects the cone defined by two rays from a common origin.
	template <typename Vector1, typename Vector2, typename Segment, typename Point, typename NumberComparisonPolicy>
	BOOST_FORCEINLINE bool is_segment_in_range_2d(
		const Segment&                segment,
		const Vector1&                lo,
		const Vector2&                hi,
		const Point&                  origin,
		const NumberComparisonPolicy& cmp )
	{
		using namespace geometrix;
		return is_segment_in_range_2d( get_start( segment ), get_end( segment ), lo, hi, origin, cmp );
	}

	//! Test if a segment intersects the cone defined by two rays from a common origin.
	template <typename Vector1, typename Vector2, typename Segment, typename Point>
	BOOST_FORCEINLINE bool is_segment_in_range_2d_direct_cmp( const Segment& segment, const Vector1& lo, const Vector2& hi, const Point& origin )
	{
		direct_comparison_policy directCmp;
		return is_segment_in_range_2d( segment, lo, hi, origin, directCmp );
	}

	//! Backward-compatible overload that defaults to direct comparison.
	template <typename Vector1, typename Vector2, typename Segment, typename Point>
	BOOST_FORCEINLINE bool is_segment_in_range_2d( const Segment& segment, const Vector1& lo, const Vector2& hi, const Point& origin )
	{
		return is_segment_in_range_2d_direct_cmp(segment, lo, hi, origin );
	}

	//! Test if a segment intersects the cone defined by two rays from a common origin.
	template <typename Vector1, typename Vector2, typename Segment, typename Point, typename NumberComparisonPolicy>
	BOOST_FORCEINLINE bool is_segment_in_range_2d(
		const Segment&                segment,
		const Vector1&                lo,
		const Vector2&                hi,
		const Point&                  origin,
		Point*                        xPoints,
		const NumberComparisonPolicy& cmp )
	{
		using namespace geometrix;

		BOOST_CONCEPT_ASSERT( (Vector2DConcept<Vector1>));
		BOOST_CONCEPT_ASSERT( (Vector2DConcept<Vector2>));
		BOOST_CONCEPT_ASSERT( (Point2DConcept<Point>)); // <-- FIXED
		BOOST_CONCEPT_ASSERT( (NumberComparisonPolicyConcept<NumberComparisonPolicy>));

		using segment_point_type = typename geometric_traits<Segment>::point_type;

		//! Consider including Vector1/Vector2 here if they may carry wider scalar types:
		//! using length_t = typename select_arithmetic_type_from_sequences<segment_point_type, Point, Vector1, Vector2>::type;
		using length_t = typename select_arithmetic_type_from_sequences<segment_point_type, Point>::type;

		using area_t = decltype( length_t() * length_t() );
		using vector_t = vector<length_t, 2>;

		const auto zero = constants::zero<area_t>();

		const Point& A = get_start( segment );
		const Point& B = get_end( segment );

		const vector_t vA = A - origin;
		const vector_t vB = B - origin;

		auto in_range_vec = [&]( const vector_t& v ) -> bool
		{
			const auto detLo = exterior_product_area( lo, v );
			const auto detHi = exterior_product_area( hi, v );
			return cmp.greater_than_or_equal( detLo, zero ) && cmp.less_than_or_equal( detHi, zero );
		};

		auto in_range_pt = [&]( const Point& p ) -> bool
		{
			return in_range_vec( p - origin );
		};

		const bool startIn = in_range_vec( vA );
		const bool endIn = in_range_vec( vB );

		//! Both endpoints inside => whole segment is inside the cone.
		if( startIn && endIn )
		{
			xPoints[0] = A;
			xPoints[1] = B;
			return true;
		}

		//! Collect candidate points that lie on the segment AND are in/on the cone.
		Point candidates[6];
		int   n = 0;

		auto push_if_in = [&]( const Point& p )
		{
			//! max unique pushes is bounded; duplicates are harmless for min/max selection
			if( in_range_pt( p ) )
				candidates[n++] = p;
		};

		if( startIn )
			push_if_in( A );
		if( endIn )
			push_if_in( B );

		//! Intersections with boundary rays
		Point    loPts[2], hiPts[2];
		length_t t; //! API requires it; not used for selection here

		const auto loType = ray_segment_intersection( origin, normalize( lo ), segment, t, loPts, cmp );
		const auto hiType = ray_segment_intersection( origin, normalize( hi ), segment, t, hiPts, cmp );

		auto add_from_ray = [&]( intersection_type ty, Point pts[2] )
		{
			if( ty == e_crossing || ty == e_endpoint )
			{
				push_if_in( pts[0] );
			}
			else if( ty == e_overlapping )
			{
				//! Overlap returns two endpoints of the overlapping portion
				push_if_in( pts[0] );
				push_if_in( pts[1] );
			}
		};

		add_from_ray( loType, loPts );
		add_from_ray( hiType, hiPts );

		if( n == 0 )
			return false;

		if( n == 1 )
		{
			xPoints[0] = candidates[0];
			xPoints[1] = candidates[0];
			return true;
		}

		//! Pick the two extreme points along the segment as the clipped interval.
		const vector_t d = B - A;

		auto proj = [&]( const Point& p )
		{
			//! Parameter ordering along the segment is monotone with dot(d, p-A) when d != 0.
			return dot_product( d, p - A );
		};

		int  iMin = 0, iMax = 0;
		auto sMin = proj( candidates[0] );
		auto sMax = sMin;

		for( int i = 1; i < n; ++i )
		{
			const auto s = proj( candidates[i] );
			if( cmp.less_than( s, sMin ) )
			{
				sMin = s;
				iMin = i;
			}
			if( cmp.greater_than( s, sMax ) )
			{
				sMax = s;
				iMax = i;
			}
		}

		xPoints[0] = candidates[iMin];
		xPoints[1] = candidates[iMax];
		return true;
	}

}//namespace geometrix;

#endif //GEOMETRIX_IS_SEGMENT_IN_RANGE_HPP
