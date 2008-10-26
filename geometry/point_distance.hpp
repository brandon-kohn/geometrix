//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_POINT_DISTANCE_HPP
#define _BOOST_GEOMETRY_POINT_DISTANCE_HPP
#pragma once

#include "number_comparison_policy.hpp"
#include "point_traits.hpp"
#include "vector_traits.hpp"
#include "reference_frame_traits.hpp"
#include "indexed_access_traits.hpp"

namespace boost
{
namespace numeric
{
namespace geometry
{

//! \brief Calculate the distance between two points in any dimensional space for a cartesian reference frame.
template <typename Point>
inline typename point_traits<Point>::coordinate_type distance_to_point_squared( const Point& A, 
                                                                                const Point& B,
                                                                                typename boost::enable_if<
                                                                                boost::is_same< 
                                                                                    typename point_traits<Point>::dimension_type,
                                                                                    dimension_traits<2> >
                                                                                >::type* dummy = 0 )
{
    typedef cartesian_access_traits< Point > access_traits;
    typename point_traits<Point>::coordinate_type dx = access_traits::get<0>( B ) - access_traits::get<0>( A );
    typename point_traits<Point>::coordinate_type dy = access_traits::get<1>( B ) - access_traits::get<1>( A );
    return ( dx * dx + dy * dy );
}

//! \brief Compute the distance between two points A-B in the 2D Cartesian plane.
template <typename Point>
inline typename point_traits<Point>::coordinate_type distance_to_point( const Point& A,
                                                                        const Point& B,
                                                                        typename boost::enable_if<
                                                                            boost::is_same< 
                                                                                typename point_traits<Point>::dimension_type,
                                                                                dimension_traits<2> >
                                                                        >::type* dummy = 0 )
{
    return math_functions< point_traits<Point>::coordinate_type >::sqrt( distance_to_point_squared( A, B ) );
}


//! \brief Calculate the distance between two points in any dimensional space for a cartesian reference frame.
template <typename Point>
inline typename point_traits<Point>::coordinate_type distance_to_point_squared( const Point& A, 
                                                                                const Point& B,
                                                                                typename boost::disable_if<
                                                                                    boost::is_same< 
                                                                                        typename point_traits<Point>::dimension_type,
                                                                                        dimension_traits<2> >
                                                                                >::type* dummy = 0 )
{
    return magnitude_squared( A-B );
}

//! \brief Compute the distance between two points A-B in the ND Cartesian plane.
template <typename Point>
inline typename point_traits<Point>::coordinate_type distance_to_point( const Point& A,
                                                                        const Point& B,
                                                                        typename boost::disable_if<
                                                                            boost::is_same< 
                                                                                typename point_traits<Point>::dimension_type,
                                                                                dimension_traits<2> >
                                                                        >::type* dummy = 0 )
{
    return magnitude( vector< point_traits<Point>::coordinate_type, point_traits<Point>::dimension_type::value >(B) - vector< point_traits<Point>::coordinate_type, point_traits<Point>::dimension_type::value >( A ) );
}

}}}//namespace boost::numeric::geometry

#endif //_BOOST_GEOMETRY_POINT_DISTANCE_HPP
