//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_RATIONAL_UTILITIES_HPP
#define GEOMETRIX_RATIONAL_UTILITIES_HPP
#pragma once

#include <geometrix/numeric/number_comparison_policy.hpp>
#include <geometrix/primitive/point_traits.hpp>
#include <geometrix/space/cartesian_access_traits.hpp>
#include <boost/rational.hpp>

namespace geometrix {

//! \brief A type to promote an integral type to a rational type. 
template <typename NumericType>
struct rational_promotion_policy
{
    typedef boost::rational< NumericType > rational_type;
        
    //! \brief Promote a single integral instance to a rational.
    static rational_type promote( const NumericType& i ){ return rational_type( i ); }

    //! \brief Promote a ratio of two integral instances to a rational.
    static rational_type promote( const NumericType& numerator, const NumericType& denominator ){ return rational_type( numerator, denominator ); }
};

//! \brief Given two points which define a (non-vertical) line segment and a coordinate X, calculate Y and the slope.
template <typename Point, typename CoordinateType, typename NumberComparisonPolicy>
inline typename rational_promotion_policy<CoordinateType>::rational_type rational_y_of_x( const Point& s_start, const Point& s_end, CoordinateType x, typename rational_promotion_policy< CoordinateType >::rational_type & slope, const NumberComparisonPolicy& compare )
{
    typedef Point point_type;        
    typedef typename geometric_traits< point_type >::coordinate_type coordinate_type;
    typedef typename rational_promotion_policy< coordinate_type >::rational_type rational_type;
    coordinate_type y0, y1, x0, x1;
    
    x0 = cartesian_access_traits< point_type >::get<0>( s_start );
    x1 = cartesian_access_traits< point_type >::get<0>( s_end );
    y0 = cartesian_access_traits< point_type >::get<1>( s_start );
    y1 = cartesian_access_traits< point_type >::get<1>( s_end );

    slope = rational_type( (y1-y0),(x1-x0) );
    rational_type y = slope * (x - x0) + y0;
    
    return y;
}

//! \brief Given two points which define a (non-vertical) line segment and a coordinate X, calculate Y
template <typename Point, typename CoordinateType, typename NumberComparisonPolicy>
inline typename rational_promotion_policy<CoordinateType>::rational_type rational_y_of_x( const Point& s_start, const Point& s_end, CoordinateType x, const NumberComparisonPolicy& compare )
{
    typedef Point point_type;
    typedef typename geometric_traits< point_type >::coordinate_type coordinate_type;
    typedef rational_promotion_policy< coordinate_type > rational_promote;
    typedef typename rational_promote::rational_type rational_type;
    rational_type slope;
    return rational_y_of_x( s_start, s_end, x, slope, compare );
}

//! \brief Given two points which define a (non-vertical) line segment and a coordinate Y, calculate X and the slope.
template <typename Point, typename CoordinateType, typename NumberComparisonPolicy>
inline typename rational_promotion_policy<CoordinateType>::rational_type rational_x_of_y( const Point& s_start, const Point& s_end, CoordinateType y, boost::rational<CoordinateType>& slope, const NumberComparisonPolicy& compare )
{
    typedef Point point_type;
    typedef typename geometric_traits< point_type >::coordinate_type coordinate_type;
    typedef rational_promotion_policy< coordinate_type > rational_promote;
    typedef typename rational_promote::rational_type rational_type;
    CoordinateType y0, y1, x0, x1;
    
    x0 = cartesian_access_traits< point_type >::get<0>( s_start );
    x1 = cartesian_access_traits< point_type >::get<0>( s_end );
    y0 = cartesian_access_traits< point_type >::get<1>( s_start );
    y1 = cartesian_access_traits< point_type >::get<1>( s_end );
    
    slope = rational_promote::promote( (y1-y0), (x1-x0) );
    rational_type x = ( rational_promote::promote(y - y0) / slope );
    return x + x0;
}

//! \brief Given two points which define a (non-vertical) line segment and a coordinate Y, calculate X
template <typename Point, typename CoordinateType, typename NumberComparisonPolicy>
inline typename rational_promotion_policy<CoordinateType>::rational_type rational_x_of_y( const Point& s_start, const Point& s_end, CoordinateType y, const NumberComparisonPolicy& compare )
{
    typedef Point point_type;
    typedef typename geometric_traits< point_type >::coordinate_type coordinate_type;
    typedef rational_promotion_policy< coordinate_type > rational_promote;
    typedef typename rational_promote::rational_type rational_type;
    rational_type slope;
    return rational_x_of_y( s_start, s_end, y, slope, compare  );
}

}//namespace geometrix;

#endif //GEOMETRIX_RATIONAL_UTILITIES_HPP
