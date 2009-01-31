//
//! Copyright © 2008
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_DOUBLY_CONNECTED_EDGE_LIST_HPP
#define _BOOST_GEOMETRY_DOUBLY_CONNECTED_EDGE_LIST_HPP
#pragma once

#include "point_sequence_traits.hpp"
#include <boost/graph/edge_list.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/planar_face_traversal.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>

namespace boost
{
    //! Install some properties.
	enum vertex_position_t 
	{ 
		vertex_position = 1313//TODO: What's a good number?
	}; 
	BOOST_INSTALL_PROPERTY(vertex, position);

namespace numeric
{
namespace geometry
{  
    
    template <typename Point, typename NumberComparisonPolicy>
    class doubly_connected_edge_list
    {
    public:

        typedef Point                                                point_type;
        typedef typename point_traits< point_type >::coordinate_type coordinate_type;
        typedef std::vector< point_type >                            face_type;
        typedef boost::shared_ptr< face_type >                       face_ptr;
        typedef std::vector< face_ptr >                              face_collection;
        typedef boost::shared_ptr< face_collection >                 face_collection_ptr;

        typedef boost::property< boost::vertex_position_t, point_type >         vertex_properties;        
        typedef boost::property< boost::edge_weight_t, coordinate_type, boost::property< boost::edge_index_t, size_t > >        edge_properties;
    
        typedef boost::adjacency_list
        <   boost::vecS,           // edges list 
            boost::vecS,           // vertex list
            boost::bidirectionalS, // directed graph         
            vertex_properties,     // vertex properties - point 
            edge_properties        // edge properties, weight and segment edge
        > half_edge_list; 

        typedef typename boost::graph_traits< half_edge_list >::vertex_descriptor vertex_descriptor;
        typedef typename boost::graph_traits< half_edge_list >::edge_descriptor   edge_descriptor;
        
        typedef std::map< point_type, vertex_descriptor, lexicographical_compare< NumberComparisonPolicy > > point_vertex_map;
        
        doubly_connected_edge_list()
        {}

        doubly_connected_edge_list( typename const NumberComparisonPolicy& compare )
            : m_pointVertexMap( compare )
            , m_compare( compare )
        {}

        //! add an edge from a segment.
        template <typename Segment>
        inline void add_edge( const Segment& edge )
        {
            add_edge( segment_access_traits< Segment >::get_start( edge ), segment_access_traits< Segment >::get_end( edge ) );
        }

        //! add an edge from a point.
        inline void add_edge( const point_type& source, const point_type& target )
        {
            vertex_descriptor s = add_vertex( source );
            vertex_descriptor t = add_vertex( target );

            if( m_edgesContained.find( std::make_pair( s, t ) ) == m_edgesContained.end() )
            {
                edge_descriptor e;
                bool inserted;
                boost::tie( e, inserted ) = boost::add_edge( s, t, m_graph );
                if( inserted )
                {
                    m_pFaces.reset();
                    boost::put( boost::edge_weight, m_graph, e, euclidean_distance( source, target ) );
                    m_edgesContained.insert( std::make_pair( s, t ) );
                }
            }
        }

        //! method to extract the faces (external boundaries counter clockwise, holes clockwise )        
        inline face_collection_ptr get_faces() const
        {
            if( !m_pFaces )
            {
                calculate_faces();
            }
            return m_pFaces;            
        }

        //! method to get the edges (in no particular order).
        template <typename Segment>
        inline boost::shared_ptr< std::vector< Segment > > get_edges() const
        {
            boost::shared_ptr< std::vector< Segment > > pEdges( new std::vector< Segment >() );
            boost::graph_traits< half_edge_list >::edge_iterator ei, ei_end;
            for( boost::tie( ei, ei_end ) = boost::edges( m_graph ); ei != ei_end; ++ei )
            {   
                edge_descriptor e = *ei;
                vertex_descriptor s = boost::source( e, m_graph );
                vertex_descriptor t = boost::target( e, m_graph );
                pEdges->push_back( construction_traits< Segment >::construct( boost::get( vertex_position, m_graph, s ), boost::get( vertex_position, m_graph, t ) ) );
            }
            
            return pEdges;
        }

    private:

        //! Method to lookup a vertex or add it if not already in place.
        inline vertex_descriptor add_vertex( const point_type& p )
        {
            point_vertex_map::iterator pvIter( m_pointVertexMap.lower_bound( p ) );
            vertex_descriptor v;
            if( pvIter != m_pointVertexMap.end() && !( m_pointVertexMap.key_comp()( p, pvIter->first ) ) )
            {
                v = pvIter->second;
            }
            else
            {
                v = boost::add_vertex( m_graph );
                m_pointVertexMap.insert( pvIter, std::make_pair( p, v ) );
                boost::put( boost::vertex_position, m_graph, v, p );
            }

            return v;
        }

        // Some planar face traversal visitors that will 
        // print the vertices and edges on the faces
        template <typename Graph>
        struct output_visitor : public planar_face_traversal_visitor
        {
            output_visitor( face_collection_ptr& pFaces, Graph& graph )
                : m_pFaces( pFaces )
                , m_graph( graph )
            {}
            
            void begin_face() { m_pCurrentFace.reset( new face_type() ); }
            void end_face() { m_pFaces->push_back( m_pCurrentFace ); }

            template <typename Vertex> 
            void next_vertex(Vertex v) 
            { 
                add_vertex( v );
            }

            template <typename Vertex>
            void add_vertex( Vertex v ) 
            {
                m_pCurrentFace->push_back( boost::get( boost::vertex_position, m_graph, v ) );
            }

            face_ptr m_pCurrentFace;
            face_collection_ptr m_pFaces;          
            Graph& m_graph;
        };
        
        //! Method to calculate the connected components and set the faces        
        inline void calculate_faces() const
        {
            m_pFaces.reset( new face_collection() );
            
            size_t edge_count = 0;
            boost::graph_traits<half_edge_list>::edge_iterator ei, ei_end;
           
            boost::property_map<half_edge_list, boost::edge_index_t>::type e_index = boost::get( boost::edge_index, m_graph );            
            for( boost::tie(ei, ei_end) = boost::edges( m_graph ); ei != ei_end; ++ei )
            {
                edge_descriptor e = *ei;
                vertex_descriptor s = boost::source( e, m_graph );
                vertex_descriptor t = boost::target( e, m_graph );
                boost::put( boost::edge_index, m_graph, e, edge_count );
                ++edge_count;
            }

            face_ptr pCurrentFace;       

            std::set< size_t > visitedEdges;
            for( boost::tie( ei, ei_end ) = boost::edges( m_graph ); ei != ei_end; ++ei )
            {   
                edge_descriptor e = *ei;
                size_t eIndex = boost::get( boost::edge_index, m_graph, e );

                edge_descriptor eD = e;                
                //std::cout << "(" << cartesian_access_traits< point_type >::get<0>( boost::get( boost::vertex_position, m_graph, boost::source( eD, m_graph ) ) ) << "," << cartesian_access_traits< point_type >::get<1>( boost::get( boost::vertex_position, m_graph, boost::source( eD, m_graph ) ) ) << "--";
                //std::cout << "(" << cartesian_access_traits< point_type >::get<0>( boost::get( boost::vertex_position, m_graph, boost::target( eD, m_graph ) ) ) << "," << cartesian_access_traits< point_type >::get<1>( boost::get( boost::vertex_position, m_graph, boost::target( eD, m_graph ) ) ) << std::endl;
            
                if( visitedEdges.find( eIndex ) == visitedEdges.end() )
                {
                    pCurrentFace.reset( new face_type() );
                    //std::cout << "---------New Face-----------" << std::endl;

                    while( visitedEdges.find( eIndex ) == visitedEdges.end() )
                    {
                        vertex_descriptor s = boost::source( e, m_graph );
                        vertex_descriptor t = boost::target( e, m_graph );

                        point_type sPoint = boost::get( vertex_position, m_graph, s );
                        //std::cout << "Adding: (" << cartesian_access_traits< point_type >::get<0>( sPoint ) << "," << cartesian_access_traits< point_type >::get<1>( sPoint ) << std::endl;

                        pCurrentFace->push_back( boost::get( vertex_position, m_graph, s ) );

                        boost::graph_traits< half_edge_list >::out_edge_iterator oei, oei_end;            
                        boost::tie( oei, oei_end ) = boost::out_edges( t, m_graph );      
                        size_t outDegree = boost::out_degree( t, m_graph );
                        if( outDegree > 0 )
                        {
                            if( outDegree > 1 )
                            {
                                //find the next edges by sorting them relative to this ones target.
                                typedef orientation_compare< point_type, NumberComparisonPolicy > winding_compare;
                                winding_compare windRule( boost::get( boost::vertex_position, m_graph, t ), oriented_left, m_compare );
                                pair_first_compare< winding_compare > leftTurnCompare( windRule );
                                typedef std::pair< point_type,  boost::graph_traits< half_edge_list >::out_edge_iterator > winding_pair;
                                typedef std::set< winding_pair, pair_first_compare< winding_compare > > winding_sorter;
                                winding_sorter windingSorter( leftTurnCompare );
                                for( ; oei != oei_end; ++oei )
                                {
                                    vertex_descriptor v = boost::target( *oei, m_graph );
                                    if( v != s )
                                    {
                                        windingSorter.insert( std::make_pair( boost::get( boost::vertex_position, m_graph, v ), oei ) );
                                    }
                                }

                                winding_sorter::iterator sIter = windingSorter.lower_bound( std::make_pair( sPoint, oei_end ) ); 
                                if( sIter != windingSorter.end() )
                                {
                                    e = *(sIter->second);
                                }
                            }
                            else
                            {
                                e = *oei;
                            }

                            visitedEdges.insert( eIndex );
                            eIndex = boost::get( boost::edge_index, m_graph, e );
                        }                        
                    }

                    pCurrentFace->push_back( boost::get( vertex_position, m_graph, boost::source( e, m_graph ) ) );
                    m_pFaces->push_back( pCurrentFace ); 
                }
            }
        }

        mutable point_vertex_map                                      m_pointVertexMap;
        mutable half_edge_list                                        m_graph;
        mutable face_collection_ptr                                   m_pFaces;
        NumberComparisonPolicy                                        m_compare;
        std::set< std::pair< vertex_descriptor, vertex_descriptor > > m_edgesContained;

    };

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_DOUBLY_CONNECTED_EDGE_LIST_HPP
