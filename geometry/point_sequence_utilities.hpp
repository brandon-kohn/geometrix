//
//! Copyright © 2008
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_POINT_SEQUENCE_UTILITIES_HPP
#define _BOOST_GEOMETRY_POINT_SEQUENCE_UTILITIES_HPP
#pragma once

#include "point_sequence_traits.hpp"
#include <boost/foreach.hpp>
#include <boost/tuple/tuple.hpp>

namespace boost
{
namespace numeric
{
namespace geometry
{
	//! Function to calculate the centroid of a point sequence.
	template <typename PointSequence, typename NumberComparisonPolicy>
    inline typename point_sequence_traits<PointSequence>::point_type get_centroid( const PointSequence& polygon, const NumberComparisonPolicy& compare, typename boost::enable_if< boost::is_same< typename point_traits< typename point_sequence_traits<PointSequence>::point_type >::dimension_type, dimension_traits<2> > >::type* dummy = 0 )
	{
        boost::function_requires< PointSequenceConcept< PointSequence > >();
        //assert( equals( polygon.front(), polygon.back(), fraction_tolerance_comparison_policy<double>(1e-10) ) );//needs to be a closed boundary.

        typedef typename point_sequence_traits<PointSequence>::point_type point_type;		
		typedef typename point_traits<point_type>::coordinate_type        coordinate_type;
        typedef cartesian_access_traits< point_type >                     access_traits;
        boost::function_requires< CartesianCoordinateAccessorConcept< access_traits > >();

        coordinate_type mX = 0.;
        coordinate_type mY = 0.;
		coordinate_type area = 0;
		for( typename PointSequence::const_iterator it( point_sequence_traits< PointSequence >::begin( polygon ) ), nextIt( point_sequence_traits< PointSequence >::begin( polygon ) + 1), end( point_sequence_traits< PointSequence >::end( polygon ) ); nextIt != end; ++it, ++nextIt )
		{
			const point_type& currentPoint = *it;
			const point_type& nextPoint = *nextIt;
			double ai = cross_product( currentPoint, nextPoint );
			area += ai;
            mX += ai * ( access_traits::get<0>( currentPoint ) + access_traits::get<0>( nextPoint ) );
            mY += ai * ( access_traits::get<1>( currentPoint ) + access_traits::get<1>( nextPoint ) );	
		}

        if( !numeric_sequence_equals( point_sequence_traits< PointSequence >::front( polygon ), point_sequence_traits< PointSequence >::back( polygon ), compare ) )
        {
            const point_type& backPoint = point_sequence_traits< PointSequence >::back( polygon );
			const point_type& frontPoint = point_sequence_traits< PointSequence >::front( polygon );

            double ai = cross_product( backPoint, frontPoint );
            area += ai;
            mX += ai * ( access_traits::get<0>( backPoint ) + access_traits::get<0>( frontPoint ) );
            mY += ai * ( access_traits::get<1>( backPoint ) + access_traits::get<1>( frontPoint ) );	
        }
		
		area *= 0.5;
		coordinate_type q = 1. /( 6.0 * area);		
        return construction_traits<point_type>::construct( mX * q, mY * q );
	}

    //! Function to calculate the centroid of a point sequence.
	template <typename PointSequence, typename NumberComparisonPolicy>
	inline typename double get_area( const PointSequence& polygon, const NumberComparisonPolicy& compare, typename boost::enable_if< boost::is_same< typename point_traits< typename point_sequence_traits<PointSequence>::point_type >::dimension_type, dimension_traits<2> > >::type* dummy = 0 )
	{
        boost::function_requires< PointSequenceConcept< PointSequence > >();        
        assert( numeric_sequence_equals( polygon.front(), polygon.back(), fraction_tolerance_comparison_policy<double>(1e-10) ) );//needs to be a closed boundary.

        typedef typename PointSequence::value_type                 point_type;		        
		typedef typename point_traits<point_type>::coordinate_type coordinate_type;

		double area = 0;
		for( typename PointSequence::const_iterator it( point_sequence_traits< PointSequence >::begin( polygon ) ), nextIt( point_sequence_traits< PointSequence >::begin( polygon ) + 1), end( point_sequence_traits< PointSequence >::end( polygon ) ); nextIt != end; ++it, ++nextIt )
		{
			const point_type& currentPoint = *it;
			const point_type& nextPoint = *nextIt;
			double ai = cross_product( currentPoint, nextPoint );
			area += ai;
		}

        if( !numeric_sequence_equals( point_sequence_traits< PointSequence >::front( polygon ), point_sequence_traits< PointSequence >::back( polygon ), compare ) )
        {
            const point_type& currentPoint = point_sequence_traits< PointSequence >::back( polygon );
			const point_type& nextPoint = point_sequence_traits< PointSequence >::front( polygon );
			double ai = cross_product( currentPoint, nextPoint );
			area += ai;
        }
		
		area *= 0.5;	
		return area;
	}

    //! Function to test if a point is inside a polygon. (From Geometric Tools for Computer Graphics.)
    template <typename Point, typename PointSequence>
    inline bool point_in_polygon( const Point& A, const PointSequence& polygon, typename boost::enable_if< boost::is_same< typename point_traits< typename point_sequence_traits<PointSequence>::point_type >::dimension_type, dimension_traits<2> > >::type* dummy = 0 )
    {
        boost::function_requires< PointConcept< Point > >();
        boost::function_requires< PointSequenceConcept< PointSequence > >();
        boost::function_requires< RandomAccessContainerConcept< PointSequence > >();

        if( point_sequence_traits< PointSequence >::size( polygon ) < 3 ) //! Must at least be a triangle.
            return false;

        typedef point_sequence_traits< PointSequence>::point_type sequence_point_type;
        typedef cartesian_access_traits< Point >                  paccess;
        typedef cartesian_access_traits< sequence_point_type >    saccess;
        boost::function_requires< CartesianCoordinateAccessorConcept< paccess > >();
        boost::function_requires< CartesianCoordinateAccessorConcept< saccess > >();

        bool inside = false;
        size_t pSize = point_sequence_traits< PointSequence >::size( polygon );
        for( size_t i = 0, j = pSize - 1; i < pSize; j = i, ++i )
        {
            const sequence_point_type& u0 = point_sequence_traits< PointSequence >::get_point( polygon, i );
            const sequence_point_type& u1 = point_sequence_traits< PointSequence >::get_point( polygon, j );

            if( paccess::get<1>( A ) < saccess::get<1>( u1 ) )
            {
                // u1 above ray
                if( saccess::get<1>( u0 ) <= paccess::get<1>( A ) )
                {
                    //u0 on or below ray                    
                    if( ( paccess::get<1>( A ) - saccess::get<1>( u0 ) ) * ( saccess::get<0>( u1 ) - saccess::get<0>( u0 ) ) > ( paccess::get<0>( A ) - saccess::get<0>( u0 ) ) * ( saccess::get<1>( u1 ) - saccess::get<1>( u0 ) ) )
                    {
                        inside = !inside;
                    }
                }
            }
            else if( paccess::get<1>( A ) < saccess::get<1>( u0 ) )
            {
                // u1 on or below ray, u0 above ray
                if( ( paccess::get<1>( A ) - saccess::get<1>( u0 ) ) * ( saccess::get<0>( u1 ) - saccess::get<0>( u0 ) ) < ( paccess::get<0>( A ) - saccess::get<0>( u0 ) ) * ( saccess::get<1>( u1 ) - saccess::get<1>( u0 ) ) )
                {
                    inside = !inside;
                }
            }
        }

        return inside;
    }

     //! Function to calculate the min/max bounds of a point sequence.
    enum cartesian_bound
    {
        e_xmin = 0,
        e_ymin = 1,
        e_xmax = 2,
        e_ymax = 3
    };
    template <typename PointSequence>
    struct bounds_tuple
    {
        typedef boost::tuple< typename point_sequence_traits< PointSequence >::coordinate_type, 
                              typename point_sequence_traits< PointSequence >::coordinate_type,
                              typename point_sequence_traits< PointSequence >::coordinate_type,
                              typename point_sequence_traits< PointSequence >::coordinate_type > type;
    };
    template <typename PointSequence, typename NumberComparisonPolicy>
    typename bounds_tuple< PointSequence >::type get_bounds( const PointSequence& pointSequence, const NumberComparisonPolicy& compare, typename boost::enable_if< boost::is_same< typename point_traits< typename point_sequence_traits<PointSequence>::point_type >::dimension_type, dimension_traits<2> > >::type* dummy = 0 )
    {
        typedef typename point_sequence_traits< PointSequence >::point_type point_type;
        typedef typename point_traits< point_type >::coordinate_type        coordinate_type;

        typedef boost::tuple< typename point_sequence_traits< PointSequence >::coordinate_type, 
                              typename point_sequence_traits< PointSequence >::coordinate_type,
                              typename point_sequence_traits< PointSequence >::coordinate_type,
                              typename point_sequence_traits< PointSequence >::coordinate_type > bounds_tuple;

        bounds_tuple bounds;
        if( std::numeric_limits< coordinate_type >::has_infinity )
        {
            bounds = boost::make_tuple( std::numeric_limits< coordinate_type >::infinity(), std::numeric_limits< coordinate_type >::infinity(), -std::numeric_limits< coordinate_type >::infinity(), -std::numeric_limits< coordinate_type >::infinity() );
        }
        else
        {
            bounds = boost::make_tuple( (std::numeric_limits< coordinate_type >::max)(), (std::numeric_limits< coordinate_type >::max)(), -(std::numeric_limits< coordinate_type >::max)(), -(std::numeric_limits< coordinate_type >::max)() );
        }

        point_sequence_traits< PointSequence >::const_iterator pIt = point_sequence_traits< PointSequence >::begin( pointSequence ); 
        point_sequence_traits< PointSequence >::const_iterator pEnd = point_sequence_traits< PointSequence >::end( pointSequence );        
        while( pIt != pEnd )
        {
            const point_type& p = *pIt++;
            const coordinate_type& x = cartesian_access_traits< point_type >::get<0>( p );            
            if( compare.less_than( x, bounds.get<e_xmin>() ) )
                bounds.get<e_xmin>() = x;
            if( compare.greater_than( x, bounds.get<e_xmax>() ) )
                bounds.get<e_xmax>() = x;

            const coordinate_type& y = cartesian_access_traits< point_type >::get<1>( p );
            if( compare.less_than( y, bounds.get<e_ymin>() ) )
                bounds.get<e_ymin>() = y;
            if( compare.greater_than( y, bounds.get<e_ymax>() ) )
                bounds.get<e_ymax>() = y;
        }

        return bounds;
    }
	
}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_POINT_SEQUENCE_UTILITIES_HPP
