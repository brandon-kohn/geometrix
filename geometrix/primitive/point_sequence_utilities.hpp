//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_POINT_SEQUENCE_UTILITIES_HPP
#define GEOMETRIX_POINT_SEQUENCE_UTILITIES_HPP
#pragma once

#include <geometrix/primitive/point_sequence_traits.hpp>
#include <geometrix/tensor/tensor_traits.hpp>
#include <geometrix/space/cartesian_access_traits.hpp>
#include <geometrix/algebra/cross_product.hpp>

#include <boost/foreach.hpp>
#include <boost/tuple/tuple.hpp>

namespace geometrix {
    //! Function to calculate the centroid of a point sequence.
    template <typename PointSequence>
    inline typename point_sequence_traits<PointSequence>::point_type 
    get_centroid( const PointSequence& polygon, 
                  typename boost::enable_if
                  <
                    boost::is_same
                    <
                        typename geometric_traits
                        <
                            typename point_sequence_traits<PointSequence>::point_type
                        >::dimension_type,
                        dimension<2>
                    >
                  >::type* = 0 )
    {
        BOOST_CONCEPT_ASSERT((PointSequenceConcept< PointSequence >));

        typedef typename point_sequence_traits<PointSequence>::point_type point_type;		
        typedef typename geometric_traits<point_type>::arithmetic_type        arithmetic_type;
        typedef cartesian_access_traits< point_type >                     tensor_traits;
        BOOST_CONCEPT_ASSERT((CartesianCoordinateAccessorConcept< tensor_traits >));

        arithmetic_type mX = 0.;
        arithmetic_type mY = 0.;
        arithmetic_type area = 0.;
        for( typename PointSequence::const_iterator it( point_sequence_traits< PointSequence >::begin( polygon ) ), 
             nextIt( point_sequence_traits< PointSequence >::begin( polygon ) + 1),
             end( point_sequence_traits< PointSequence >::end( polygon ) );
             nextIt != end; 
             ++it, ++nextIt )
        {
            const point_type& currentPoint = *it;
            const point_type& nextPoint = *nextIt;
            typename geometric_traits<point_type>::arithmetic_type ai = exterior_product_area( as_vector( currentPoint ), as_vector( nextPoint ) );
            area += ai;
            mX += ai * ( get<0>( currentPoint ) + get<0>( nextPoint ) );
            mY += ai * ( get<1>( currentPoint ) + get<1>( nextPoint ) );	
        }

        {
            const point_type& backPoint = point_sequence_traits< PointSequence >::back( polygon );
            const point_type& frontPoint = point_sequence_traits< PointSequence >::front( polygon );

            typename geometric_traits<point_type>::arithmetic_type ai = exterior_product_area( as_vector( backPoint ), as_vector( frontPoint ) );
            area += ai;
            mX += ai * ( get<0>( backPoint ) + get<0>( frontPoint ) );
            mY += ai * ( get<1>( backPoint ) + get<1>( frontPoint ) );	
        }
        
        area *= 0.5;
        arithmetic_type q = 1. /( 6.0 * area);		
        return construction_policy<point_type>::construct( mX * q, mY * q );
    }

    //! Function to calculate the centroid of a point sequence.
    template <typename PointSequence>
    inline double get_area( const PointSequence& polygon,
                              typename boost::enable_if
                              <
                                boost::is_same
                                <
                                    typename geometric_traits
                                    <
                                        typename point_sequence_traits<PointSequence>::point_type
                                    >::dimension_type,
                                    dimension<2>
                                >
                              >::type* = 0 )
    {
        BOOST_CONCEPT_ASSERT((PointSequenceConcept< PointSequence >));
        typedef typename point_sequence_traits<PointSequence>::point_type point_type;
        typename geometric_traits<point_type>::arithmetic_type area = 0;
        for( typename PointSequence::const_iterator it( point_sequence_traits< PointSequence >::begin( polygon ) ),
             nextIt( point_sequence_traits< PointSequence >::begin( polygon ) + 1), end( point_sequence_traits< PointSequence >::end( polygon ) );
             nextIt != end; ++it, ++nextIt )
        {
            area += exterior_product_area( as_vector(*it), as_vector(*nextIt) );			
        }

        area += exterior_product_area( as_vector( point_sequence_traits< PointSequence >::back( polygon ) ), as_vector( point_sequence_traits< PointSequence >::front( polygon ) ) );
        area *= 0.5;	
        return area;
    }

    //! Function to test if a point is inside a polygon. (From Geometric Tools for Computer Graphics.)
    template <typename Point, typename PointSequence>
    inline bool point_in_polygon( const Point& A,
                           const PointSequence& polygon, 
                           typename boost::enable_if
                           <
                               boost::is_same
                               <
                                   typename geometric_traits
                                   <
                                       typename point_sequence_traits<PointSequence>::point_type
                                   >::dimension_type, 
                                   dimension<2> 
                               > 
                           >::type* = 0 )
    {
        BOOST_CONCEPT_ASSERT((PointConcept< Point >));
        BOOST_CONCEPT_ASSERT((PointSequenceConcept< PointSequence >));

        if( point_sequence_traits< PointSequence >::size( polygon ) < 3 ) //! Must at least be a triangle.
            return false;

        typedef typename point_sequence_traits< PointSequence>::point_type sequence_point_type;
        typedef cartesian_access_traits< Point >                  paccess;
        typedef cartesian_access_traits< sequence_point_type >    saccess;
        BOOST_CONCEPT_ASSERT((CartesianCoordinateAccessorConcept< paccess >));
        BOOST_CONCEPT_ASSERT((CartesianCoordinateAccessorConcept< saccess >));

        bool inside = false;
        std::size_t pSize = point_sequence_traits< PointSequence >::size( polygon );
        for( std::size_t i = 0, j = pSize - 1; i < pSize; j = i, ++i )
        {
            const sequence_point_type& u0 = point_sequence_traits< PointSequence >::get_point( polygon, i );
            const sequence_point_type& u1 = point_sequence_traits< PointSequence >::get_point( polygon, j );

            if( get_y( A ) < get_y( u1 ) )
            {
                // u1 above ray
                if( get_y( u0 ) <= get_y( A ) )
                {
                    //u0 on or below ray                    
                    if( ( get_y( A ) - get_y( u0 ) ) * ( get_x( u1 ) - get_x( u0 ) ) >
                        ( get_x( A ) - get_x( u0 ) ) * ( get_y( u1 ) - get_y( u0 ) ) )
                    {
                        inside = !inside;
                    }
                }
            }
            else if( get_y( A ) < get_y( u0 ) )
            {
                // u1 on or below ray, u0 above ray
                if( ( get_y( A ) - get_y( u0 ) ) * ( get_x( u1 ) - get_x( u0 ) ) < 
                    ( get_x( A ) - get_x( u0 ) ) * ( get_y( u1 ) - get_y( u0 ) ) )
                {
                    inside = !inside;
                }
            }
        }

        return inside;
    }

    namespace detail
    {        
        inline int get_middle_index( int i0, int i1, int N )
        {
            return ( i0 < i1 ? ( ( i0 + i1 ) / 2 )
                : ( i0 + i1 + N ) / 2 ) % N;
        }
    }

    template <typename Point, typename PointSequence, typename NumberComparisonPolicy>
    inline bool point_in_subpolygon( const Point& p, const PointSequence& polygon, int i0, int i1, const NumberComparisonPolicy& compare )
    {
        using namespace geometrix::detail;
        const Point& v0 = point_sequence_traits<PointSequence>::get_point( polygon, i0 );
        const Point& v1 = point_sequence_traits<PointSequence>::get_point( polygon, i1 );

        int N = polygon.size();

        //! If the indices are adjacent then check the final segment.
        if( ( (i1 - i0 + N) % N ) == 1 )
        {
            orientation_type oType = get_orientation( v0, v1, p, compare );
            return oType == oriented_left || oType == oriented_collinear;
        }

        int mid = get_middle_index( i0, i1, N );
        const Point& midP = point_sequence_traits<PointSequence>::get_point( polygon, mid );

        if( get_orientation( v0, midP, p, compare ) == oriented_right )
            return point_in_subpolygon( p, polygon, i0, mid, compare );
        else
            return point_in_subpolygon( p, polygon, mid, i1, compare );
    }

    template <typename Point, typename PointSequence, typename NumberComparisonPolicy>
    inline bool point_in_convex_polygon( const Point& p, const PointSequence& polygon, const NumberComparisonPolicy& compare )
    {
        BOOST_CONCEPT_ASSERT((PointSequenceConcept< PointSequence >));
        BOOST_ASSERT( point_sequence_traits< PointSequence >::size( polygon ) > 2 );

        BOOST_ASSERT( numeric_sequence_equals( polygon.front(), polygon.back(), compare ) );//needs to be a closed boundary.

        return point_in_subpolygon( p, polygon, 0, 0, compare );
    }

    template <typename Point, typename PointSequence, typename NumberComparisonPolicy>
    inline bool point_in_convex_quadrilateral( const Point& p, const PointSequence& polygon, const NumberComparisonPolicy& compare )
    {
        BOOST_CONCEPT_ASSERT((PointSequenceConcept< PointSequence >));
        BOOST_ASSERT( numeric_sequence_equals( polygon.front(), polygon.back(), compare ) );//needs to be a closed boundary.
        BOOST_ASSERT( point_sequence_traits< PointSequence >::size( polygon ) == 5 );//! ugh.. this was bad :D.        

        const Point& v0 = point_sequence_traits<PointSequence>::get_point( polygon, 0 );
        const Point& v2 = point_sequence_traits<PointSequence>::get_point( polygon, 2 );

        //! Check if the point orientation with the v0->v2 bisect.
        if( get_orientation( v0, v2, p, compare ) != oriented_left )
        {
            //! The point is either collinear or right of the bisect. So check the triangle to the right of the bisect.
            const Point& v1 = point_sequence_traits<PointSequence>::get_point( polygon, 1 );

            //! If the point is right of v0->v1 it's outside.
            if( get_orientation( v0, v1, p, compare ) == oriented_right )
                return false;

            //! If the point is right of v1->v2 it's outside.
            if( get_orientation( v1, v2, p, compare ) == oriented_right )
                return false;
        }
        else
        {
            //! The point is left of the bisect.
            const Point& v3 = point_sequence_traits<PointSequence>::get_point( polygon, 3 );

            //! If the point is right of v2->v3 it's outside.
            if( get_orientation( v2, v3, p, compare ) == oriented_right )
                return false;

            //! If the point is right of v3->v0 it's outside.
            if( get_orientation( v3, v0, p, compare ) == oriented_right )
                return false;
        }

        return true;
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
        typedef boost::tuple< typename geometric_traits< typename point_sequence_traits< PointSequence >::point_type >::arithmetic_type,
                              typename geometric_traits< typename point_sequence_traits< PointSequence >::point_type >::arithmetic_type,
                              typename geometric_traits< typename point_sequence_traits< PointSequence >::point_type >::arithmetic_type,
                              typename geometric_traits< typename point_sequence_traits< PointSequence >::point_type >::arithmetic_type > type;
    };
    template <typename PointSequence, typename NumberComparisonPolicy>
    inline typename bounds_tuple< PointSequence >::type 
    get_bounds( const PointSequence& pointSequence, 
                const NumberComparisonPolicy& compare,
                typename boost::enable_if
                <
                    boost::is_same
                    <
                        typename geometric_traits
                        <
                            typename point_sequence_traits<PointSequence>::point_type 
                        >::dimension_type,
                        dimension<2>
                    >
                >::type* = 0 )
    {
        typedef typename point_sequence_traits< PointSequence >::point_type point_type;
        typedef typename geometric_traits< point_type >::arithmetic_type    arithmetic_type;

        typedef boost::tuple< arithmetic_type, 
                              arithmetic_type,
                              arithmetic_type,
                              arithmetic_type > bounds_tuple;

        bounds_tuple bounds;
        if( std::numeric_limits< arithmetic_type >::has_infinity )
        {
            bounds = boost::make_tuple( std::numeric_limits< arithmetic_type >::infinity(),
                                        std::numeric_limits< arithmetic_type >::infinity(),
                                        -std::numeric_limits< arithmetic_type >::infinity(),
                                        -std::numeric_limits< arithmetic_type >::infinity() );
        }
        else
        {
            bounds = boost::make_tuple( (std::numeric_limits< arithmetic_type >::max)(),
                                        (std::numeric_limits< arithmetic_type >::max)(),
                                        -(std::numeric_limits< arithmetic_type >::max)(),
                                        -(std::numeric_limits< arithmetic_type >::max)() );
        }

        typename point_sequence_traits< PointSequence >::const_iterator pIt = point_sequence_traits< PointSequence >::begin( pointSequence ); 
        typename point_sequence_traits< PointSequence >::const_iterator pEnd = point_sequence_traits< PointSequence >::end( pointSequence );        
        while( pIt != pEnd )
        {
            const point_type& p = *pIt++;
            const arithmetic_type& x = get<0>( p );            
            if( compare.less_than( x, bounds.get<e_xmin>() ) )
                bounds.get<e_xmin>() = x;
            if( compare.greater_than( x, bounds.get<e_xmax>() ) )
                bounds.get<e_xmax>() = x;

            const arithmetic_type& y = get<1>( p );
            if( compare.less_than( y, bounds.get<e_ymin>() ) )
                bounds.get<e_ymin>() = y;
            if( compare.greater_than( y, bounds.get<e_ymax>() ) )
                bounds.get<e_ymax>() = y;
        }

        return bounds;
    }
    
}//namespace geometrix;

#endif //GEOMETRIX_POINT_SEQUENCE_UTILITIES_HPP
