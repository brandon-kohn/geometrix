//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GENERATIVE_GEOMETRY_SEGMENT_INTERSECTION_HPP
#define GENERATIVE_GEOMETRY_SEGMENT_INTERSECTION_HPP
#pragma once

#include "linear_components_intersection.hpp"
#include "cartesian_access_traits.hpp"
#include "products.hpp"
#include "bounding_box_intersection.hpp"

/////////////////////////////////////////////////////////////////////////////
//
// FORWARD DECLARATION
//
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
// NAMESPACE
//
/////////////////////////////////////////////////////////////////////////////
namespace generative
{
namespace numeric
{
namespace geometry
{

template <typename Point, typename NumberComparisonPolicy>
intersection_type parallel_intersection( const Point& A, const Point& B, const Point& C, const Point& D, Point* xPoint, const NumberComparisonPolicy& tolCompare )
{
    typedef cartesian_access_traits< Point >       coordinate_access;
    typedef point_traits< Point >::coordinate_type coordinate_type;
    boost::function_requires< CartesianCoordinateAccessorConcept< coordinate_access > >();

	if( !is_collinear( A, B, C, tolCompare ) )
	{
		return e_non_crossing;
	}

	bool isBetweenABC = is_between( A, B, C, false, tolCompare );
	bool isBetweenABD = is_between( A, B, D, false, tolCompare );

	if ( isBetweenABC && isBetweenABD )
	{
		if(xPoint)
		{
			xPoint[0] = C;
			xPoint[1] = D;
		}

		return e_overlapping;
	}

	bool isBetweenCDA = is_between( C, D, A, false, tolCompare );
	bool isBetweenCDB = is_between( C, D, B, false, tolCompare );

	if ( isBetweenCDA && isBetweenCDB )
	{
		if(xPoint)
		{
			xPoint[0] = A;
			xPoint[1] = B;
		}

		return e_overlapping;
	}

	if( isBetweenABC && isBetweenCDB )
	{
		if(xPoint)
		{
			xPoint[0] = C;
			xPoint[1] = B;
		}

		return e_overlapping;
	}

	if( isBetweenABC && isBetweenCDA )
	{
		if(xPoint)
		{
			xPoint[0] = C;
			xPoint[1] = A;
		}

		return e_overlapping;
	}

	if( isBetweenABD && isBetweenCDB )
	{
		if(xPoint)
		{
			xPoint[0] = D;
			xPoint[1] = B;
		}

		return e_overlapping;
	}

	if( isBetweenABD && isBetweenCDA )
	{
		if(xPoint)
		{
			xPoint[0] = D;
			xPoint[1] = A;
		}

		return e_overlapping;
	}

	bool originEqualsC = numeric_sequence_equals( A, C, tolCompare );
	bool destinationEqualsD = numeric_sequence_equals( B, D, tolCompare );
	bool originEqualsD = numeric_sequence_equals( A, D, tolCompare );
	bool destinationEqualsC = numeric_sequence_equals( B, C, tolCompare );
	if( (originEqualsC && destinationEqualsD) || (originEqualsD && destinationEqualsC) )
	{
		if(xPoint)
		{
			xPoint[0] = A;
			xPoint[1] = B;
		}

		return e_overlapping;
	}

	if(originEqualsC)
	{
		if(xPoint)
		{
			xPoint[0] = A;
		}

		return e_endpoint;
	}

	if(originEqualsD)
	{
		if(xPoint)
		{
			xPoint[0] = A;
		}
		return e_endpoint;
	}

	if(destinationEqualsC)
	{
		if(xPoint)
		{
			xPoint[0] = B;
		}
		return e_endpoint;
	}

	if(destinationEqualsD)
	{
		if(xPoint)
		{
			xPoint[0] = B;
		}
		return e_endpoint;
	}

	return e_non_crossing;
}

template <typename Point, typename NumberComparisonPolicy>
intersection_type calculate_intersection( const Point& A, const Point& B, const Point& C, const Point& D, Point* xPoint, const NumberComparisonPolicy& tolCompare )
{    
    NumberComparisonPolicy                         compare;
    typedef Point                                  point_type;
    typedef cartesian_access_traits< point_type >  coordinate_access;
    typedef point_traits< Point >::coordinate_type coordinate_type;

    boost::function_requires< CartesianCoordinateAccessorConcept< coordinate_access > >();

    //! first invoke bounding box filter
//     if( !bounding_box_intersection<NumberComparisonPolicy>( A, B, C, D, tolCompare ) )
//     {
//         return e_non_crossing;
//     }

	coordinate_type s, t;
	coordinate_type num, denom;
	intersection_type iType = e_invalid_intersection;

    coordinate_type zero = 0;
    coordinate_type one = 1;

	denom = coordinate_access::get<0>( A ) * (coordinate_access::get<1>( D ) - coordinate_access::get<1>( C )) +
			coordinate_access::get<0>( B ) * (coordinate_access::get<1>( C ) - coordinate_access::get<1>( D )) +
			coordinate_access::get<0>( D ) * (coordinate_access::get<1>( B ) - coordinate_access::get<1>( A )) +
			coordinate_access::get<0>( C ) * (coordinate_access::get<1>( A ) - coordinate_access::get<1>( B ));

	//If denom is zeros then segments are parallel.
	if( tolCompare.equals( denom, zero ) )
	{
		return parallel_intersection( A, B, C, D, xPoint, tolCompare );
	}

	num = coordinate_access::get<0>( A ) * (coordinate_access::get<1>( D ) - coordinate_access::get<1>( C )) +
		  coordinate_access::get<0>( C ) * (coordinate_access::get<1>( A ) - coordinate_access::get<1>( D )) +
		  coordinate_access::get<0>( D ) * (coordinate_access::get<1>( C ) - coordinate_access::get<1>( A ));
	if( tolCompare.equals(num, zero ) || tolCompare.equals( num, denom ) )
	{
		iType = e_endpoint;
	}
	s = num / denom;

	num = -( coordinate_access::get<0>( A ) * (coordinate_access::get<1>( C ) - coordinate_access::get<1>( B )) +
			 coordinate_access::get<0>( B ) * (coordinate_access::get<1>( A ) - coordinate_access::get<1>( C )) +
			 coordinate_access::get<0>( C ) * (coordinate_access::get<1>( B ) - coordinate_access::get<1>( A )) );
	if( tolCompare.equals( num, zero ) || tolCompare.equals( num, denom ) )
	{
		iType = e_endpoint;
	}
	t = num / denom;

	if( tolCompare.greater_than( s, zero ) &&
        tolCompare.less_than( s, one )   &&
        tolCompare.greater_than( t, zero ) &&
        tolCompare.less_than( t, one ) )
	{
		iType = e_crossing;
	}
	else if( tolCompare.less_than(s, zero )       ||
             tolCompare.greater_than( s, one ) ||
             tolCompare.less_than( t, zero )     ||
             tolCompare.greater_than( t, one ) )
	{
		iType = e_non_crossing;
	}

	coordinate_type x = (coordinate_access::get<0>( A ) + s * (coordinate_access::get<0>( B ) - coordinate_access::get<0>( A )));
	coordinate_type y = (coordinate_access::get<1>( A ) + s * (coordinate_access::get<1>( B ) - coordinate_access::get<1>( A )));
    xPoint[0] = construction_traits< point_type >::construct( x, y );

	return iType;
}

//! Compute whether the segment defined by A->B intersects the specified segment.
template <typename Segment, typename Point, typename NumberComparisonPolicy>
intersection_type calculate_intersection( const Segment& segment1, const Segment& segment2, Point* xPoints, const NumberComparisonPolicy& tolCompare )
{    
    typedef Point                                  point_type;
    typedef segment_access_traits< Segment >       segment_access;
    boost::function_requires< SegmentAccessorConcept< segment_access > >();

    const point_type& A = segment_access::get_start( segment1 );
    const point_type& B = segment_access::get_end( segment1 );
	const point_type& C = segment_access::get_start( segment2 );
	const point_type& D = segment_access::get_end( segment2 );

    return calculate_intersection( A, B, C, D, xPoints, tolCompare );
}

}}}//namespace generative::numeric::geometry;

#endif //GENERATIVE_GEOMETRY_SEGMENT_INTERSECTION_HPP
