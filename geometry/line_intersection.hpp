//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_LINE_INTERSECTION_HPP
#define _BOOST_GEOMETRY_LINE_INTERSECTION_HPP
#pragma once

#include "linear_components_intersection.hpp"

/////////////////////////////////////////////////////////////////////////////
//
// NAMESPACE
//
/////////////////////////////////////////////////////////////////////////////
namespace boost
{
namespace numeric
{
namespace geometry
{

//! Compute whether the line defined by A->B intersects the specified segment (C->D).
template <typename Point, typename NumberComparisonPolicy>
intersection_type line_intersect( const Point& A, const Point& B, const Point& C, const Point& D, Point& xPoint, const NumberComparisonPolicy& tolCompare )
{    
    NumberComparisonPolicy                         compare;
    typedef Point                                  point_type;
    typedef cartesian_access_traits< Point >       coordinate_access;
    typedef point_traits< Point >::coordinate_type coordinate_type;
    boost::function_requires< CartesianCoordinateAccessorConcept< coordinate_access > >();
    
	coordinate_type t;
	coordinate_type num, denom;
	intersection_type iType = e_invalid_intersection;

    coordinate_type zero = 0;
    coordinate_type one = 1;

	denom = coordinate_access::get_x( A ) * (coordinate_access::get_y( D ) - coordinate_access::get_y( C )) +
			coordinate_access::get_x( B ) * (coordinate_access::get_y( C ) - coordinate_access::get_y( D )) +
			coordinate_access::get_x( D ) * (coordinate_access::get_y( B ) - coordinate_access::get_y( A )) +
			coordinate_access::get_x( C ) * (coordinate_access::get_y( A ) - coordinate_access::get_y( B ));

	//If denom is zeros then segments are parallel.
	if( tolCompare.equals( denom, zero ) )
	{
		if( is_collinear( A, B, C, tolCompare ) )
        {
            return e_overlapping;
        }
        else
        {
            return e_non_crossing;
        }
	}

    num = -( coordinate_access::get_x( A ) * (coordinate_access::get_y( C ) - coordinate_access::get_y( B )) +
			 coordinate_access::get_x( B ) * (coordinate_access::get_y( A ) - coordinate_access::get_y( C )) +
			 coordinate_access::get_x( C ) * (coordinate_access::get_y( B ) - coordinate_access::get_y( A )) );
	
    if( tolCompare.equals( num, zero ) || tolCompare.equals( num, denom ) )
	{
		iType = e_endpoint;
	}
	t = num / denom;
	
	if( tolCompare.greater_than( t, zero ) && tolCompare.less_than( t, one ) )
	{
		iType = e_crossing;
	}
	else if( tolCompare.less_than( t, zero ) || tolCompare.greater_than( t, one ) )
	{
		iType = e_non_crossing;
	}

	coordinate_type x = (coordinate_access::get_x( C ) + t * (coordinate_access::get_x( D ) - coordinate_access::get_x( C )));
	coordinate_type y = (coordinate_access::get_y( C ) + t * (coordinate_access::get_y( D ) - coordinate_access::get_y( C )));
    xPoint = coordinate_access::construct< point_type >( x, y );
	return iType;
}

//! Compute whether the line defined by A->B intersects the specified segment.
template <typename Segment, typename Point, typename NumberComparisonPolicy>
intersection_type line_intersect( const Point& A, const Point& B, const Segment& segment, Point& xPoint, const NumberComparisonPolicy& tolCompare )
{    
    typedef Point                                  point_type;
    typedef segment_access_traits< Segment >       segment_access;
    boost::function_requires< SegmentAccessorConcept< segment_access > >();

	const point_type& C = segment_access::get_start( segment );
	const point_type& D = segment_access::get_end( segment );

    return line_intersect( A, B, C, D, xPoint, tolCompare );
}

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_LINE_INTERSECTION_HPP
