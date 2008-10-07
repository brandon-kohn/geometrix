//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_BOUNDING_BOX_INTERSECTION_HPP
#define _BOOST_GEOMETRY_BOUNDING_BOX_INTERSECTION_HPP
#pragma once

#include "geometric_concepts.hpp"

namespace boost
{
namespace numeric
{
namespace geometry
{
    //! Bounding box intersection test between two boxes (defined by segment diagonals p1->p2 and p3->p4.)
    template <typename NumberComparisonPolicy, typename Point>
    bool bounding_box_intersection( const Point& p1, const Point& p2, const Point& p3, const Point& p4, const NumberComparisonPolicy& compare ) 
    {
        typedef typename point_traits<Point>::coordinate_type coordinate_type;
        typedef cartesian_access_traits< Point >              access_traits;

        boost::function_requires< Point2DConcept< Point > >();
        boost::function_requires< CartesianCoordinateAccessorConcept< access_traits > >();

        coordinate_type x1 = access_traits::get_x( p1 );
        coordinate_type y1 = access_traits::get_y( p1 );
        coordinate_type x2 = access_traits::get_x( p2 );
        coordinate_type y2 = access_traits::get_y( p2 );
        
        coordinate_type x3 = access_traits::get_x( p3 );
        coordinate_type y3 = access_traits::get_y( p3 );
        coordinate_type x4 = access_traits::get_x( p4 );
        coordinate_type y4 = access_traits::get_y( p4 );

        coordinate_type X1 = (std::min)( x1, x2 );
        coordinate_type Y1 = (std::min)( y1, y2 );
        coordinate_type X2 = (std::max)( x1, x2 );
        coordinate_type Y2 = (std::max)( y1, y2 );
        
        coordinate_type X3 = (std::min)( x3, x4 );
        coordinate_type Y3 = (std::min)( y3, y4 );        
        coordinate_type X4 = (std::max)( x3, x4 );
        coordinate_type Y4 = (std::max)( y3, y4 );        

        return ( compare.greater_than_or_equals( X2, X3 ) &&
                 compare.greater_than_or_equals( X4, X1 ) &&
                 compare.greater_than_or_equals( Y2, Y3 ) &&
                 compare.greater_than_or_equals( Y4, Y1 ) );
    }

    //! Bounding box test between two segments.
    template <typename NumberComparisonPolicy, typename Segment>
    bool bounding_box_intersection( const Segment& s1, const Segment& s2, const NumberComparisonPolicy& compare ) 
    {        
        typedef segment_access_traits< Segment > segment_access;
        return bounding_box_intersection( segment_access::get_start( s1 ), segment_access::get_end( s1 ), segment_access::get_start( s2 ), segment_access::get_end( s2 ), compare );
    }

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_BOUNDING_BOX_INTERSECTION_HPP


