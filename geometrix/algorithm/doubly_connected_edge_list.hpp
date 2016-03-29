//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_DOUBLY_CONNECTED_EDGE_LIST_HPP
#define GEOMETRIX_DOUBLY_CONNECTED_EDGE_LIST_HPP
#pragma once

#include <geometrix/primitive/point_sequence_traits.hpp>
#include <geometrix/utility/point_angle_compare.hpp>
#include <geometrix/primitive/point_sequence_utilities.hpp>
#include <geometrix/utility/utilities.hpp>

#include <boost/graph/edge_list.hpp>
#include <boost/graph/adjacency_list.hpp>

//! \internal Install graph properties into the boost namespace.
namespace boost
{
    enum vertex_position_t 
    { 
        vertex_position = 1313//TODO: What's a good number?
    }; 
    BOOST_INSTALL_PROPERTY(vertex, position);
}

namespace geometrix
{     
    template <typename Point, typename NumberComparisonPolicy>
    class doubly_connected_edge_list
    {
    public:

        typedef Point                     point_type;
		typedef typename geometric_traits<point_type>::arithmetic_type arithmetic_type;
        typedef std::vector< point_type > face;
        typedef std::vector< face >       face_collection;

        typedef boost::property<boost::vertex_position_t, point_type> vertex_properties;        
        typedef boost::property<boost::edge_weight_t, arithmetic_type, boost::property< boost::edge_index_t, std::size_t >> edge_properties;
    
        typedef boost::adjacency_list
        <   boost::vecS,           // edges list 
            boost::vecS,           // vertex list
            boost::bidirectionalS, // directed graph         
            vertex_properties,     // vertex properties - point 
            edge_properties        // edge properties, weight and segment edge
        > half_edge_list; 

        typedef typename boost::graph_traits< half_edge_list >::vertex_descriptor vertex_descriptor;
        typedef typename boost::graph_traits< half_edge_list >::edge_descriptor   edge_descriptor;
        
        typedef std::map< point_type, vertex_descriptor, lexicographical_comparer< NumberComparisonPolicy > > point_vertex_map;
        
        doubly_connected_edge_list()
        {}

        doubly_connected_edge_list( const NumberComparisonPolicy& compare )
            : m_pointVertexMap( compare )
            , m_compare( compare )
        {}

		doubly_connected_edge_list(const doubly_connected_edge_list& other)
			: m_pointVertexMap(other.m_pointVertexMap)
			, m_compare(other.m_compare)
		{}

		doubly_connected_edge_list(doubly_connected_edge_list&& other)
			: m_pointVertexMap(std::move<point_vertex_map>(other.m_pointVertexMap))
			, m_compare( other.m_compare)
		{}

		template <typename Segments>
		doubly_connected_edge_list(const Segments& segs, const NumberComparisonPolicy& compare)
			: m_pointVertexMap(compare)
			, m_compare(compare)
		{
			for (const auto& seg : segs)
				add_edge(seg);
			calculate_faces();
		}

        //! add an edge from a segment.
        template <typename Segment>
        void add_edge( const Segment& edge )
        {
            add_edge( get_start( edge ), get_end( edge ) );
        }

        //! add an edge from a point.
        void add_edge( const point_type& source, const point_type& target )
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
                    boost::put( boost::edge_weight, m_graph, e, point_point_distance( source, target ) );
                    m_edgesContained.insert( std::make_pair( s, t ) );
                }
            }
        }

        //! method to extract the faces (external boundaries counter clockwise, holes clockwise )        
        const face_collection& get_faces() const
        {
            return m_faces;            
        }

        //! method to get the edges (in no particular order).
        template <typename Segment>
        std::vector<Segment> get_edges() const
        {
            std::vector<Segment> edges;
            typename boost::graph_traits< half_edge_list >::edge_iterator ei, ei_end;
            for( boost::tie( ei, ei_end ) = boost::edges( m_graph ); ei != ei_end; ++ei )
            {   
                edge_descriptor e = *ei;
                vertex_descriptor s = boost::source( e, m_graph );
                vertex_descriptor t = boost::target( e, m_graph );
                edges.push_back( construct<Segment>( boost::get( boost::vertex_position, m_graph, s ), boost::get( boost::vertex_position, m_graph, t ) ) );
            }
            
            return std::move(edges);
        }

		//! Method to calculate the connected components and set the faces        
		void calculate_faces()
		{
			m_faces.clear();

			std::size_t edge_count = 0;
			typename boost::graph_traits<half_edge_list>::edge_iterator ei, ei_end;

			boost::property_map<half_edge_list, boost::edge_index_t>::type e_index = boost::get(boost::edge_index, m_graph);
			for (boost::tie(ei, ei_end) = boost::edges(m_graph); ei != ei_end; ++ei)
			{
				edge_descriptor e = *ei;
				vertex_descriptor s = boost::source(e, m_graph);
				vertex_descriptor t = boost::target(e, m_graph);
				boost::put(boost::edge_index, m_graph, e, edge_count);
				++edge_count;
			}

			face currentFace;
			std::set< std::size_t > visitedEdges;
			for (boost::tie(ei, ei_end) = boost::edges(m_graph); ei != ei_end; ++ei)
			{
				edge_descriptor e = *ei;
				std::size_t eIndex = boost::get(boost::edge_index, m_graph, e);

				edge_descriptor eD = e;

				if (visitedEdges.find(eIndex) == visitedEdges.end())
				{
					currentFace.clear();
					
					while (visitedEdges.find(eIndex) == visitedEdges.end())
					{
						vertex_descriptor s = boost::source(e, m_graph);
						vertex_descriptor t = boost::target(e, m_graph);

						point_type sPoint = boost::get(boost::vertex_position, m_graph, s);						
						currentFace.push_back(boost::get(boost::vertex_position, m_graph, s));

						boost::graph_traits< half_edge_list >::out_edge_iterator oei, oei_end;
						boost::tie(oei, oei_end) = boost::out_edges(t, m_graph);
						std::size_t outDegree = boost::out_degree(t, m_graph);
						if (outDegree > 0)
						{
							if (outDegree > 1)
							{
								//find the next edges by sorting them relative to this ones target.
								typedef point_angle_compare< NumberComparisonPolicy > winding_compare; ;
								typedef std::pair< point_type, typename boost::graph_traits< half_edge_list >::out_edge_iterator > winding_pair;
								typedef std::set< winding_pair, pair_first_compare< winding_compare > > winding_sorter;
								winding_sorter windingSorter(pair_first_compare< winding_compare >(winding_compare(boost::get(boost::vertex_position, m_graph, t), m_compare)));
								for (; oei != oei_end; ++oei)
								{
									vertex_descriptor v = boost::target(*oei, m_graph);
									if (v != s)
										windingSorter.insert(std::make_pair(boost::get(boost::vertex_position, m_graph, v), oei));
								}

								winding_sorter::iterator sIter = windingSorter.lower_bound(std::make_pair(sPoint, oei_end));
								if (sIter != windingSorter.end())
									e = *(sIter->second);
							}
							else
								e = *oei;

							visitedEdges.insert(eIndex);
							eIndex = boost::get(boost::edge_index, m_graph, e);
						}
					}

					//! Polygons don't need the back added
					//currentFace.push_back(boost::get(boost::vertex_position, m_graph, boost::source(e, m_graph)));
					GEOMETRIX_ASSERT(numeric_sequence_equals(currentFace[0], boost::get(boost::vertex_position, m_graph, boost::source(e, m_graph)), m_compare));

					m_faces.push_back(std::move(currentFace));
				}
			}
		}

		doubly_connected_edge_list& operator = (doubly_connected_edge_list&& other)
		{
			m_pointVertexMap = std::move<point_vertex_map>(other.m_pointVertexMap);
			m_compare = other.m_compare;
			return *this;
		}

		doubly_connected_edge_list& operator = (const doubly_connected_edge_list& other)
		{
			m_pointVertexMap = other.m_pointVertexMap;
			m_compare = other.m_compare;
			return *this;
		}

    private:

        //! Method to lookup a vertex or add it if not already in place.
        vertex_descriptor add_vertex( const point_type& p )
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
		        
        mutable point_vertex_map                                      m_pointVertexMap;
        mutable half_edge_list                                        m_graph;
        mutable face_collection                                       m_faces;
        NumberComparisonPolicy                                        m_compare;
        std::set< std::pair< vertex_descriptor, vertex_descriptor > > m_edgesContained;

    };

	template <typename Point, typename Segments, typename NumberComparisonPolicy>
	inline doubly_connected_edge_list<Point, NumberComparisonPolicy> make_dcel(const Segments& segs, const NumberComparisonPolicy& cmp)
	{
		return doubly_connected_edge_list<Point, NumberComparisonPolicy>(segs, cmp);
	}

}//namespace geometrix;

#endif //GEOMETRIX_DOUBLY_CONNECTED_EDGE_LIST_HPP
