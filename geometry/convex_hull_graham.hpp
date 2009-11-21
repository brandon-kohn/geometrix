//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GENERATIVE_GEOMETRY_CONVEX_HULL_GRAHAM_HPP
#define GENERATIVE_GEOMETRY_CONVEX_HULL_GRAHAM_HPP
#pragma once

#include <geometry\point_sequence_traits.hpp>
#include <geometry\products.hpp>
#include <geometry\utilities.hpp>

namespace generative
{
namespace numeric
{
namespace geometry
{
    //! Convex Hull algorithm data and functionality
    //! as implemented in O'Rourke Computational Geometry in C.
    //!
    class graham_scan
    {
    public:

        //! Method to calculate the convex hull from an array of points.
        template <typename Polygon, typename PointSequence, typename NumberComparisonPolicy>
        static boost::shared_ptr< Polygon > get_convex_hull( PointSequence& points, const NumberComparisonPolicy& compare );

    private:

        template <typename Point, typename NumberComparisonPolicy>
        class graham_compare
        {
        public:
            
            typedef Point point_type;
            
            graham_compare()
            {}
            
            graham_compare( const NumberComparisonPolicy& compare )
                : m_compare( compare )
            {}

            //! Method to set the relative point
            void set_point(const point_type& point) { m_point = point; }
            
            //! Compare functor operator used in sorting routine
            bool operator () (const point_type& p1, const point_type& p2)
            {
                double area = cross_product( m_point, p1, p2 );
                return ( m_compare.greater_than( area, 0.) );
            }
            
        private:
            
            point_type m_point; /// The point used for the relative compare.
            NumberComparisonPolicy m_compare;
            
        };

        //! Method to reorder the points into the proper ordering with lowest point on top.
        template <typename Polygon, typename NumberComparisonPolicy>
        static void find_lowest( Polygon& points, const NumberComparisonPolicy& compare );

    };

    //! Method to calculate the convex hull from an array of points.
    template <typename Polygon, typename PointSequence, typename NumberComparisonPolicy>
    boost::shared_ptr< Polygon > graham_scan::get_convex_hull( PointSequence& points, const NumberComparisonPolicy& compare )
    {
        typedef typename point_sequence_traits<Polygon>::point_type point_type;
        typedef cartesian_access_traits< point_type >               access;
        boost::function_requires< CartesianCoordinateAccessorConcept< access > >();

        find_lowest( points, compare );
        Polygon::iterator iterFirst(++(point_sequence_traits< PointSequence >::begin( points )));
        graham_compare< point_type, NumberComparisonPolicy > gCompare( compare );
        gCompare.set_point( point_sequence_traits< PointSequence >::front( points ) );
        std::sort( iterFirst, points.end(), gCompare );

        unsigned int i = 2;
        typedef std::list< point_type > graham_stack;
        graham_stack gStack;
        gStack.push_front( point_sequence_traits< PointSequence >::front( points ) );
        gStack.push_front( point_sequence_traits< PointSequence >::get_point( points, 1 ) );

        size_t size = point_sequence_traits< PointSequence >::size( points );
        while( i < size )
        {
            graham_stack::iterator iter = gStack.begin();
            BOOST_ASSERT( iter != gStack.end() );
            point_type& p2 = *(iter++);
            BOOST_ASSERT( iter != gStack.end() );
            point_type& p1 = *iter;
            const point_type& pI = point_sequence_traits< PointSequence >::get_point( points, i );
            if( get_orientation( p1, p2, pI, compare ) == oriented_left )
            {
                gStack.push_front( pI );
                ++i;
            }
            else
            {
                gStack.erase( gStack.begin() );
            }
        }
        
        boost::shared_ptr< Polygon > pHull( construction_traits< Polygon >::construct( gStack ) );
        return pHull;
    }

    //! Method to reorder the points into the proper ordering with lowest point
    //! on top.
    template <typename PointSequence, typename NumberComparisonPolicy>
    void graham_scan::find_lowest( PointSequence& points, const NumberComparisonPolicy& compare )
    {
        typedef typename point_sequence_traits<PointSequence>::point_type point_type;
        typedef cartesian_access_traits< point_type >                     access;
        boost::function_requires< CartesianCoordinateAccessorConcept< access > >();

        size_t indexLowest = 0;
        size_t size = points.size();
        for( unsigned int i=1; i < size ; ++i )
        {
            point_type& lowestPoint = point_sequence_traits< PointSequence >::get_point( points, indexLowest );
            point_type& pI = point_sequence_traits< PointSequence >::get_point( points, i );

            if( ( compare.less_than( access::get<1>( pI ), access::get<1>( lowestPoint ) ) ) ||
                ( ( compare.equals( access::get<1>( pI ), access::get<1>( lowestPoint ) ) )  && 
                ( compare.greater_than( access::get<0>( pI ), access::get<0>( lowestPoint ) ) ) ) )
            {
                indexLowest = i;
                std::swap( *( point_sequence_traits< PointSequence >::begin( points ) ), lowestPoint );
            }
        }
    }
    
}}}//namespace generative::numeric::geometry;

#endif //GENERATIVE_GEOMETRY_CONVEX_HULL_GRAHAM_HPP
