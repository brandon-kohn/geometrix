//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_RATIONAL_UTILITIES_HPP
#define _BOOST_GEOMETRY_RATIONAL_UTILITIES_HPP
#pragma once

#include "../geometry/number_comparison_policy.hpp"
#include <boost/rational.hpp>

namespace boost
{
namespace numeric
{
namespace geometry
{

//! Given two points which define a (non-vertical) line segment and a coordinate X calculate Y and the slope.
template <typename Point, typename CoordinateType, typename NumberComparisonPolicy>
inline boost::rational<CoordinateType> rational_y_of_x( const Point& s_start, const Point& s_end, CoordinateType x, boost::rational<CoordinateType>& slope, const NumberComparisonPolicy& compare )
{
        typedef Point point_type;        
        typedef typename point_traits< point_type >::coordinate_type coordinate_type;
        coordinate_type y0, y1, x0, x1;

        x0 = cartesian_access_traits< point_type >::get<0>( s_start );
        x1 = cartesian_access_traits< point_type >::get<0>( s_end );
        y0 = cartesian_access_traits< point_type >::get<1>( s_start );
        y1 = cartesian_access_traits< point_type >::get<1>( s_end );

        slope = boost::rational<CoordinateType>( (y1-y0),(x1-x0) );
        boost::rational<CoordinateType> y = slope * (x - x0) + y0;
        
        return y;
    }

    //! Given two points which define a (non-vertical) line segment and a coordinate X calculate Y and the slope.
    template <typename Point, typename CoordinateType, typename NumberComparisonPolicy>
    inline boost::rational<CoordinateType> rational_y_of_x( const Point& s_start, const Point& s_end, CoordinateType x, const NumberComparisonPolicy& compare )
    {
        CoordinateType slope;
        return rational_y_of_x( s_start, s_end, x, slope, compare );
    }

    //! Given two points which define a (non-vertical) line segment and a coordinate X calculate Y and the slope.
    template <typename Point, typename CoordinateType, typename NumberComparisonPolicy>
    inline boost::rational<CoordinateType> rational_x_of_y( const Point& s_start, const Point& s_end, CoordinateType y, boost::rational<CoordinateType>& slope, const NumberComparisonPolicy& compare )
    {
        typedef Point point_type;
        CoordinateType y0, y1, x0, x1;

        x0 = cartesian_access_traits< point_type >::get<0>( s_start );
        x1 = cartesian_access_traits< point_type >::get<0>( s_end );
        y0 = cartesian_access_traits< point_type >::get<1>( s_start );
        y1 = cartesian_access_traits< point_type >::get<1>( s_end );
               
        slope = boost::rational<CoordinateType>( (y1-y0),(x1-x0) );
        boost::rational<CoordinateType> x( (y - y0), slope );

        return x + x0;
    }

//! Given two points which define a (non-vertical) line segment and a coordinate X calculate Y and the slope.
template <typename Point, typename CoordinateType, typename NumberComparisonPolicy>
inline boost::rational<CoordinateType> rational_x_of_y( const Point& s_start, const Point& s_end, CoordinateType y, const NumberComparisonPolicy& compare )
{
    boost::rational<CoordinateType> slope;
    return rational_x_of_y( s_start, s_end, y, slope, compare  );
}

}}}//namespace boost::numeric::geometry;


#endif //_BOOST_GEOMETRY_RATIONAL_UTILITIES_HPP



