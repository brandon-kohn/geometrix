//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GENERATIVE_GEOMETRY_POINT_SEQUENCE_TESTS_HPP
#define GENERATIVE_GEOMETRY_POINT_SEQUENCE_TESTS_HPP
#pragma once

#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include "../geometry/point.hpp"
#include "../geometry/utilities.hpp"
#include "../geometry/point_sequence.hpp"
#include "../geometry/point_sequence_utilities.hpp"

namespace generative 
{
namespace numeric 
{
namespace geometry
{
    template <typename Point>
    inline Point perp( const Point& u )
    {
        return construction_traits< Point >::construct( - indexed_access_traits<Point>::get<1>(u), indexed_access_traits<Point>::get<0>(u) );
    }

    inline int get_middle_index( int i0, int i1, int N )
    {
        return ( i0 < i1 ? ( ( i0 + i1 ) / 2 )
            : ( i0 + i1 + N ) / 2 ) % N;
    }

    template <typename Point, typename PointSequence, typename NumberComparisonPolicy>
    inline bool point_in_subpolygon( const Point& p, const PointSequence& polygon, int i0, int i1, const NumberComparisonPolicy& compare )
    {        
        const Point& v0 = point_sequence_traits<PointSequence>::get_point( polygon, i0 );
        const Point& v1 = point_sequence_traits<PointSequence>::get_point( polygon, i1 );

        int N = polygon.size() - 1;

        //! If the indices are adjacent then check the final segment.
        if( ( (i1 - i0 + N) % N ) == 1 )
        {
            BOOST_ASSERT( dot_product( perp( v1 - v0 ), p - v0 ) == cross_product( v1 - v0, p - v0 ) );
            orientation_type oType = get_orientation( v0, v1, p, compare );
            return oType == oriented_left || oType == oriented_collinear;
        }

        int mid = get_middle_index( i0, i1, N );
        const Point& midP = point_sequence_traits<PointSequence>::get_point( polygon, mid );

        BOOST_ASSERT( dot_product( perp( midP - v0 ), p - v0 ) == cross_product( midP - v0, p - v0 ) );
        //if( cross_product( midP - v0, p - v0 ) > 0 )
        if( get_orientation( v0, midP, p, compare ) == oriented_right )
            return point_in_subpolygon( p, polygon, i0, mid, compare );
        else
            return point_in_subpolygon( p, polygon, mid, i1, compare );
    }

    template <typename Point, typename PointSequence, typename NumberComparisonPolicy>
    inline bool point_in_convex_polygon( const Point& p, const PointSequence& polygon, const NumberComparisonPolicy& compare )
    {
        boost::function_requires< PointSequenceConcept< PointSequence > >();        
        BOOST_ASSERT( point_sequence_traits< PointSequence >::size( polygon ) > 2 );

        assert( numeric_sequence_equals( polygon.front(), polygon.back(), compare ) );//needs to be a closed boundary.

        return point_in_subpolygon( p, polygon, 0, 0, compare );
    }

    template <typename Point, typename PointSequence, typename NumberComparisonPolicy>
    inline bool point_in_convex_quadrilateral( const Point& p, const PointSequence& polygon, const NumberComparisonPolicy& compare )
    {
        boost::function_requires< PointSequenceConcept< PointSequence > >();        
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
}}}

BOOST_AUTO_TEST_CASE( TestPointSequences )
{
	using namespace generative::numeric::geometry;

	typedef point_double_2d point_2d;
	typedef point_double_3d point_3d;
	
    std::vector< point_2d > polygon;
    
    polygon.push_back( point_2d( 0., 0. ) ); 
    polygon.push_back( point_2d( 10., 0. ) ); 
    polygon.push_back( point_2d( 10., 10. ) ); 
    polygon.push_back( point_2d( 0., 10. ) ); 
    polygon.push_back( point_2d( 0., 0. ) ); 

    double area = get_area( polygon, fraction_tolerance_comparison_policy<double>(1e-10) );
    BOOST_CHECK_CLOSE( area, 100.0, 1e-10 );

    point_2d centroid = get_centroid( polygon, fraction_tolerance_comparison_policy<double>(1e-10) );
    BOOST_CHECK( numeric_sequence_equals( centroid, point_2d( 5., 5. ), fraction_tolerance_comparison_policy<double>(1e-10) ) );

    BOOST_CHECK( point_in_polygon( centroid, polygon ) );
    BOOST_CHECK( point_in_convex_polygon( centroid, polygon, fraction_tolerance_comparison_policy<double>(1e-10) ) );
    BOOST_CHECK( point_in_convex_quadrilateral( centroid, polygon, fraction_tolerance_comparison_policy<double>(1e-10) ) );

    BOOST_CHECK( !point_in_polygon( point_2d( -5, -5 ), polygon ) );
    BOOST_CHECK( !point_in_convex_polygon( point_2d( -5, -5 ), polygon, fraction_tolerance_comparison_policy<double>(1e-10) ) );
    BOOST_CHECK( !point_in_convex_quadrilateral( point_2d( -5, -5 ), polygon, fraction_tolerance_comparison_policy<double>(1e-10) ) );

    BOOST_CHECK( point_in_polygon( point_2d( 5, 6 ), polygon ) );
    BOOST_CHECK( point_in_convex_polygon( point_2d( 5, 6 ), polygon, fraction_tolerance_comparison_policy<double>(1e-10) ) );
    BOOST_CHECK( point_in_convex_quadrilateral( point_2d( 5, 6 ), polygon, fraction_tolerance_comparison_policy<double>(1e-10) ) );

    BOOST_CHECK( point_in_polygon( point_2d( 5, 4 ), polygon ) );
    BOOST_CHECK( point_in_convex_polygon( point_2d( 5, 4 ), polygon, fraction_tolerance_comparison_policy<double>(1e-10) ) );
    BOOST_CHECK( point_in_convex_quadrilateral( point_2d( 5, 4 ), polygon, fraction_tolerance_comparison_policy<double>(1e-10) ) );

    BOOST_CHECK( !point_in_polygon( point_2d( 5, 11 ), polygon ) );
    BOOST_CHECK( !point_in_convex_polygon( point_2d( 5, 11 ), polygon, fraction_tolerance_comparison_policy<double>(1e-10) ) );
    BOOST_CHECK( !point_in_convex_quadrilateral( point_2d( 5, 11 ), polygon, fraction_tolerance_comparison_policy<double>(1e-10) ) );

}

#endif //GENERATIVE_GEOMETRY_POINT_SEQUENCE_TESTS_HPP
