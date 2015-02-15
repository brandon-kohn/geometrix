//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_LINE_INTERSECTION_HPP
#define GEOMETRIX_LINE_INTERSECTION_HPP

#include <geometrix/algorithm/linear_components_intersection.hpp>

#include <boost/typeof/typeof.hpp>

/////////////////////////////////////////////////////////////////////////////
//
// NAMESPACE
//
/////////////////////////////////////////////////////////////////////////////
namespace geometrix {

//! \brief Compute whether the line defined by A->B intersects the specified segment (C->D).
//! Currently implemented to work on types which support fractions (floating-type or rationals).
template <typename Point, typename NumberComparisonPolicy>
inline intersection_type line_intersect( const Point& A, const Point& B, const Point& C, const Point& D, Point& xPoint, const NumberComparisonPolicy& compare )
{       
    intersection_type iType = e_invalid_intersection;

    BOOST_AUTO( denom, get<0>( A ) * (get<1>( D ) - get<1>( C )) +
                       get<0>( B ) * (get<1>( C ) - get<1>( D )) +
                       get<0>( D ) * (get<1>( B ) - get<1>( A )) +
                       get<0>( C ) * (get<1>( A ) - get<1>( B )) );

    //If denom is zeros then segments are parallel.
    if( compare.equals( denom, 0 ) )
    {
        if( is_collinear( A, B, C, compare ) )
            return e_overlapping;
        else
            return e_non_crossing;
    }
        

    BOOST_AUTO( num, -( get<0>( A ) * (get<1>( C ) - get<1>( B )) +
                        get<0>( B ) * (get<1>( A ) - get<1>( C )) +
                        get<0>( C ) * (get<1>( B ) - get<1>( A )) ) );
    
    if( compare.equals( num, 0 ) || compare.equals( num, denom ) )
        iType = e_endpoint;
    
    BOOST_AUTO( t, num / denom );
    
    if( compare.greater_than(t, 0) && compare.less_than(t, 1) )
        iType = e_crossing;
    else if( compare.less_than(t, 0) || compare.greater_than(t, 1) )
        iType = e_non_crossing;
    
    xPoint <<= C + t * (D-C);
    return iType;
}

//! \brief Compute whether the line defined by A->B intersects the specified segment.
//! Currently implemented to work on types which support fractions (floating-type or rationals).
template <typename Segment, typename Point, typename NumberComparisonPolicy>
inline intersection_type line_intersect( const Point& A, const Point& B, const Segment& segment, Point& xPoint, const NumberComparisonPolicy& compare )
{   
    return line_intersect( A, B, get_start( segment ), get_end( segment ), xPoint, compare );
}

}//namespace geometrix;

#endif //GEOMETRIX_LINE_INTERSECTION_HPP
