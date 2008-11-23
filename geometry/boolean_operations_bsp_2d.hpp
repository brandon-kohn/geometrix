//
//! Copyright © 2008
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_BOOLEAN_OPERATIONS_BSP_2D_HPP
#define _BOOST_GEOMETRY_BOOLEAN_OPERATIONS_BSP_2D_HPP
#pragma once

#include "bsp_tree_2d.hpp"
#include "segment.hpp"
#include "doubly_connected_edge_list.hpp"

namespace boost
{
namespace numeric
{
namespace geometry
{

    //! function to negate a bsp tree
    template <typename Segment, typename NumberComparisonPolicy>
    inline boost::shared_ptr< bsp_tree_2d< Segment, NumberComparisonPolicy > > bsp_tree_negation( const bsp_tree_2d< Segment, NumberComparisonPolicy >& tree ) { return tree.negatation(); }

    template <typename Polygon, typename NumberComparisonPolicy>
    inline boost::shared_ptr< doubly_connected_edge_list< typename point_sequence_traits< Polygon >::point_type, NumberComparisonPolicy > > create_dcel( const Polygon& p, const NumberComparisonPolicy& compare )
    {
        typedef typename point_sequence_traits< Polygon >::point_type point_type;
        
        typedef doubly_connected_edge_list<point_type, NumberComparisonPolicy> dcel;
        typedef boost::shared_ptr< dcel > dcel_ptr;
        dcel_ptr pfaces( new dcel( compare ) );

        point_sequence_traits< Polygon >::const_iterator pNext = point_sequence_traits< Polygon >::begin( p ); 
        point_sequence_traits< Polygon >::const_iterator pIt = pNext++;
        point_sequence_traits< Polygon >::const_iterator pEnd = point_sequence_traits< Polygon >::end( p );        
        while( pNext != pEnd )
        {
            pfaces->add_edge( *pIt++, *pNext++ );
        }

        if( !equals( point_sequence_traits< Polygon >::front( p ), point_sequence_traits< Polygon >::back( p ), compare ) )
        {
            pfaces->add_edge( point_sequence_traits< Polygon >::back( p ), point_sequence_traits< Polygon >::front( p ) );
        }
        
        return pfaces;
    }

    template <typename Point, typename NumberComparisonPolicy>
    inline boost::shared_ptr< doubly_connected_edge_list< Point, NumberComparisonPolicy > > dcel_negation( const doubly_connected_edge_list<Point,NumberComparisonPolicy>& p, const NumberComparisonPolicy& compare )
    {        
        typedef doubly_connected_edge_list<Point, NumberComparisonPolicy> dcel;
        typedef boost::shared_ptr< dcel > dcel_ptr;
        dcel_ptr pfaces( new dcel( compare ) );

        boost::shared_ptr< std::vector< segment< Point > > > pEdges = p.get_edges< segment<Point> >();

        BOOST_FOREACH( const segment<Point>& edge, *pEdges )
        {
            pfaces->add_edge( edge.get_end(), edge.get_start() );
        }
        
        return pfaces;
    }

    //! function to negate a polygon
    template <typename Polygon, typename NumberComparisonPolicy>
    inline boost::shared_ptr< doubly_connected_edge_list< typename point_sequence_traits< Polygon >::point_type, NumberComparisonPolicy > >  polygon_negation( const Polygon& polygon, const NumberComparisonPolicy& compare )
    {        
        return create_dcel( *construction_traits< Polygon >::construct( std::make_pair( point_sequence_traits< Polygon >::rbegin( polygon ), point_sequence_traits< Polygon >::rend( polygon ) ) ), compare );
    }
    
    template <typename Polygon, typename NumberComparisonPolicy>
    inline boost::shared_ptr< doubly_connected_edge_list< typename point_sequence_traits< Polygon >::point_type, NumberComparisonPolicy > > boolean_intersection( const Polygon& p, const Polygon& q, const NumberComparisonPolicy& compare )
    {
        typedef typename point_sequence_traits< Polygon >::point_type point_type;

        if( point_sequence_traits< Polygon >::empty( p ) || point_sequence_traits< Polygon >::empty( q ) )
        {
            //TODO: what do we do throw?   
        }

        typedef doubly_connected_edge_list<point_type, NumberComparisonPolicy> dcel;
        typedef boost::shared_ptr< dcel > dcel_ptr;
        dcel_ptr pfaces( create_dcel( p, compare ) );
        dcel_ptr qfaces( create_dcel( q, compare ) );
        return dcel_boolean_intersection( *pfaces, *qfaces, compare );
    }

    template <typename Polygon, typename NumberComparisonPolicy>
    inline boost::shared_ptr< doubly_connected_edge_list< typename point_sequence_traits< Polygon >::point_type, NumberComparisonPolicy > > boolean_union( const Polygon& p, const Polygon& q, const NumberComparisonPolicy& compare )
    {
        return dcel_negation( *dcel_boolean_intersection( *polygon_negation( p, compare ), *polygon_negation( q, compare ), compare ), compare );
    }

    template <typename Polygon, typename NumberComparisonPolicy>
    inline boost::shared_ptr< doubly_connected_edge_list< typename point_sequence_traits< Polygon >::point_type, NumberComparisonPolicy > > boolean_difference( const Polygon& p, const Polygon& q, const NumberComparisonPolicy& compare )
    {
        typedef typename point_sequence_traits< Polygon >::point_type point_type;
        typedef doubly_connected_edge_list<point_type, NumberComparisonPolicy> dcel;
        typedef boost::shared_ptr< dcel > dcel_ptr;
        dcel_ptr pfaces( create_dcel( p, compare ) );
        return dcel_boolean_intersection( *pfaces, *polygon_negation( q, compare ), compare );
    }

    template <typename Polygon, typename NumberComparisonPolicy>
    inline boost::shared_ptr< doubly_connected_edge_list< typename point_sequence_traits< Polygon >::point_type, NumberComparisonPolicy > > boolean_exclusive_or( const Polygon& p, const Polygon& q, const NumberComparisonPolicy& compare )
    {
        typedef typename point_sequence_traits< Polygon >::point_type point_type;
        typedef doubly_connected_edge_list<point_type, NumberComparisonPolicy> dcel;
        typedef boost::shared_ptr< dcel > dcel_ptr;
        dcel_ptr pfaces( create_dcel( p, compare ) );
        dcel_ptr qfaces( create_dcel( q, compare ) );
        return dcel_boolean_exclusive_or( *pfaces, *qfaces, compare );        
    }

    template <typename Point, typename NumberComparisonPolicy>
    inline boost::shared_ptr< doubly_connected_edge_list< Point, NumberComparisonPolicy > > dcel_boolean_intersection( const doubly_connected_edge_list<Point,NumberComparisonPolicy>& p, const doubly_connected_edge_list<Point,NumberComparisonPolicy>& q, const NumberComparisonPolicy& compare )
    {
        typedef Point                 point_type;
        typedef segment< point_type > segment_type;

        typedef doubly_connected_edge_list<point_type, NumberComparisonPolicy> dcel;
        typedef boost::shared_ptr< dcel > dcel_ptr;
        dcel_ptr faces( new dcel( compare ) );

        boost::shared_ptr< std::vector< segment_type > > pEdges = p.get_edges< segment_type >();
        boost::shared_ptr< std::vector< segment_type > > qEdges = q.get_edges< segment_type >();

        //! create bsp for q.
        bsp_tree_2d< segment_type, NumberComparisonPolicy > Q( qEdges->begin(), qEdges->end(), compare );
        
        BOOST_FOREACH( const segment_type& edge, *pEdges )
        {
            std::vector< segment_type > inside, outside, coincidentSame, coincidentDiff;
            Q.get_partition( edge, inside, outside, coincidentSame, coincidentDiff );
            BOOST_FOREACH( const segment_type& s, coincidentSame )
            {
                faces->add_edge( s.get_start(), s.get_end() );
            }

            BOOST_FOREACH( const segment_type& s, inside )
            {
                faces->add_edge( s.get_start(), s.get_end() );
            }
        }

        //! create bsp for P.
        bsp_tree_2d< segment_type, NumberComparisonPolicy > P( pEdges->begin(), pEdges->end(), compare );
   
        BOOST_FOREACH( const segment_type& edge, *qEdges )
        {
            std::vector< segment_type > inside, outside, coincidentSame, coincidentDiff;
            P.get_partition( edge, inside, outside, coincidentSame, coincidentDiff );
            BOOST_FOREACH( const segment_type& s, coincidentSame )
            {
                faces->add_edge( s.get_start(), s.get_end() );
            }

            BOOST_FOREACH( const segment_type& s, inside )
            {
                faces->add_edge( s.get_start(), s.get_end() );
            }
        }

        return faces;

    }

    template <typename Point, typename NumberComparisonPolicy>
    inline boost::shared_ptr< doubly_connected_edge_list< Point, NumberComparisonPolicy > > dcel_boolean_union( const doubly_connected_edge_list<Point,NumberComparisonPolicy>& p, const doubly_connected_edge_list<Point,NumberComparisonPolicy>& q, const NumberComparisonPolicy& compare )
    {        
        return dcel_negation( *dcel_boolean_intersection( *dcel_negation( p, compare ), *dcel_negation( q, compare ), compare ), compare );
    }

    template <typename Point, typename NumberComparisonPolicy>
    inline boost::shared_ptr< doubly_connected_edge_list< Point, NumberComparisonPolicy > > dcel_boolean_difference( const doubly_connected_edge_list<Point,NumberComparisonPolicy>& p, const doubly_connected_edge_list<Point,NumberComparisonPolicy>& q, const NumberComparisonPolicy& compare )
    {        
        return dcel_boolean_intersection( p, *dcel_negation( q, compare ), compare );
    }

    template <typename Point, typename NumberComparisonPolicy>
    inline boost::shared_ptr< doubly_connected_edge_list< Point, NumberComparisonPolicy > > dcel_boolean_exclusive_or( const doubly_connected_edge_list<Point,NumberComparisonPolicy>& p, const doubly_connected_edge_list<Point,NumberComparisonPolicy>& q, const NumberComparisonPolicy& compare )
    {        
        return dcel_boolean_union( *dcel_boolean_difference( p , q, compare ), *dcel_boolean_difference( q, p, compare ), compare );
    }

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_BOOLEAN_OPERATIONS_BSP_2D_HPP
