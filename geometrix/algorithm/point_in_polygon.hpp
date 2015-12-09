//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_POINT_IN_POLYGON_HPP
#define GEOMETRIX_POINT_IN_POLYGON_HPP
#pragma once

#include <geometrix/primitive/point_sequence_traits.hpp>
#include <geometrix/tensor/tensor_traits.hpp>
#include <geometrix/algebra/cross_product.hpp>

#include <boost/foreach.hpp>
#include <boost/tuple/tuple.hpp>

namespace geometrix {
        
    //! Function to test if a point is inside a polygon. (From Geometric Tools for Computer Graphics.)
    namespace detail {
    
        template <typename Point, typename PointSequence>
        inline bool point_in_polygon( const Point& A
                                    , const PointSequence& polygon
                                    , dimension<2> )
        {
            BOOST_CONCEPT_ASSERT((Point2DConcept< Point >));            

            if( point_sequence_traits< PointSequence >::size( polygon ) < 3 ) //! Must at least be a triangle.
                return false;

            typedef typename point_sequence_traits< PointSequence>::point_type sequence_point_type;
        
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
     
        inline int get_middle_index( int i0, int i1, int N )
        {
            return ( i0 < i1 ? ( ( i0 + i1 ) / 2 )
                : ( i0 + i1 + N ) / 2 ) % N;
        }
    }//! namespace geometrix::detail;

    template <typename Point, typename PointSequence>
    inline bool point_in_polygon( const Point& A, const PointSequence& polygon )
    {
        BOOST_CONCEPT_ASSERT((PointSequenceConcept< PointSequence >));
        return detail::point_in_polygon(A, polygon, typename dimension_of<typename point_sequence_traits<PointSequence>::point_type>::type() );
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
        BOOST_ASSERT( point_sequence_traits< PointSequence >::size( polygon ) == 5 );//! there should be five points if we expect front and back to be repeated.

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
    
}//namespace geometrix;

#endif //GEOMETRIX_POINT_IN_POLYGON_HPP
