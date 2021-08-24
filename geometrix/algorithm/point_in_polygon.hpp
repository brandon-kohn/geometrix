//
//! Copyright � 2008-2011
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
#include <geometrix/algebra/cross_product.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/tensor/numeric_sequence_compare.hpp>
#include <geometrix/algorithm/distance/point_segment_distance.hpp>

#include <boost/tuple/tuple.hpp>

namespace geometrix {

    //! Function to test if a point is inside a polygon. (From Geometric Tools for Computer Graphics.)
    namespace detail {

        template <typename Point, typename Polygon>
        inline bool point_in_polygon( const Point& A
                                    , const Polygon& pgon
                                    , dimension<2> )
        {
            BOOST_CONCEPT_ASSERT((Point2DConcept< Point >));

            if( point_sequence_traits< Polygon >::size( pgon ) < 3 ) //! Must at least be a triangle.
                return false;

            typedef typename point_sequence_traits< Polygon>::point_type sequence_point_type;

            bool inside = false;
            std::size_t pSize = point_sequence_traits< Polygon >::size( pgon );
            for( std::size_t i = 0, j = pSize - 1; i < pSize; j = i, ++i )
            {
                const sequence_point_type& u0 = point_sequence_traits< Polygon >::get_point( pgon, i );
                const sequence_point_type& u1 = point_sequence_traits< Polygon >::get_point( pgon, j );

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

    template <typename Point, typename Polygon>
    inline bool point_in_polygon( const Point& A, const Polygon& pgon )
    {
        BOOST_CONCEPT_ASSERT((PointSequenceConcept< Polygon >));
        return detail::point_in_polygon(A, pgon, typename dimension_of<typename point_sequence_traits<Polygon>::point_type>::type() );
    }

    template <typename Point, typename Polygon, typename NumberComparisonPolicy>
    inline bool point_in_subpolygon( const Point& p, const Polygon& pgon, int i0, int i1, const NumberComparisonPolicy& compare )
    {
        using namespace geometrix::detail;
        const Point& v0 = point_sequence_traits<Polygon>::get_point( pgon, i0 );
        const Point& v1 = point_sequence_traits<Polygon>::get_point( pgon, i1 );

        int N = static_cast<int>(point_sequence_traits<Polygon>::size(pgon));

        //! If the indices are adjacent then check the final segment.
        if( ( (i1 - i0 + N) % N ) == 1 )
        {
            orientation_type oType = get_orientation( v0, v1, p, compare );
            return oType == oriented_left || oType == oriented_collinear;
        }

        int mid = get_middle_index( i0, i1, N );
        const Point& midP = point_sequence_traits<Polygon>::get_point( pgon, mid );

        if( get_orientation( v0, midP, p, compare ) == oriented_right )
            return point_in_subpolygon( p, pgon, i0, mid, compare );
        else
            return point_in_subpolygon( p, pgon, mid, i1, compare );
    }

    template <typename Point, typename Polygon, typename NumberComparisonPolicy>
    inline bool point_in_convex_polygon( const Point& p, const Polygon& pgon, const NumberComparisonPolicy& compare )
    {
        BOOST_CONCEPT_ASSERT((PointSequenceConcept< Polygon >));
        GEOMETRIX_ASSERT( point_sequence_traits< Polygon >::size( pgon ) > 2 );

        //GEOMETRIX_ASSERT( numeric_sequence_equals( pgon.front(), pgon.back(), compare ) );//needs to be a closed boundary.

        return point_in_subpolygon( p, pgon, 0, 0, compare );
    }

    template <typename Point, typename Polygon, typename NumberComparisonPolicy>
    inline bool point_in_convex_quadrilateral( const Point& p, const Polygon& pgon, const NumberComparisonPolicy& compare )
    {
        BOOST_CONCEPT_ASSERT((PointSequenceConcept< Polygon >));
        //GEOMETRIX_ASSERT( numeric_sequence_equals( pgon.front(), pgon.back(), compare ) );//needs to be a closed boundary.
        GEOMETRIX_ASSERT( point_sequence_traits< Polygon >::size( pgon ) == 4 );//! there should be five points if we expect front and back to be repeated.

        const Point& v0 = point_sequence_traits<Polygon>::get_point( pgon, 0 );
        const Point& v2 = point_sequence_traits<Polygon>::get_point( pgon, 2 );

        //! Check if the point orientation with the v0->v2 bisect.
        if( get_orientation( v0, v2, p, compare ) != oriented_left )
        {
            //! The point is either collinear or right of the bisect. So check the triangle to the right of the bisect.
            const Point& v1 = point_sequence_traits<Polygon>::get_point( pgon, 1 );

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
            const Point& v3 = point_sequence_traits<Polygon>::get_point( pgon, 3 );

            //! If the point is right of v2->v3 it's outside.
            if( get_orientation( v2, v3, p, compare ) == oriented_right )
                return false;

            //! If the point is right of v3->v0 it's outside.
            if( get_orientation( v3, v0, p, compare ) == oriented_right )
                return false;
        }

        return true;
    }

    namespace detail_point_in_triangle
    {
        namespace result_of {
            template <typename Vector1, typename Vector2>
            struct psuedo_cross_2d
            {
                using length_t = typename select_arithmetic_type_from_sequences<Vector1, Vector2>::type;
                using type = decltype(length_t() * length_t());
            };
        }//!namespace result_of

        template <typename Vector1, typename Vector2>
        inline typename result_of::psuedo_cross_2d<Vector1, Vector2>::type psuedo_cross_2d( const Vector1& u, const Vector2& v )
        {
            return get<1>( u ) * get<0>( v ) - get<0>( u ) * get<1>( v );
        }

        //! \brief Test if  Point p is inside the triangle formed by A, B, C.
        template <typename Point1, typename Point2, typename Point3, typename Point4, typename NumberComparisonPolicy>
        inline bool point_in_triangle( const Point1& p, const Point2& A, const Point3& B, const Point4& C, const NumberComparisonPolicy& cmp, dimension<2> )
        {
            //! From real time collision detection.

            // If P to the right of AB then outside triangle
            if( cmp.less_than( psuedo_cross_2d( p - A, B - A ), constants::zero<typename result_of::psuedo_cross_2d<decltype(p - A), decltype(B - A)>::type>() ) )
                return false;
            // If P to the right of BC then outside triangle
            if( cmp.less_than( psuedo_cross_2d( p - B, C - B ), constants::zero<typename result_of::psuedo_cross_2d<decltype(p - B), decltype(C - B)>::type>()) )
                return false;
            // If P to the right of CA then outside triangle
            if( cmp.less_than( psuedo_cross_2d( p - C, A - C ), constants::zero<typename result_of::psuedo_cross_2d<decltype(p - C), decltype(A - C)>::type>()) )
                return false;
            // Otherwise P must be in (or on) the triangle
            return true;
        }

        //! \brief Test if  Point p is inside the triangle formed by A, B, C.
        template <typename Point1, typename Point2, typename Point3, typename Point4, typename NumberComparisonPolicy>
        inline bool point_in_triangle( const Point1& p, const Point2& A, const Point3& B, const Point4& C, const NumberComparisonPolicy& cmp, dimension<3> )
        {
            typedef typename select_arithmetic_type_from_sequences<Point1, Point2>::type arithmetic_type;

            // Translate point and triangle so that point lies at origin
            vector<arithmetic_type, 3> a = A - p;
            vector<arithmetic_type, 3> b = B - p;
            vector<arithmetic_type, 3> c = C - p;

            // Compute normal vectors for triangles pab and pbc
            auto u = cross_product( b, c );
            auto v = cross_product( c, a );

            // Make sure they are both pointing in the same direction
            if( cmp.less_than(dot_product(u, v), constants::zero<decltype(dot_product(u,v))>() ) )
                return false;

            // Compute normal vector for triangle pca
            auto w = cross_product(a, b);

            // Make sure it points in the same direction as the first two
            if( cmp.less_than(dot_product(u, w), constants::zero<decltype(dot_product(u,w))>() ) )
                return false;

            // Otherwise P must be in (or on) the triangle
            return true;
        }
    }

    template <typename Point1, typename Point2, typename Point3, typename Point4, typename NumberComparisonPolicy>
    inline bool point_in_triangle( const Point1& p, const Point2& A, const Point3& B, const Point4& C, const NumberComparisonPolicy& cmp )
    {
        return detail_point_in_triangle::point_in_triangle( p, A, B, C, cmp, typename dimension_of<Point1>::type() );
    }
    
    template <typename Point1, typename Point2, typename Point3, typename Point4, typename NumberComparisonPolicy, typename BorderDistanceTolerance>
    inline bool point_in_triangle_or_on_border( const Point1& p, const Point2& A, const Point3& B, const Point4& C, const NumberComparisonPolicy& cmp, const BorderDistanceTolerance& bTol )
    {
        if(detail_point_in_triangle::point_in_triangle( p, A, B, C, cmp, typename dimension_of<Point1>::type() ) )
            return true;

        auto bTol2 = bTol * bTol;
        return bTol2 > point_segment_distance_sqrd(p, A, B) 
            || bTol2 > point_segment_distance_sqrd(p, B, C) 
            || bTol2 > point_segment_distance_sqrd(p, C, A);
    }

    enum class polygon_containment
    {
        exterior = 0
      , interior = 1
      , border = 2
      , vertex = 3
    };

    //! Check if a point is contained in a polygon or on its border (From O'Rourke Computational Geometry in C.)
    template <typename Point, typename Polygon, typename NumberComparisonPolicy>
    inline polygon_containment point_polygon_containment_or_on_border(const Point& p, const Polygon& poly, const NumberComparisonPolicy& cmp )
    {
        typedef point_sequence_traits<Polygon> access;
        typedef typename access::point_type point_type;
        typedef typename geometric_traits<point_type>::arithmetic_type arithmetic_type;
        using dimensionless_type = typename geometric_traits<point_type>::dimensionless_type;

        std::size_t size = access::size(poly);
        GEOMETRIX_ASSERT(size > 2);

        std::size_t rcross = 0;
        std::size_t lcross = 0;
        bool rstrad, lstrad;
        for (std::size_t i = 0; i < size; ++i)
        {
            point_type pointi = access::get_point(poly, i);

            if (numeric_sequence_equals(p, pointi, cmp))
                return polygon_containment::vertex;

            std::size_t h = (i + size - 1) % size;
            point_type pointh = access::get_point(poly, h);

            rstrad = cmp.greater_than(get<1>(pointi), get<1>(p)) != cmp.greater_than(get<1>(pointh), get<1>(p));
            lstrad = cmp.less_than(get<1>(pointi), get<1>(p)) != cmp.less_than(get<1>(pointh), get<1>(p));

            if (rstrad || lstrad)
            {
                arithmetic_type ydiff = get<1>(pointh) - get<1>(pointi);
                arithmetic_type denom = (ydiff == constants::zero<arithmetic_type>()) ? std::numeric_limits<arithmetic_type>::epsilon() : ydiff;
                dimensionless_type slopeInverse = (get<0>(pointh) - get<0>(pointi)) / denom;

                arithmetic_type x = slopeInverse * (get<1>(p) - get<1>(pointi)) + get<0>(pointi);
                if (rstrad && cmp.greater_than(x, get<0>(p)))
                    ++rcross;
                if (lstrad && cmp.less_than(x, get<0>(p)))
                    ++lcross;
            }
        }

        if ((rcross % 2) != (lcross % 2))
            return polygon_containment::border;

        if ((rcross % 2) == 1)
            return polygon_containment::interior;

        return polygon_containment::exterior;
    }


}//namespace geometrix;

#endif //GEOMETRIX_POINT_IN_POLYGON_HPP
