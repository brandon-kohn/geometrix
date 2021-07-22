//
//! Copyright © 2008-2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_SEGMENT_TRIANGLE_INTERSECTION_HPP
#define GEOMETRIX_SEGMENT_TRIANGLE_INTERSECTION_HPP

#include <geometrix/primitive/point_traits.hpp>
#include <geometrix/primitive/segment_traits.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/algebra/expression.hpp>
#include <geometrix/arithmetic/vector/perp.hpp>
#include <geometrix/algebra/dot_product.hpp>
#include <geometrix/algorithm/point_in_polygon.hpp>
#include <geometrix/algorithm/segment_intersection.hpp>
#include <geometrix/utility/utilities.hpp>

#include <boost/container/flat_set.hpp>

/////////////////////////////////////////////////////////////////////////////
//
// NAMESPACE
//
/////////////////////////////////////////////////////////////////////////////
namespace geometrix {

//! \brief Compute whether the segment defined by A->B intersects the triangle defined by t0, t1, t2.
template <typename PointA, typename PointB, typename PointT0, typename PointT1, typename PointT2, typename PointX, typename NumberComparisonPolicy>
inline std::size_t segment_triangle_intersect( const PointA& A, const PointB& B, const PointT0& t0, const PointT1 t1, const PointT2& t2, PointX* xPoints, const NumberComparisonPolicy& cmp )
{
	BOOST_CONCEPT_ASSERT( (Point2DConcept<PointA>) );
	BOOST_CONCEPT_ASSERT( (Point2DConcept<PointB>) );
	BOOST_CONCEPT_ASSERT( (Point2DConcept<PointT0>) );
	BOOST_CONCEPT_ASSERT( (Point2DConcept<PointT1>) );
	BOOST_CONCEPT_ASSERT( (Point2DConcept<PointT2>) );
	BOOST_CONCEPT_ASSERT( (Point2DConcept<PointX>) );
	BOOST_CONCEPT_ASSERT( (NumberComparisonPolicyConcept<NumberComparisonPolicy>) );
		
	boost::container::flat_set<PointX, lexicographical_comparer<NumberComparisonPolicy>> iPoints( cmp );

	//! First check if the points are interior.
	bool aInside = point_in_triangle( A, t0, t1, t2, cmp );
	bool bInside = point_in_triangle( B, t0, t1, t2, cmp );
	if( aInside )
	{		
		if( bInside )
		{
			xPoints[0] = A;
			xPoints[1] = B;
			return 2;
		}

		iPoints.insert( construct<PointX>( A ) );
	}
	else if( bInside )
		iPoints.insert( construct<PointX>( B ) );
		
	PointX nXPoints[2];
	auto iType = segment_segment_intersection( A, B, t0, t1, nXPoints, cmp );
	if( iType == e_crossing || iType == e_endpoint )
		iPoints.insert( nXPoints[0] );
	else if( iType == e_overlapping )
	{
		iPoints.insert( nXPoints[0] );
		iPoints.insert( nXPoints[1] );
	}

	iType = segment_segment_intersection( A, B, t1, t2, nXPoints, cmp );
	if( iType == e_crossing || iType == e_endpoint )
		iPoints.insert( nXPoints[0] );
	else if( iType == e_overlapping )
	{
		iPoints.insert( nXPoints[0] );
		iPoints.insert( nXPoints[1] );
	}

	iType = segment_segment_intersection( A, B, t2, t0, nXPoints, cmp );
	if( iType == e_crossing || iType == e_endpoint )
		iPoints.insert( nXPoints[0] );
	else if( iType == e_overlapping )
	{
		iPoints.insert( nXPoints[0] );
		iPoints.insert( nXPoints[1] );
	}

	if( iPoints.empty() )
		return 0;
		
	xPoints[0] = *iPoints.begin();

	if( iPoints.size() > 1 )
	{
		xPoints[1] = *(iPoints.begin() + 1);
		return 2;
	}

	GEOMETRIX_ASSERT( iPoints.size() < 3 );

	return 1;
}

//! \brief Compute whether the segment defined by A->B intersects the triangle defined by t0, t1, t2.
template <typename PointA, typename PointB, typename PointT0, typename PointT1, typename PointT2>
inline bool segment_triangle_intersect( const PointA& A, const PointB& B, const PointT0& t0, const PointT1 t1, const PointT2& t2)
{
    BOOST_CONCEPT_ASSERT((Point2DConcept<PointA>));
    BOOST_CONCEPT_ASSERT((Point2DConcept<PointB>));
    BOOST_CONCEPT_ASSERT((Point2DConcept<PointT0>));
    BOOST_CONCEPT_ASSERT((Point2DConcept<PointT1>));
	BOOST_CONCEPT_ASSERT((Point2DConcept<PointT2>));

	typedef typename select_arithmetic_type_from_sequences<PointA, PointT0>::type arithmetic_type;
	typedef vector<arithmetic_type, 2> vector2;
	using area_t = decltype(std::declval<arithmetic_type>()*std::declval<arithmetic_type>());

	auto pmin = (std::numeric_limits<area_t>::max)(), pmax = -(std::numeric_limits<area_t>::max)(), tmin = (std::numeric_limits<area_t>::max)(), tmax = -(std::numeric_limits<area_t>::max)();

	vector2 n = left_normal<vector2>( B - A );
	{
		auto nt0 = dot_product( n, as_vector( t0 ) );
		auto nt1 = dot_product( n, as_vector( t1 ) );
		auto nt2 = dot_product( n, as_vector( t2 ) );
		
		tmin = (std::min)( tmin, nt0 );
		tmin = (std::min)( tmin, nt1 );
		tmin = (std::min)( tmin, nt2 );

		tmax = (std::max)( tmax, nt0 );
		tmax = (std::max)( tmax, nt1 );
		tmax = (std::max)( tmax, nt2 );

		auto nA = dot_product( n, as_vector( A ) );
		auto nB = dot_product( n, as_vector( B ) );

		pmin = (std::min)( pmin, nA );
		pmin = (std::min)( pmin, nB );

		pmax = (std::max)( pmax, nA );
		pmax = (std::max)( pmax, nB );

		if( tmin > pmax || tmax < pmin )
			return false;
	}
	
	pmin = (std::numeric_limits<area_t>::max)(), pmax = -(std::numeric_limits<area_t>::max)(), tmin = (std::numeric_limits<area_t>::max)(), tmax = -(std::numeric_limits<area_t>::max)();
	n = left_normal<vector2>(t1 - t0);
	{
		auto nt0 = dot_product( n, as_vector( t0 ) );
		auto nt1 = dot_product( n, as_vector( t1 ) );
		auto nt2 = dot_product( n, as_vector( t2 ) );

		tmin = (std::min)( tmin, nt0 );
		tmin = (std::min)( tmin, nt1 );
		tmin = (std::min)( tmin, nt2 );

		tmax = (std::max)( tmax, nt0 );
		tmax = (std::max)( tmax, nt1 );
		tmax = (std::max)( tmax, nt2 );

		auto nA = dot_product( n, as_vector( A ) );
		auto nB = dot_product( n, as_vector( B ) );

		pmin = (std::min)( pmin, nA );
		pmin = (std::min)( pmin, nB );

		pmax = (std::max)( pmax, nA );
		pmax = (std::max)( pmax, nB );

		if( tmin > pmax || tmax < pmin )
			return false;
	}
	
	pmin = (std::numeric_limits<area_t>::max)(), pmax = -(std::numeric_limits<area_t>::max)(), tmin = (std::numeric_limits<area_t>::max)(), tmax = -(std::numeric_limits<area_t>::max)();
	n = left_normal<vector2>(t2 - t1);
	{
		auto nt0 = dot_product( n, as_vector( t0 ) );
		auto nt1 = dot_product( n, as_vector( t1 ) );
		auto nt2 = dot_product( n, as_vector( t2 ) );

		tmin = (std::min)( tmin, nt0 );
		tmin = (std::min)( tmin, nt1 );
		tmin = (std::min)( tmin, nt2 );

		tmax = (std::max)( tmax, nt0 );
		tmax = (std::max)( tmax, nt1 );
		tmax = (std::max)( tmax, nt2 );

		auto nA = dot_product( n, as_vector( A ) );
		auto nB = dot_product( n, as_vector( B ) );

		pmin = (std::min)( pmin, nA );
		pmin = (std::min)( pmin, nB );

		pmax = (std::max)( pmax, nA );
		pmax = (std::max)( pmax, nB );

		if( tmin > pmax || tmax < pmin )
			return false;
	}

	pmin = (std::numeric_limits<area_t>::max)(), pmax = -(std::numeric_limits<area_t>::max)(), tmin = (std::numeric_limits<area_t>::max)(), tmax = -(std::numeric_limits<area_t>::max)();
	n = left_normal<vector2>(t0 - t2);
	{
		auto nt0 = dot_product( n, as_vector( t0 ) );
		auto nt1 = dot_product( n, as_vector( t1 ) );
		auto nt2 = dot_product( n, as_vector( t2 ) );

		tmin = (std::min)( tmin, nt0 );
		tmin = (std::min)( tmin, nt1 );
		tmin = (std::min)( tmin, nt2 );

		tmax = (std::max)( tmax, nt0 );
		tmax = (std::max)( tmax, nt1 );
		tmax = (std::max)( tmax, nt2 );

		auto nA = dot_product( n, as_vector( A ) );
		auto nB = dot_product( n, as_vector( B ) );

		pmin = (std::min)( pmin, nA );
		pmin = (std::min)( pmin, nB );

		pmax = (std::max)( pmax, nA );
		pmax = (std::max)( pmax, nB );

		if( tmin > pmax || tmax < pmin )
			return false;
	}

	return true;
}

//! \brief Compute whether the segment defined by A->B intersects the triangle defined by t0, t1, t2.
template <typename Segment, typename PointT0, typename PointT1, typename PointT2>
inline bool segment_triangle_intersect( const Segment& segment, const PointT0& t0, const PointT1& t1, const PointT2& t2 )
{   
	return segment_triangle_intersect( get_start( segment ), get_end( segment ), t0, t1, t2 );
}

}//namespace geometrix;

#endif //GEOMETRIX_SEGMENT_TRIANGLE_INTERSECTION_HPP
