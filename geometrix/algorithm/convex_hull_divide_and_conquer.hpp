//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_CONVEX_HULL_DIVIDE_AND_CONQUER_HPP
#define GEOMETRIX_CONVEX_HULL_DIVIDE_AND_CONQUER_HPP
#pragma once

#include <geometrix\primitive\point_traits.hpp>
#include <geometrix\primitive\point_sequence_traits.hpp>
#include <set>

namespace geometrix
{
    namespace detail
    {
        template <typename Polygon>
        inline size_t index_of_maximum_x( const Polygon& polygon )
        {
            typedef typename point_sequence_traits<Polygon>::point_type point_type;
            typedef typename point_traits<point_type>::coordinate_type  coordinate_type;
            typedef cartesian_access_traits< point_type >               access;
            size_t lMax = (std::numeric_limits<size_t>::max)();
            coordinate_type maxX = -( std::numeric_limits<coordinate_type>::max )();
            for( size_t i=0; i < polygon.size(); ++i )
            {
                coordinate_type x = access::get_x( polygon[i] );
                if( x > maxX )
                {
                    maxX = x;
                    lMax = i;
                }
            }

            return lMax;
        }

        template <typename Polygon>
        inline size_t index_of_minimum_x( const Polygon& polygon )
        {
            typedef typename point_sequence_traits<Polygon>::point_type point_type;
            typedef typename point_traits<point_type>::coordinate_type  coordinate_type;
            typedef cartesian_access_traits< point_type >               access;
            size_t lMin = (std::numeric_limits<size_t>::max)();
            coordinate_type minX = ( std::numeric_limits<coordinate_type>::max )();
            for( size_t i=0; i < polygon.size(); ++i )
            {
                coordinate_type x = access::get_x( polygon[i] );
                if( x < minX )
                {
                    minX = x;
                    lMin = i;
                }
            }

            return lMin;
        }
    }

    enum collinear_orientation
    {
        e_negative = -1,
        e_positive = 1,
        e_collinear_left = 2,
        e_collinear_right = 3,
        e_collinear_contain = 4
    };

    template <typename Point>
    inline collinear_orientation collinear_test( const Point& p, const Point& q0, const Point& q1, double epsilon0 = 1e-5, double epsilon1=1e-6 )
    {
        typedef Point                                               point_type;
        typedef typename point_traits<point_type>::coordinate_type  coordinate_type;
        typedef cartesian_access_traits< point_type >               access;

        point_type D = q1-q0;
        point_type A = p - q0;

        coordinate_type NdA = access::get_x( D ) * access::get_y( A ) - access::get_y( D ) * access::get_x( A );
        coordinate_type NdN = access::get_x( D ) * access::get_x( D ) - access::get_y( D ) * access::get_y( D );
        coordinate_type AdA = access::get_x( A ) * access::get_x( A ) - access::get_y( A ) * access::get_y( A );

        if( NdA * NdA > epsilon0 * NdN * AdA )
        {
            if( NdA > 0 )
                return e_positive;
            if( NdA < 0 )
                return e_negative;
        }

        coordinate_type DdA = dot_product( D, A );
        if( DdA < -epsilon1 * NdN )
        {
            return e_collinear_left;
        }
        if( DdA > (1 + epsilon1 ) * NdN )
        {
            return e_collinear_right;
        }

        return e_collinear_contain;        
    }

    enum merge_type
    {
        e_planar = 1
    };
    template <typename Point, typename Polygon>
    inline void merge_linear( const Point& p, Polygon& hull )
    {
        typedef typename point_sequence_traits<Polygon>::point_type point_type;
        typedef cartesian_access_traits< Point >                    access;
        switch( collinear_test( p, hull[0], hull[1] ) )
        {
        case e_positive:
            {
                hull.insert( hull.begin(), p );
                break;
            }
        case e_negative:
            {
                hull.insert( hull.begin(), p );
                std::swap( hull[2], hull[1] );
                break;
            }
        case e_collinear_left:
            {
                hull.insert( hull.begin(), hull[1] );
                hull.insert( hull.begin(), p );
                break;
            }
        case e_collinear_right:
            {
                hull.insert( hull.begin(), p );
                hull.insert( hull.begin(), hull[0] );
                break;
            }
        case e_collinear_contain:
            {
                break;
            }
        }
    }

    template <typename Polygon>
    inline void get_tangent( const Polygon& lHull, const Polygon& rHull, size_t& l, size_t& r )
    {
        typedef typename point_sequence_traits<Polygon>::point_type point_type;
        typedef cartesian_access_traits< point_type >               access;

        for( size_t i = 0; i < lHull.size() + rHull.size(); ++i )
        {
            point_type l1 = lHull[l];
            point_type r0 = rHull[r];

            size_t lm1 = (l - 1) % lHull.size();
            point_type l0 = lHull[lm1];

            int test = collinear_test( r0, l0, l1 );
            if( test == e_negative || test == e_collinear_left )
            {
                l = lm1;
                continue;
            }

            size_t rp1 = (r + 1) % rHull.size();
            point_type r1 = rHull[rp1];
            test = collinear_test( l1, r0, r1 );
            if( test == e_negative || test == e_collinear_right )
            {
                r = rp1;
                continue;
            }

            //! tangent segment has been found.
            break;
        }
    }

    template <typename Polygon>
    inline void merge( Polygon& lHull, Polygon& rHull, Polygon& hull )
    {
        typedef typename point_sequence_traits<Polygon>::point_type point_type;
        typedef cartesian_access_traits< point_type >               access;
        
        if( lHull.size() == 1 && rHull.size() == 1 )
        {
            hull.clear();
            hull.push_back( lHull[0] );
            hull.push_back( rHull[0] );
            return;
        }
        if( lHull.size() == 1 && rHull.size() == 2 )
        {
            merge_linear( lHull[0], rHull );
            hull = rHull;
            return;
        }
        if( lHull.size() == 2 && rHull.size() == 1 )
        {
            merge_linear( rHull[0], lHull );
            hull = lHull;
            return;
        }
        if( lHull.size() == 2 && rHull.size() == 2 )
        {
            merge_linear( rHull[1], lHull );
            if( lHull.size() == 2 )
            {
                merge_linear( rHull[0], lHull );
                hull = lHull;
                return;
            }
            rHull.erase( rHull.begin() + 1 );
        }

        size_t lMax = detail::index_of_maximum_x( lHull );
        size_t rMin = detail::index_of_minimum_x( rHull );

        size_t llIndex = lMax;
        size_t lrIndex = rMin;
        get_tangent( lHull, rHull, llIndex, lrIndex );

        size_t ulIndex = lMax;
        size_t urIndex = rMin;
        get_tangent( rHull, lHull, urIndex, ulIndex );

        Polygon tmpHull;
        size_t i = 0;

        size_t start = (std::min)( lrIndex, urIndex );
        size_t end = (std::max)( lrIndex, urIndex );
        for( size_t j = start; j <= end; ++j )
        {
            tmpHull.push_back( hull[j] );
            ++i;
        }

        start = (std::min)( ulIndex, llIndex );
        end   = (std::max)( ulIndex, llIndex );
        for( size_t j = start; j <= end; ++j )
        {
            tmpHull.push_back( hull[j] );
            ++i;
        }
        
        hull = tmpHull;
    }

    template <typename Polygon, typename Point>
    inline void get_hull( size_t i0, size_t i1, std::vector< Point >& pVector, Polygon& hull )
    {
        size_t quantity = i1-i0 + 1;
        if( quantity > 1 )
        {
            size_t mid = (i0+i1) / 2;

            Polygon lHull, rHull;
            get_hull( i0, mid, pVector, lHull );
            get_hull( mid+1, i1, pVector, rHull );

            merge( lHull, rHull, hull );
        }
        else
        {
            hull.clear();
            hull.push_back( pVector[i0] );
        }
    }

    template <typename Polygon, typename PointCollection>
    inline boost::shared_ptr< Polygon > convex_hull( const PointCollection& points )
    {
        typedef typename point_sequence_traits< PointCollection >::point_type point_type;
        std::set< point_type, point_lexicographical_compare > pSet( points.begin(), points.end() );
        std::vector< point_type > pVector( pSet.begin(), pSet.end() );
        boost::shared_ptr< Polygon > hull( new Polygon() );
        get_hull( 0, pVector.size() - 1, pVector, *hull ); 
        return hull;
    }

}//namespace geometrix;

#endif //GEOMETRIX_CONVEX_HULL_DIVIDE_AND_CONQUER_HPP
