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
#include <geometrix/algebra/dot_product.hpp>

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

                if( get<1>( A ) < get<1>( u1 ) )
                {
                    // u1 above ray
                    if( get<1>( u0 ) <= get<1>( A ) )
                    {
                        //u0 on or below ray                    
                        if( ( get<1>( A ) - get<1>( u0 ) ) * ( get<0>( u1 ) - get<0>( u0 ) ) >
                            ( get<0>( A ) - get<0>( u0 ) ) * ( get<1>( u1 ) - get<1>( u0 ) ) )
                        {
                            inside = !inside;
                        }
                    }
                }
                else if( get<1>( A ) < get<1>( u0 ) )
                {
                    // u1 on or below ray, u0 above ray
                    if( ( get<1>( A ) - get<1>( u0 ) ) * ( get<0>( u1 ) - get<0>( u0 ) ) < 
                        ( get<0>( A ) - get<0>( u0 ) ) * ( get<1>( u1 ) - get<1>( u0 ) ) )
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

	//! \brief Test if  Point p is inside the triangle formed by A, B, C.
	template <typename Point1, typename Point2, typename Point3, typename Point4, typename NumberComparisonPolicy>
	inline bool point_in_triangle( const Point1& p, const Point2& A, const Point3& B, const Point4& C, const NumberComparisonPolicy& cmp )
	{
		//! from http://www.blackpawn.com/texts/pointinpoly/default.html
		typedef typename select_arithmetic_type_from_sequences<Point1, Point2>::type arithmetic_type;
		// Compute vectors        
		vector<arithmetic_type, 2> v0 = C - A;
		vector<arithmetic_type, 2> v1 = B - A;
		vector<arithmetic_type, 2> v2 = p - A;

		// Compute dot products
		arithmetic_type dot00 = dot_product( v0, v0 );
		arithmetic_type dot01 = dot_product( v0, v1 );
		arithmetic_type dot02 = dot_product( v0, v2 );
		arithmetic_type dot11 = dot_product( v1, v1 );
		arithmetic_type dot12 = dot_product( v1, v2 );

		// Compute barycentric coordinates
		arithmetic_type invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
		arithmetic_type u = (dot11 * dot02 - dot01 * dot12) * invDenom;
		arithmetic_type v = (dot00 * dot12 - dot01 * dot02) * invDenom;

		// Check if point is in triangle
		return cmp.greater_than_or_equal( u, 0 ) && cmp.greater_than_or_equal( v, 0 ) && cmp.less_than( u + v, 1 );
	}
    
}//namespace geometrix;

#endif //GEOMETRIX_POINT_IN_POLYGON_HPP
